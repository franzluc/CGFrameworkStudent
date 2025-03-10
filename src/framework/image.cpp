#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"
#include "application.h"
#include "entity.h"
#include <cmath>
#include <random>
#include <cstdlib>


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}



#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//Funcion para dibujar una linea
void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c)
{
	//Se calcula la diferencia entre los puntos
	int dx = x1 - x0;
	int dy = y1 - y0;

	//Se calculan los puntos intermedios que formar�n la l�nea
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	//Se calcula el incremento
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;

	//Se dibuja la linea
	float X = x0;
	float Y = y0;
	for (int i = 0; i <= steps; i++) {
		SetPixel(round(X), round(Y), c);
        SetPixel(round(X), round(Y), c);
		X += Xinc;
		Y += Yinc;
	}

}



void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table) {

	int dx = x1 - x0;
	int dy = y1 - y0;

	int d = std::max(abs(dx), abs(dy));

	Vector2 v(dx / static_cast<float>(d), dy / static_cast<float>(d));

	Vector2 A = Vector2(x0, y0);

	for (int i = 0; i < d; ++i) {
		if (table[A.y].minX > A.x) {
			table[A.y].minX = A.x;
		}

		if (table[A.y].maxX < A.x) {
			table[A.y].maxX = A.x;
		}

		A += v;
	}
}

void Image::DrawRect(int x, int y, int w, int h, const Color& bordercolor, int borderWidth, bool isFilled, const Color& fillColor)
{
    if (w > 0 && h < 0) { //Cuando la 1a posici�n del cursor y la segunda forman un vector diagonal hacia la parte inferior derecha
        for (int p = 0; p < borderWidth; ++p) {  //Este "for" es por el ancho del border en las coordenadas X
            for (int i = 0; i < w + (borderWidth - 1); ++i) {
                SetPixel(x + i, y + p, bordercolor);
                SetPixel(x + i, y + h - 1 + p, bordercolor);
            }
        }
        for (int q = 0; q < borderWidth; ++q) {  //Este "for" es por el ancho del border en las coordenadas Y
            for (int j = 0; j > h; --j) {
                SetPixel(x + q, y - abs(j), bordercolor);
                SetPixel(x + w - 1 + q, y - abs(j), bordercolor);
            }
        }
        //Si el rect�ngulo debe estar llenado de color:
        if (isFilled) {
            for (int i = 0; i < w - borderWidth - 1; ++i) {
                for (int j = 0; j > (h + borderWidth - 1); --j) {
                    SetPixel(x + i + borderWidth, y + j - 1, fillColor);
                }
            }
        }
    }
    else if (w < 0 && h < 0) { //Cuando la 1a posici�n del cursor y la segunda forman un vector diagonal hacia la parte inferior izquierda
        for (int p = 0; p < borderWidth; ++p) {  //Este "for" es por el ancho del border en las coordenadas X
            for (int i = 0; i > (w + (borderWidth - 1)); --i) {
                SetPixel(x - abs(i), y - p, bordercolor);
                SetPixel(x - abs(i), y + h + 1 + p, bordercolor);
            }
        }
        for (int q = 0; q < borderWidth; ++q) {  //Este "for" es por el ancho del border en las coordenadas Y
            for (int j = 0; j > h; --j) {
                SetPixel(x - q, y - abs(j), bordercolor);
                SetPixel(x - abs(w) + 1 + q, y - abs(j), bordercolor);
            }
        }
        //Si el rect�ngulo debe estar llenado de color:
        if (isFilled) {
            for (int i = 0; i > w + (borderWidth * 2); --i) {
                for (int j = 0; j > (h + borderWidth * 2); --j) {
                    SetPixel(x + i - borderWidth, y + j - borderWidth, fillColor);
                }
            }
        }
    }
    else if (w < 0 && h > 0) { //Cuando la 1a posici�n del cursor y la segunda forman un vector diagonal hacia la parte superior izquierda
        for (int p = 0; p < borderWidth; ++p) {  //Este "for" es por el ancho del border en las coordenadas X
            for (int i = 0; i > (w + (borderWidth - 1)); --i) {
                SetPixel(x - abs(i), y - p, bordercolor);
                SetPixel(x - abs(i), y + h + 1 + p, bordercolor);
            }
        }
        for (int q = 0; q < borderWidth; ++q) {  //Este "for" es por el ancho del border en las coordenadas Y
            for (int j = 0; j < (h + (borderWidth * 2)); ++j) {
                SetPixel(x + q, (y - borderWidth + 1) + j, bordercolor);
                SetPixel(x + q + w, (y - borderWidth + 1) + j, bordercolor);
            }
        }
        //Si el rect�ngulo debe estar llenado de color:
        if (isFilled) {
            for (int i = 0; i > (w + borderWidth); --i) {
                for (int j = 0; j < h; ++j) {
                    SetPixel(x + i - 1, y + j + 1, fillColor);
                }
            }
        }
    }
    else if (w > 0 && h > 0) { //Cuando la 1a posici�n del cursor y la segunda forman un vector diagonal hacia la parte superior derecha
        for (int p = 0; p < borderWidth; ++p) {  //Este "for" es por el ancho del border en las coordenadas X
            for (int i = 0; i < w + (borderWidth - 1); ++i) {
                SetPixel(x + i, y + p, bordercolor);
                SetPixel(x + i, y + h - 1 + p, bordercolor);
            }
        }
        for (int q = 0; q < borderWidth; ++q) {  //Este "for" es por el ancho del border en las coordenadas Y
            for (int j = 0; j < (h + borderWidth - 1); ++j) {
                SetPixel(x + q, y + j, bordercolor);
                SetPixel(x + q + w, y + j, bordercolor);
            }
        }
        //Si el rect�ngulo debe estar llenado de color:
        if (isFilled) {
            for (int i = 0; i < (w - borderWidth); ++i) {
                for (int j = 0; j < (h - borderWidth); ++j) {
                    SetPixel(x + borderWidth + i, y + borderWidth + j, fillColor);
                }
            }
        }
    }
}



