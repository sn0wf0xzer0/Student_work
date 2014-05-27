//File compression
//Author: John Palacios

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct color
{
	unsigned char r, g, b;
};

const int SIZEOFCOLOR = 3;

struct Image
{
	int w, h, max;
	color* pixels;
};

Image *loadImage(string filename)
{
	ifstream origional;
	string type, comment;
	int w, h, max;
	Image *img;

	origional.open(filename, ios::binary);
	if(origional.fail())
	{
		exit(1);
	}

	getline(origional, type);
	if(origional.peek() == '#')
	{
		getline(origional, comment);
	}
	origional >> w >> h >> max;

	img = new Image;
	img->w = w;
	img->h = h;
	img->max = max;
	img->pixels = new color [w*h];

	if(type == "P3")
	{
		int red, green, blue;
		for(int i=0; i<w*h; i++){
			origional >> red >> green >> blue;
			img->pixels[i].r = red;
			img->pixels[i].g = green;
			img->pixels[i].b = blue;
		}
	}else if(type == "P6"){
		origional.ignore();
		origional.read((char*)img->pixels, w*h*SIZEOFCOLOR);
	}

	origional.close();

	return img;
}
//f(x) here.

void compressImage(string dest_file, Image *img)
{
	//Still need to implement the most important part: compression.
	fstream compressed;
	compressed.open(dest_file, ios::binary | ios::app);
	if(compressed.fail()){
		exit(2);
	}
	int head[3];
	head[0] = img->w;
	head[1] = img->h;
	head[2] = img->max;
	color temp;

	for(int i = 0; i < 3; i++){
		compressed.write(reinterpret_cast<char *>(head[i]), sizeof(int));
	}

	for(int i = 0; i < head[0] * head[1]; i++){
		temp = img->pixels[i];
		compressed.write(reinterpret_cast<char *>(&temp), sizeof(temp));
	}

	compressed.close();
}

void uncompressImage(string src_file, string dest_file)
{

}

int main()
{

	Image *img = loadImage("grim.ppm");
	//compressImage("compressed.txt", img); use a RLE compression scheme. a pixel = triple 225 0 0 256
										//  may use Huffman key to turn 225 0 0  into 0, 000 into 01, and 225 225 225 into 011.
	//uncompressImage("compressed.txt", "uncompressed.ppm");
	return 0;
}