#ifndef __window_hh__
#define __window_hh__

#include <string>
#include "Image.hh"

namespace Display{

	enum KeyCode{
		ESC, RETURN, DELETE, LEFT, RIGHT, UP, DOWN, SPACE
	};

	enum KeyboardEvent{
		KEY_DOWN, KEY_UP
	};

	enum MouseEvent{
		BUTTON_DOWN, BUTTON_UP, MOUSE_MOVE, SCROLL_UP, SCROLL_DOWN
	};

	enum MouseButton{
		LEFT_B, RIGHT_B
	};

	class Event{
		public:
			virtual Image* 	onDraw()												= 0;
			virtual void 	onKeyboardEvent(KeyboardEvent, KeyCode) 				= 0;
	  virtual void 	onMouseEvent(MouseEvent, MouseButton, int, int) 	= 0;
	};

	class Window{
	protected:
		std::string const&	_name;
		int 				_width;
		int					_height;
		Event* 				_event;

	public:
		Window(std::string const& name, int width, int height):
		_name(name), _width(width), _height(height){}
		virtual void start(Event*) = 0;
	};
};

#endif