// Funci�n para dibujar un circulo
void Image::DrawCircle(int x, int y, int r, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor) {
    
    for (int p = 0; p < borderWidth; ++p){
        for (int i=0; i < width; ++i){
            for (int j=0; j < height; ++j){
                if ((sqrt((i-x)*(i-x)+(j-y)*(j-y)) > (r-1-p)) && (sqrt((i-x)*(i-x)+(j-y)*(j-y)) < (r+1))){
                    SetPixel(i, j, borderColor);
                }
            }
        }
    }
    
    if(isFilled) {
        for (int i=0; i < width; ++i){
            for (int j=0; j < height; ++j){
                if ((sqrt((i-x)*(i-x)+(j-y)*(j-y)) < (r+1-borderWidth))){
                    SetPixel(i, j, fillColor);
                }
            }
        }
    }
}
    
// Funci�n para dibujar un triangulo
void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor) {

	std::vector<Cell> table;
	table.resize(height);

	ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table);
	ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table);
	ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table);

	for (int i = 0; i < table.size(); ++i) {
		if (table[i].minX < table[i].maxX) {
			for (int j = table[i].minX; j < table[i].maxX; ++j) {
				SetPixelSafe(j, i, fillColor);
			}
		}
	}
}

void Image::DrawImage(const Image& image, int x, int y, bool top) {
	Image imatge;
	imatge = image;
	Color* pixels;
	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {
			Color pixel = imatge.GetPixel(i, j);
            pixels = &pixel;
			SetPixel(x + i, y + j, *pixels);
		}
	}
}

