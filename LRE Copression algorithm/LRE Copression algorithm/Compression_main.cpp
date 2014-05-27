//File compression
//Author: John Palacios

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int SIZEOFCOLOR = 3;

struct color
{
	unsigned char r, g, b;
};

struct Image
{
	int w, h, max;
	color* pixels;
	string type;
	string comment;
};

struct Quad
{
	color triple;
	int quantity;
};

Image *loadImage(string filename)
{
	ifstream origional;
	string type, comment;
	int w, h, max;
	long int pos;
	Image *img;

	origional.open(filename, ios::in);
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
	pos = origional.tellg();

	img = new Image;
	img->w = w;
	img->h = h;
	img->max = max;
	img->pixels = new color [w*h];
	img->type = type;
	if(!comment.empty())
		img->comment = comment;
	else
		img->comment = "#noComment";

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
		origional.close();
		origional.open(filename, ios::binary | ios::in);
		origional.seekg(pos - 2, ios::beg);
		origional.read((char*)img->pixels, w*h*SIZEOFCOLOR);
	}

	origional.close();

	return img;
}

void compressImage(string dest_file, Image *img)
{
	fstream compressed;
	int num_pixels = 0;
	int length_run = 1;
	color current_color = img->pixels[0];
	color next_color;

	if(img->type == "P3"){
		int red;
		int green;
		int blue;
		compressed.open(dest_file, ios::app | ios::out);
		compressed << img->type << "\n" << img->comment << "\n" << img->w << " " << img->h << "\n" << img->max << "\n";

		while(num_pixels < img->w * img->h){
			num_pixels++;
			next_color = img->pixels[num_pixels];
			if(next_color.r != current_color.r || next_color.g != current_color.g || next_color.b != current_color.b){
				red = current_color.r;
				green = current_color.g;
				blue = current_color.b;
				compressed << " " << red << " " << green << " " << blue << " " << length_run;
				length_run = 1;
				current_color = next_color;
			}
			else
				length_run++;
		}

		if(length_run > 1){
			red = current_color.r;
			green = current_color.g;
			blue = current_color.b;
			compressed << red << " " << green << " " << blue << " -" << length_run;
		}
	}
	if(img->type == "P6"){
		Quad block;
		compressed.open(dest_file, ios::binary | ios::app | ios::out);
		compressed << img->type << "\n" << img->comment << "\n" << img->w << " " << img->h << "\n" << img->max << "\n";

		while(num_pixels < img->w * img->h){
			num_pixels++;
			next_color = img->pixels[num_pixels];
			if(next_color.r != current_color.r || next_color.g != current_color.g || next_color.b != current_color.b){
				block.triple.r = current_color.r;
				block.triple.g = current_color.g;
				block.triple.b = current_color.b;
				block.quantity = length_run;
				compressed.write(reinterpret_cast<char *>(&block), sizeof(block));
				length_run = 1;
				current_color = next_color;
			}
			else
				length_run++;
		}

		if(length_run > 1){
			block.triple.r = current_color.r;
			block.triple.g = current_color.g;
			block.triple.b = current_color.b;
			block.quantity = length_run;
			compressed.write(reinterpret_cast<char *>(&block), sizeof(block));
		}
	}

	compressed.close();
}

void uncompressImage(string src_file, string dest_file)
{
	fstream restored;
	fstream compressed;
	string type, comment;
	int w, h, max;
	int num_pixels = 0;
	int length_run = 0;
	long int pos;
	

	compressed.open(src_file, ios::in);
	if(compressed.fail()){
		cout << "Error oppening src_file, aborting program.\n";
		exit(2);
	}

	getline(compressed, type);
	if(compressed.peek() == '#')
	{
		getline(compressed, comment);
	}
	compressed >> w >> h >> max;
	pos = compressed.tellg();
	compressed.close();
	if(type == "P3"){
		compressed.open(src_file, ios::in);
		compressed.seekg(pos, ios::beg);
		restored.open(dest_file, ios::out | ios::app);
		restored << type << "\n" << comment << "\n" << w << " " << h << "\n" << max << "\n";
		int red, green, blue;
		while(num_pixels < w * h){
			compressed >> red >> green >> blue >> length_run;
			for(int i = 0; i < length_run; i++){
				restored << red << " " << green << " " << blue << " ";
				num_pixels++;
			}
		}
	}

	if(type == "P6"){
		compressed.open(src_file, ios::binary | ios::in);
		compressed.seekg(pos - 2, ios::beg);
		restored.open(dest_file, ios::binary | ios::out | ios::app);
		restored << type << "\n" << comment << "\n" << w << " " << h << "\n" << max << "\n";
		Quad block;
		color temp;
		while(num_pixels < w * h){
			compressed.read(reinterpret_cast<char *>(&block), sizeof(block));
			for(int i = 0; i < block.quantity; i++){
				//temp = block.triple;
				temp.r = block.triple.r;
				temp.g = block.triple.g;
				temp.b = block.triple.b;
				restored.write(reinterpret_cast<char *>(&temp), sizeof(temp));
				num_pixels++;
			}
		}
	}

	compressed.close();
	restored.close();
}

int main()
{
	string orig_name;
	string comp_name;
	string comp_type;
	string rest_name;
	string user_chce;
	string loop_reply;
	Image *img;

	cout << "Welcome to Crown Image Compressor 0.1.3! This software compresses\n"
		<< "type P3 and P6 .ppm image files. Please follow the prompts as they\n"
		<< "appear, thank you.\n -John\n";
	do{
		cout << "What would you like to do?\n"
			<< "type [compress] to compress a file\n"
			<< "type [uncompress] to uncompress a file\n";
		getline(cin, user_chce);
		if(tolower(user_chce[0]) == 'c'){
			cout << "Please enter the ppm file name (without the \".ppm\")\n";
			getline(cin, orig_name);
			cout << "Thank you; please enter the destination file name\n"
				<< "(Ommiting the extension type: I will determine that)\n";
			getline(cin, comp_name);
			orig_name.append(".ppm");
			img = loadImage(orig_name);
			if(img->type == "P3")
				comp_name.append(".txt");
			else
				comp_name.append(".dat");
			cout << "Thank you, compressing " << orig_name << " into \n"
				<< comp_name << "...\n";
			compressImage(comp_name, img);
			cout << "Success!\n";
		}
		if(tolower(user_chce[0]) == 'u'){
			cout << "Please enter the target compressed file name\n"
				<< "(with the \".txt or .dat\" file extension)\n";
			getline(cin, comp_name);
			cout << "Thank you, please enter the destination file\n"
				<< "name (without the \".ppm\" file extension)\n";
			getline(cin, rest_name);
			cout << "Thank you, uncompressing " << comp_name << " into\n"
				<< rest_name << "...\n";
			rest_name.append(".ppm");
			uncompressImage(comp_name, rest_name);
			cout << "Success!\n";
		}
		cout << "Thank you for using Crown Image Compressor 0.1.3\n"
			<< "Would you like to run again?\n";
		getline(cin, loop_reply);
	}while(tolower(loop_reply[0]) == 'y');

	cout << "Good bye.\n";
	return 0;
}