#pragma once

#include "matrixClass.h"
#include "STL.h"
#include "WindowManagerClass.h"
#include "OpenGLInitializerClass.h"
#include "GUI_ManagerClass.h"
#include "shaderLoaderClass.h"

// openMP
#include <omp.h>

// 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576,
#define NUM_PARTICLE 131072

class mainController
{
public:

	OpenGLInitializerClass* opengl_initializer_obj;
	WindowManagerClass* window_manager_obj;
	GUI_ManagerClass* gui_manager_obj;

	matrixClass* matrix_obj;
	matrixClass* scale_mat_obj;
	STL* STL_obj;
	shaderLoaderClass* shader_loader_obj;

	HWND glWindow;
	HWND openGL_view;


	//******
	unsigned long elapsedTime = 0;


	// shader
	GLuint NORMAL_VS;
	GLuint NORMAL_FS;
	GLuint NORMAL_PRG;
	GLint UNF_NORMAL_scaleMatrix;

	GLuint SECOND_VS;
	GLuint SECOND_FS;
	GLuint SECOND_PRG;
	GLint UNF_SECOND_scaleMatrix;

	GLuint POINT_VS;
	GLuint POINT_FS;
	GLuint POINT_PRG;

	// compute shader
	GLuint SSBO_CS;
	GLuint SSBO_PRG;
	GLint UNF_SSBO_numPoint;

	int numPoint;


	// VAO, VBO
	GLuint VAO_name;
	GLuint VBO_name[2];

	// UBO data,
	GLuint UBO_name;
	struct sharedUniform {
		GLfloat mat[16];
		float coef;
	} struct_uniform;



	GLuint SSBO_name;
	struct Particle {
		float tempData[4];
		float tempVelocity[4];
		float tempColor[4];
	} particles[NUM_PARTICLE];


	float debugRadian = 0.0;
	long debugCounter = 0;


	mainController();
	~mainController();


	void setup_openGL(HWND mainWindow);
	void load_stl(const wchar_t* path);

	void setup_shader();
	void setup_buffer();
	void setup_compute_shader();
	void setup_SSBO();

	void drawGL();

	double test_func(int n);
};