void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2, FloatImage* zBuffer, Image* texture, const Vector2& uv0, const Vector2& uv1, const Vector2& uv2 ) {
    
    // Inicializamos la matriz que utilizaremos para obtener las coordenadas baric�ntricas y el vector con dichas coordenadas.
    Matrix44 bCoordsMatrix;
    Vector3 bCoords;
    
    // Asignamos los valores para la matriz
    bCoordsMatrix.M[0][0] = p0.x;
    bCoordsMatrix.M[1][0] = p1.x;
    bCoordsMatrix.M[2][0] = p2.x;
    bCoordsMatrix.M[0][1] = p0.y;
    bCoordsMatrix.M[1][1] = p1.y;
    bCoordsMatrix.M[2][1] = p2.y;
    bCoordsMatrix.M[0][2] = 1;
    bCoordsMatrix.M[1][2] = 1;
    bCoordsMatrix.M[2][2] = 1;
    
    // Invertimos la matriz
    bCoordsMatrix.Inverse();
    
    // Realizamos la misma operaci�n para las coordenadas UVS
    Matrix44 uvCoordsMatrix;
    Vector3 uvCoords;
    
    uvCoordsMatrix.M[0][0] = uv0.x;
    uvCoordsMatrix.M[1][0] = uv1.x;
    uvCoordsMatrix.M[2][0] = uv2.x;
    uvCoordsMatrix.M[0][1] = uv0.y;
    uvCoordsMatrix.M[1][1] = uv1.y;
    uvCoordsMatrix.M[2][1] = uv2.y;
    uvCoordsMatrix.M[0][2] = 1;
    uvCoordsMatrix.M[1][2] = 1;
    uvCoordsMatrix.M[2][2] = 1;
    
    uvCoordsMatrix.Inverse();
    
    // Realizamos el proceso para la rasterizaci�n de tri�ngulos. Creamos la tabla con una dimensi�n igual a la altura del framebuffer
    std::vector<Cell> table2;
    table2.resize(height);
    
    
    ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table2);
    ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table2);
    ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table2);
    
    // Recorremos en horizontal cada posici�n vertical y rellenaremos entre el valor m�ximo y m�nimo de cada l�nea horizontal
    for (int y = 0; y < height; y++) {
        
        if (table2[y].minX <= table2[y].maxX) {
            
            for (int x = table2[y].minX; x < table2[y].maxX; x++) {
                
                // Obtenemos las coordenadas baric�ntricas multiplicando la posici�n (x, y) en (forma de vector 3) por la matriz.
                bCoords = bCoordsMatrix * Vector3(x, y, 1);
                bCoords.Clamp(0, 1); // Ajustamos su valor entre 0 y 1
                float suma = bCoords.x + bCoords.y + bCoords.z;
                bCoords = bCoords / suma; // Normalizamos
                
                // Realizamos la misma operaci�n con las UVs
                uvCoords = uvCoordsMatrix * Vector3(x, y, 1);
                float suma2 = uvCoords.x + uvCoords.y + uvCoords.z;
                uvCoords = uvCoords/suma2;
                
                uvCoords.Clamp(0, 1);
                
                //Obtenemos el valor del pixel para cada x e y
                Color pColor = texture->GetPixel(uvCoords.x * uv0.x + uvCoords.y * uv1.x + uvCoords.z * uv2.x,
                                                 uvCoords.x * uv0.y + uvCoords.y * uv1.y + uvCoords.z * uv2.y);
                
                // Error encontrando las coordenadas baric�ntricas para las UVs (Se pinta un s�lo color por triangulo y no hemos logrado encontrar el error.
                
                
                // Obtenemos la variable que denota la distancia entre el pixel y la c�mara, que utilizaremos para actualizar el zBuffer
                float zeta = bCoords.x * p0.z + bCoords.y * p1.z + bCoords.z * p2.z;
                
                // Ahora estableceremos una serie de condicionales en funci�n del enum mode. En funci�n de que valor de enum tengamos, la entidad se 'colorear�' de una manera u otra.
                
                // Por defecto, se colorear�n todos los tri�ngulos del mismo color
                if (mode == eRenderMode::TRIANGLES){
                    SetPixelSafe(x, y, c0);
                }
                
                if (mode == eRenderMode::TRIANGLES_INTERPOLATED){ // Tri�ngulos interpolados s�n oclusi�n
                    SetPixelSafe(x, y, bCoords.x * c0 + bCoords.y * c1 + bCoords.z * c2);
                    
                }
                
                if (mode == eRenderMode::TRIANGLES_OCLUSSION){ // Tri�ngulos interpolados con oclusi�n
                    if (x < zBuffer->width && y < zBuffer->height){
                        if (zeta < (zBuffer->GetPixel(x, y))){
                                SetPixelSafe(x, y, bCoords.x * c0 + bCoords.y * c1 + bCoords.z * c2);
                                zBuffer->SetPixel(x, y, zeta);
                        }
                    }
                }
                
                
                if (mode == eRenderMode::TEXTURE){ // Texturas
                    
                    if (x < zBuffer->width && y < zBuffer->height){
                        if (zeta < (zBuffer->GetPixel(x, y))){
                            SetPixelSafe(x, y, pColor);
                            zBuffer->SetPixel(x, y, zeta);
                        }
                    }
                }
            }
        }
    }
}
 



