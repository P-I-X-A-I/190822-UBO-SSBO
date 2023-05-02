#pragma once


// glew.h & wglew.h should be included
// glew32.dll & glew32.lib & glew32s.lib are needed

#include "glew.h"
#include "wglew.h"

#include <gl\GL.h> // in system folder
#include <gl\GLU.h>// in system folder
#pragma comment (lib, "opengl32.lib") // in system folder
#pragma comment (lib, "glew32.lib") // in own folder


class OpenGLInitializerClass
{
public:
	OpenGLInitializerClass();
	~OpenGLInitializerClass();


	// opengl context
	HDC OpenGL_HDC;
	HGLRC OpenGL_RC;

	
	bool init_OpenGL_functions(HWND dummyWnd, HWND glWnd);
	void set_OpenGL_status();
};

