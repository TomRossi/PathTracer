//
//  Jpeg.cpp
//  auto_stereo
//
//  Created by Tom on 02/08/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//

#include <stdlib.h>
#include <fstream>
#include "Jpeg.h"

Jpeg::Jpeg(){
  this->_width = 0;
  this->_height = 0;
  this->_file = "";
  this->_buffer = NULL;
  this->_size = 0;
}

/* copies to the buffer */
void Jpeg::handleBuffer(JSAMPLE *buf, int len)
{
  memcpy(this->_buffer + this->_size, buf, len);
  this->_size += len;
}

void Jpeg::allocate(j_decompress_ptr cinfo){
    this->_width = cinfo->image_width;
    this->_height = cinfo->image_height;
    this->_buffer = (JSAMPLE*)
                    malloc(sizeof(JSAMPLE) * 3
                           * this->_width * this->_height + 16);
}

void Jpeg::loadFromFile(std::string const& filename){
    FILE * infile;
    JSAMPARRAY buffer;
    int row_stride;
    
    
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr.pub);
    
    if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename.c_str());
        return;
    }
    
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    this->allocate(&cinfo);

    (void) jpeg_start_decompress(&cinfo);
    
    
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        this->handleBuffer(buffer[0], row_stride);
    }
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    
    fclose(infile);
}

void Jpeg::exportToFile(std::string const& filename){
    struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
    FILE * outfile;
    JSAMPROW row_pointer[1];
    int row_stride;
       cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
      if ((outfile = fopen(filename.c_str(), "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename.c_str());
        exit(1); //TODO throw error
    }
    jpeg_stdio_dest(&cinfo, outfile);
    
    cinfo.image_width = this->_width;
    cinfo.image_height = this->_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
  
    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, 100, TRUE);
    
    jpeg_start_compress(&cinfo, TRUE);
    

    row_stride = this->_width * 3;
    
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = & this->_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}
