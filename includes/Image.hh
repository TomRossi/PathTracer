#ifndef __IMAGE_HH__
#define __IMAGE_HH__

class Image{
	unsigned char*			_data;
	int						_width;
	int						_height;
	int						_length;

public:
	unsigned char const* 	getData() const;
	void 	setBuffer(unsigned char*, int length);
	int 	getLength();
	int 	getHeight();
	int 	getWidth();
	Image(int, int);
};

#endif
