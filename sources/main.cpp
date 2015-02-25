//
//  main.cpp
//  auto_stereo
//
//  Created by Tom on 02/08/14.
//  Copyright (c) 2014 Tom. All rights reserved.
//


#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "Jpeg.h"
#include "Glut.hh"
#include "PathTracer.hh"

int main(int argc, char * argv[])
{
	int ww = 400;
  
	GlutWindow* w = new GlutWindow("salut", ww, ww);
	w->start(new PathTracer(ww, ww));
  	return 0;
}
