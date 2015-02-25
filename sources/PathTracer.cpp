#include "PathTracer.hh"
#include <iostream>
#include <iomanip>

void trace(Image*, float, float, float);
float x = 0;
float z = -8;
float y = 0;

float pi = 3.14;

float distance = 8;

float xx = .3;
float yy = -3.14;
float speed = .5f;
Image*	PathTracer::onDraw(){
		return this->image;
}


void 	PathTracer::onKeyboardEvent(Display::KeyboardEvent, Display::KeyCode kc){
	if (kc == Display::UP)
		distance += speed;
	if (kc == Display::DOWN)
		distance -= speed;

	if (kc == Display::RIGHT)
		yy += speed;
	if (kc == Display::LEFT)
		yy -= speed;


	x = distance * cos(xx) * sin(yy);
	z = distance * cos(xx) * cos(yy);
	y = distance * sin(xx);

	//std::cout << std::fixed << std::setprecision(1) << x << ":" << y << ":" << z << std::endl;
	trace(this->image, x, y, z);
	//toast(this->image, 10, 2, 0);
}


int mx = -1;
int my = -1;

void 	PathTracer::onMouseEvent(Display::MouseEvent event,
				 Display::MouseButton, int mouse_x, int mouse_y){
	if (event == Display::BUTTON_UP){
		mx = -1;
		return;
	}
	if (mx < 0){
		mx = mouse_x;
		my = mouse_y;
		return;
	}
	yy += (mouse_x - mx) * .01f;
	xx += (mouse_y - my) * .01f;
	mx = mouse_x;
	my = mouse_y;
	this->onKeyboardEvent(Display::KEY_UP, Display::RETURN);
}


PathTracer::PathTracer(int width, int height){
	this->image = new Image(width, height);
	//this->image->setColor(200, 100, Color(255, 255, 0));
	


	this->onKeyboardEvent(Display::KEY_UP, Display::RETURN);
}
