#include <GLUT/glut.h>
#include "Glut.hh"

GlutWindow* GlutWindow::instance;

GlutWindow::GlutWindow(std::string const& name, int width, int height):
	Display::Window(name, width, height){
		GlutWindow::instance = this;
}


// TODO
// check returns
void GlutWindow::start(Display::Event* event){
	this->_event = event;

	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(this->_width, this->_height);
	glutInitWindowPosition(100,100); // TODO
	this->id = glutCreateWindow(this->_name.c_str());

	glutDisplayFunc(GlutWindow::display);
	glutKeyboardFunc(GlutWindow::keyboard);
	glutMotionFunc(GlutWindow::mouseMove);
	glutMouseFunc(GlutWindow::mouseEvent);
	glutMainLoop();
}

void GlutWindow::mouseEvent(int button, int state, int x, int y){
	if (state == GLUT_UP)
		GlutWindow::instance->_event->onMouseEvent(Display::BUTTON_UP, 
							   Display::LEFT_B, 0, 0);

}
void GlutWindow::mouseMove(int x, int y){
	GlutWindow::instance->_event->onMouseEvent(Display::MOUSE_MOVE, 
						   Display::LEFT_B, x, y);
	 glutPostRedisplay();
}

void GlutWindow::display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	
	Image*	image = 0;

	if (GlutWindow::instance->_event)
		image = GlutWindow::instance->_event->onDraw();
	glDrawPixels(GlutWindow::instance->_width,
		GlutWindow::instance->_height, GL_RGB, GL_UNSIGNED_BYTE,
		image->getData());	
	glutSwapBuffers();
	//glutPostRedisplay();
}

void GlutWindow::keyboard(unsigned char key, int x, int y){
	switch (key){
		case 'w':
				GlutWindow::instance->_event->onKeyboardEvent(Display::KEY_UP, 
					Display::UP);
		break;	
		case 's':
				GlutWindow::instance->_event->onKeyboardEvent(Display::KEY_UP, 
					Display::DOWN);		break;
		case 'a':
				GlutWindow::instance->_event->onKeyboardEvent(Display::KEY_UP, 
					Display::LEFT);		break;
		case 'd':
				GlutWindow::instance->_event->onKeyboardEvent(Display::KEY_UP, 
					Display::RIGHT);		break;    case 27: // Escape key
      glutDestroyWindow(GlutWindow::instance->id);
      exit (0);
      break;
  }
  glutPostRedisplay();
}
