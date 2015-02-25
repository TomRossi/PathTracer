
#include "Window.hh"

class PathTracer: public Display::Event{
public:
	Image*			image;
	PathTracer(int, int);
	virtual Image*	onDraw();
	virtual void 	onKeyboardEvent(Display::KeyboardEvent, Display::KeyCode);
  virtual void 	onMouseEvent(Display::MouseEvent, Display::MouseButton, int, int);	
};