// Funci�n para obtener valores aleatorios
float getRandomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
}

ParticleSystem::ParticleSystem(){
}
    
void ParticleSystem::Init(){
    
    for (int i=0; i < MAX_PARTICLES; i++){
        
        particles[i].position.x = getRandomFloat(0, 1280);
        particles[i].position.y = getRandomFloat(64, 720);
        
        // Definimos el vector velocidad como el vector que apunta al punto (0,0)
        particles[i].velocity.x = 0 - particles[i].position.x;
        particles[i].velocity.y = 0 - particles[i].position.y;
        
        particles[i].color = Color(getRandomFloat(0, 255),getRandomFloat(0, 255),getRandomFloat(0, 255));
        particles[i].acceleration = getRandomFloat(1, 5);
        particles[i].ttl = getRandomFloat(20, 100);
        particles[i].inactive = false;
        
    }
}

void ParticleSystem::Render(Image* framebuffer){
    
    for (int i = 1 ; i < MAX_PARTICLES; i++){
        if (!particles[i].inactive){ // Si la particula esta activa
            if ((particles[i].position.x < framebuffer->width && particles[i].position.x > 0) && (particles[i].position.y < (framebuffer->height) && particles[i].position.y > 0)) { // Condiciones para no salir de los bordes del framebuffer
                // Coloreamos un pixel
                framebuffer->SetPixel(particles[i].position.x, particles[i].position.y, particles[i].color);
            }
        }
    }
}

// Funcion para actualizar los valores de las particulas
void ParticleSystem::Update(float dt){
    for (int i=0; i < MAX_PARTICLES; i++){
        // Actualizamos la posicion en funcion de la velocidad y la aceleracion
        particles[i].position = particles[i].position + (particles[i].velocity * dt * particles[i].acceleration);
        // Actualizamos el tiempo que la particula esta en el framebuffer
        particles[i].ttl = particles[i].ttl - dt;
        // Si su tiempo ha terminado, actualizamos su estado para que Render no la use m�s
        if (particles[i].ttl == 0){
            particles[i].inactive = true;
        }
    }
}
 
Button::Button(){}
    
Button::Button(const char& _image, Vector2& _position) {
	ruta = &_image;
	posicio = _position;
	imagen.LoadPNG(ruta, true);
	altura = imagen.height;
	ancho = imagen.width;
}

void Button::DrawButton(Image& framebuffer) {
	framebuffer.DrawImage(imagen, posicio.x, posicio.y, false);
}

bool Button::isMouseInside(Vector2 coords) {
	if (coords.x >= posicio.x && coords.x <= (posicio.x + ancho) && coords.y >= posicio.y && coords.y <= (posicio.y + altura))
	{
		return true;
	}
	else {
		return false;
	}
}

int Button::getAltura() {
	return altura;
}

int Button::getAncho() {
	return ancho;
}


