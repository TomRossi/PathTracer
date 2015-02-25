#include "Image.hh"

/*void	Image::setColor(int x, int y, Color color){
	int idx = (x + (this->_height - y) * this->_width);
	idx *= 3;
	if (idx >= this->_length)
		return;
	this->_data[idx] = color.r;
	this->_data[idx + 1] = color.g; 
	this->_data[idx + 2] = color.b; 
	}*/

int Image::getWidth(){
	return _width;
}

int Image::getHeight(){
	return _height;
}

unsigned char const *Image::getData() const{
	return this->_data;
}

void Image::setBuffer(unsigned char* data, int length){
	this->_data = data;
}

int Image::getLength(){
	return this->_length;
}

Image::Image(int width, int height): _width(width), _height(height){
	this->_data = (unsigned char*)malloc(width * height * 3);
	this->_length = width * height * 3;
}
