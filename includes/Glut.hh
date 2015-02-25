#include "Window.hh"




class GlutWindow: public Display::Window{
public:
	int		id;



public:
	static void 		display(void);
	static void 		keyboard(unsigned char, int, int);
	static void 		mouseMove(int, int);
	static void 		mouseEvent(int, int, int, int);

	static GlutWindow*	instance;

	GlutWindow(std::string const&, int, int);
	virtual void start(Display::Event*);
};