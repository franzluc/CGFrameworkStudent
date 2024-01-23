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
#include <cmath>

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

void Image::DrawRect(int x, int y, int w, int h, const Color& bordercolor, int borderWidth, bool isFilled, const Color& fillColor)
{
	if (w > 0 && h < 0) { //Quan la 1a posicio del cursor i la segona formen un vector diagonal cap a la part inferior dreta
		for (int p = 0; p < borderWidth; ++p) {  //Aquest for es per l'amplada del border en les coordenades X
			for (int i = 0; i < w + (borderWidth - 1); ++i) {
				SetPixel(x + i, y + p, bordercolor);
				SetPixel(x + i, y + h - 1 + p, bordercolor);
			}
		}
		for (int q = 0; q < borderWidth; ++q) {  //Aquest for es per l'amplada del border en les coordernades Y
			for (int j = 0; j > h; --j) {
				SetPixel(x + q, y - abs(j), bordercolor);
				SetPixel(x + w - 1 + q, y - abs(j), bordercolor);
			}
		}
		//Si el rectangle ha d'estar omplert de color:
		if (isFilled) {
			for (int i = 0; i < w - borderWidth - 1; ++i) {
				for (int j = 0; j > (h + borderWidth - 1); --j) {
					SetPixel(x + i + borderWidth, y + j - 1, fillColor);
				}
			}
		}
	}
	else if (w < 0 && h < 0) { //Quan la 1a posicio del cursor i la segona formen un vector diagonal cap a la part inferior esquerra
		for (int p = 0; p < borderWidth; ++p) {  //Aquest for es per l'amplada del border en les coordenades X
			for (int i = 0; i > (w + (borderWidth - 1)); --i) {
				SetPixel(x - abs(i), y - p, bordercolor);
				SetPixel(x - abs(i), y + h + 1 + p, bordercolor);
			}
		}
		for (int q = 0; q < borderWidth; ++q) {  //Aquest for es per l'amplada del border en les coordernades Y
			for (int j = 0; j > h; --j) {
				SetPixel(x - q, y - abs(j), bordercolor);
				SetPixel(x - abs(w) + 1 + q, y - abs(j), bordercolor);
			}
		}
		//Si el rectangle ha d'estar omplert de color:
		if (isFilled) {
			for (int i = 0; i > w + (borderWidth * 2); --i) {
				for (int j = 0; j > (h + borderWidth * 2); --j) {
					SetPixel(x + i - borderWidth, y + j - borderWidth, fillColor);
				}
			}
		}
	}
	else if (w < 0 && h > 0) { //Quan la 1a posicio del cursor i la segona formen un vector diagonal cap a la part superior esquerra
		for (int p = 0; p < borderWidth; ++p) {  //Aquest for es per l'amplada del border en les coordenades X
			for (int i = 0; i > (w + (borderWidth - 1)); --i) {
				SetPixel(x - abs(i), y - p, bordercolor);
				SetPixel(x - abs(i), y + h + 1 + p, bordercolor);
			}
		}
		for (int q = 0; q < borderWidth; ++q) {  //Aquest for es per l'amplada del border en les coordernades Y
			for (int j = 0; j < (h + (borderWidth * 2)); ++j) {
				SetPixel(x + q, (y - borderWidth + 1) + j, bordercolor);
				SetPixel(x + q + w, (y - borderWidth + 1) + j, bordercolor);
			}
		}
		//Si el rectangle ha d'estar omplert de color:
		if (isFilled) {
			for (int i = 0; i > (w + borderWidth); --i) {
				for (int j = 0; j < h; ++j) {
					SetPixel(x + i - 1, y + j + 1, fillColor);
				}
			}
		}
	}
	else if (w > 0 && h > 0) { //Quan la 1a posicio del cursor i la segona formen un vector diagonal cap a la part superior dreta
		for (int p = 0; p < borderWidth; ++p) {  //Aquest for es per l'amplada del border en les coordenades X
			for (int i = 0; i < w + (borderWidth - 1); ++i) {
				SetPixel(x + i, y + p, bordercolor);
				SetPixel(x + i, y + h - 1 + p, bordercolor);
			}
		}
		for (int q = 0; q < borderWidth; ++q) {  //Aquest for es per l'amplada del border en les coordernades Y
			for (int j = 0; j < (h + borderWidth - 1); ++j) {
				SetPixel(x + q, y + j, bordercolor);
				SetPixel(x + q + w, y + j, bordercolor);
			}
		}
		//Si el rectangle ha d'estar omplert de color:
		if (isFilled) {
			for (int i = 0; i < (w - borderWidth); ++i) {
				for (int j = 0; j < (h - borderWidth); ++j) {
					SetPixel(x + borderWidth + i, y + borderWidth + j, fillColor);
				}
			}
		}
	}
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

//Funci� per dibuixar una l�nia
void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c)
{
	//Es calcula la diferencia entre els punts
	int dx = x1 - x0;
	int dy = y1 - y0;

	//Es calculen els punts intermitjos que formaran la linia
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	//Es calcula l'increment
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;

	//Es dibuixa la linia
	float X = x0;
	float Y = y0;
	for (int i = 0; i <= steps; i++) {
		SetPixel(round(X), round(Y), Color::WHITE);
		X += Xinc;
		Y += Yinc;
	}

}



void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table) {

	//Es calcula la diferencia entre els punts
	int dx = x1 - x0;
	int dy = y1 - y0;

	//Es calculen els punts intermitjos que formaran la linia
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	//Es calcula l'increment
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;

	//Es dibuixa la linia
	float X = x0;
	float Y = y0;
	/*for (int i = 0; i <= steps; i++) {
		table[]
		//SetPixel(round(X), round(Y), Color::WHITE);
		X += Xinc;
		Y += Yinc;
	}*/

	/* Algoritme DDA per calcular el pendent
	int dx = x1 - x0;
	int dy = y1 - y0;
	float pendent = static_cast<float>(dy) / static_cast<float>(dx);

	// Si el pendent es mes petit que 1
	if (std::abs(dx) >= std::abs(dy)) {
		
		int x, y;
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		y = y0;
		for (x = x0; x <= x1; ++x) {
			UpdateTable(table, y, x);
			y += pendent;
		}
	}
	// Si el pendent es mes gran que 1
	else {
		int x, y;
		if (y0 > y1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		x = x0;
		for (y = y0; y <= y1; ++y) {
			UpdateTable(table, y, x);
			x += 1 / pendent;
		}
	}*/
}

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
    

void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor) {
	// Dibuix de linies del triangle
	Image::DrawLineDDA(p0.x, p0.y, p1.x, p1.y, Color::WHITE);
	Image::DrawLineDDA(p1.x, p1.y, p2.x, p2.y, Color::WHITE);
	Image::DrawLineDDA(p2.x, p2.y, p0.x, p0.y, Color::WHITE);

	// Es crea la taula per emmagatzemar valors
	/*int table[720][2];
	std::vector<Cell> AET;*/

	// S'escanejen els valors minims i maxims
	/*for (int i = 0; i <= 720; ++i) {
		for (int j = 0; j <= 1280; ++j) {
			if ()
		}
		// S'ordena la taula AET
		ScanLineDDA(x0, y0, x1, y1, AET);
	}*/
}

void Image::DrawImage(const Image& image, int x, int y, bool top) {
	Image imatge;
	imatge = image;
	Color* pixels;
	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {
			pixels = &imatge.GetPixel(i, j);
			SetPixel(x + i, y + j, *pixels);
		}
	}
}
