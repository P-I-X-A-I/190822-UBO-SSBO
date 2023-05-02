#include "stdafx.h"
#include "mainController.h"


double mainController::test_func(int n)
{
	double x, pi, sum = 0.0, step;

	step = 1.0 / (double)n;

#pragma omp parallel for reduction(+:sum) private(x)

	for (int i = 0; i < n; i++)
	{
		x = (i - 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}

	pi = step * sum;

	return pi;

}


mainController::mainController()
{
	printf("mainController init\n");

	srand(time(NULL));

	opengl_initializer_obj = new OpenGLInitializerClass();
	window_manager_obj = new WindowManagerClass();
	gui_manager_obj = new GUI_ManagerClass();


	matrix_obj = new matrixClass();
	scale_mat_obj = new matrixClass();

	STL_obj = new STL();
	shader_loader_obj = new shaderLoaderClass();


	struct_uniform.coef = 1.0;
	for (int i = 0; i < 16; i++)
	{
		struct_uniform.mat[i] = 0.0;
	}


	for (int i = 0; i < NUM_PARTICLE; i++)
	{
		particles[i].tempData[0] = (rand() % 200 - 100)*0.01;
		particles[i].tempData[1] = (rand() % 200 - 100)*0.01;
		particles[i].tempData[2] = (rand() % 200 - 100) * 0.0025 + 1.0; // push coef
		particles[i].tempData[3] = (rand() % 200 - 100) * 0.0025 + 1.0; // pull coef

		particles[i].tempVelocity[0] = 0.0;
		particles[i].tempVelocity[1] = 0.0;
		particles[i].tempVelocity[2] = 0.0;
		particles[i].tempVelocity[3] = 0.0;

		particles[i].tempColor[0] = 0.0;
		particles[i].tempColor[1] = 0.0;
		particles[i].tempColor[2] = 0.0;
		particles[i].tempColor[3] = 0.0;

	}

	numPoint = NUM_PARTICLE;
}


mainController::~mainController()
{
}




void mainController::setup_openGL(HWND mainWindow)
{
	// create dummy window
	HWND dummyWnd = window_manager_obj->create_borderless_window(L"dummy");
	window_manager_obj->set_window_position(dummyWnd, 0, 0);
	window_manager_obj->set_window_size(dummyWnd, 2500, 500);
	window_manager_obj->set_window_level(dummyWnd, 0);

	SetWindowPos(mainWindow, 0, 10, 0, 1280, 960, SWP_SHOWWINDOW);

	openGL_view = gui_manager_obj->create_textfield(mainWindow, NULL, 0, 0, 1280, 960, 10);

	// init opengl function with dummy window
	opengl_initializer_obj->init_OpenGL_functions(dummyWnd, openGL_view);

	// close dummy window
	window_manager_obj->close_window(dummyWnd);

	// set opengl initial state
	opengl_initializer_obj->set_OpenGL_status();
	glEnable(GL_POINT_SMOOTH);
}




void mainController::load_stl(const wchar_t* path)
{
	STL_obj->loadSTLfrom_W_Path(path);
}




void mainController::setup_shader()
{
	printf("\n*** SETUP SHADER ***\n");

	HINSTANCE hInst = GetModuleHandle(NULL);

	// get resource handle
	HRSRC normal_vs_res = FindResourceW(hInst,
		MAKEINTRESOURCEW(129),
		L"TXT");

	if (normal_vs_res == NULL) { printf("find resource fail...\n"); }

	// get resource data
	HGLOBAL normal_vs_data = LoadResource(hInst, normal_vs_res);
	size_t dataSize = SizeofResource(hInst, normal_vs_res);
	//printf("data size %d\n", dataSize);

	if (normal_vs_data == NULL) { printf("load resource data fail...\n"); }


	// access data
	char* normal_vs_ptr = (char*)LockResource(normal_vs_data);
	//printf("%s\n", normal_vs_ptr);

	shader_loader_obj->loadShaderSource_And_CompileShader_fromPtr(normal_vs_ptr, 0, &NORMAL_VS);

	//***********************************************************
	HRSRC normal_fs_res = FindResourceW(hInst, MAKEINTRESOURCE(130), L"TXT");
	//if (normal_fs_res == NULL) { printf("find normal fs resource fail...\n"); }
	HGLOBAL normal_fs_data = LoadResource(hInst, normal_fs_res);
	//if (normal_fs_data == NULL) { printf("get normal fs data fail....\n"); }
	char* normal_fs_ptr = (char*)LockResource(normal_fs_data);
	//printf("%s\n", normal_fs_ptr);

	shader_loader_obj->loadShaderSource_And_CompileShader_fromPtr(normal_fs_ptr, 2, &NORMAL_FS);
	shader_loader_obj->createProgram_And_AttachShader(&NORMAL_PRG, &NORMAL_VS, NULL, &NORMAL_FS);


	shader_loader_obj->getUniformLocation(&NORMAL_PRG, &UNF_NORMAL_scaleMatrix, "scaleMatrix");





	//***********************************************************************
	HRSRC second_vs_res = FindResourceW(hInst, MAKEINTRESOURCE(131), L"TXT");
	HGLOBAL second_vs_data = LoadResource(hInst, second_vs_res);
	
	HRSRC second_fs_res = FindResourceW(hInst, MAKEINTRESOURCE(132), L"TXT");
	HGLOBAL second_fs_data = LoadResource(hInst, second_fs_res);

	char* second_vs_ptr = (char*)LockResource(second_vs_data);
	char* second_fs_ptr = (char*)LockResource(second_fs_data);

	//printf("%s\n", second_vs_ptr);
	//printf("%s\n", second_fs_ptr);

	shader_loader_obj->loadShaderSource_And_CompileShader_fromPtr(second_vs_ptr, 0, &SECOND_VS);
	shader_loader_obj->loadShaderSource_And_CompileShader_fromPtr(second_fs_ptr, 2, &SECOND_FS);
	shader_loader_obj->createProgram_And_AttachShader(&SECOND_PRG, &SECOND_VS, NULL, &SECOND_FS);


	shader_loader_obj->getUniformLocation(&SECOND_PRG, &UNF_SECOND_scaleMatrix, "scaleMatrix");



	//***********************************************
	shader_loader_obj->loadShaderSource_And_CompileShader("point_vs.txt", 0, &POINT_VS);
	shader_loader_obj->loadShaderSource_And_CompileShader("point_fs.txt", 2, &POINT_FS);
	shader_loader_obj->createProgram_And_AttachShader(&POINT_PRG, &POINT_VS, NULL, &POINT_FS);

}






void mainController::setup_buffer()
{
	printf("\n*** SETUP BUFFER ***\n");

	glGenVertexArrays(1, &VAO_name);
	glGenBuffers(2, VBO_name);

	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0); // vertex 
	glEnableVertexAttribArray(1); // normal



	// upload STL data
	float* vertPtr = STL_obj->vert_Ptr;
	float* normPtr = STL_obj->norm_Ptr;
	unsigned long numTriangle = *STL_obj->num_triangles;
	long dataSize = sizeof(float) * numTriangle * 3 * 3;

	// vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name[0]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, vertPtr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// normal
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name[1]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, normPtr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	printf("STL data size %d\n", dataSize);


	glBindVertexArray(0);



	// setup uniform buffer
	glGenBuffers(1, &UBO_name);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_name);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sharedUniform), &struct_uniform, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLint normal_block_index = glGetUniformBlockIndex(NORMAL_PRG, "uniformBlock1");
	GLint second_block_index = glGetUniformBlockIndex(SECOND_PRG, "uniformBlock2");
	GLint maxBinding;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBinding);
	printf("NORMAL_PRG uniformBlock index %d\n", normal_block_index);
	printf("SECOND_PRG uniformBlock index %d\n", second_block_index);
	printf("max buffer uniform binding %d\n", maxBinding);


	// set uniform block index to UBO table
	//glUniformBlockBinding(NORMAL_PRG, normal_block_index, 5);
	//glUniformBlockBinding(SECOND_PRG, second_block_index, 5);
}









void mainController::setup_SSBO()
{
	printf("\n*** SETUP SSBO ***\n");


//*****************************************
#ifdef _OPENMP
	printf("the number of processor %d\n", omp_get_num_procs());

	double d;
	int n = 100000000;
	DWORD dwStart = GetTickCount();

	// 1st
	d = test_func(n);
	printf("for %d steps, pi = %.9f, %d msec\n", n, d, GetTickCount() - dwStart);


	// 2nd
	dwStart = GetTickCount();
	d = test_func(n);
	printf("for %d steps, pi = %.9f, %d msec\n", n, d, GetTickCount() - dwStart);

#endif
//********************************************

	// gen ssbo name
	glGenBuffers(1, &SSBO_name);

	// bind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_name);

	// init ssbo buffer
	// SSBO 4 element alignment.

	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * numPoint, particles, GL_DYNAMIC_DRAW);



	// bind buffer base
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_name);


}







void mainController::setup_compute_shader()
{
	printf("\n*** SETUP COMPUTE SHADER ***\n");

	shader_loader_obj->loadShaderSource_And_CompileShader("SSBO_CS.txt", 3, &SSBO_CS);
	shader_loader_obj->createComputeProgram_And_AttachShader(&SSBO_PRG, &SSBO_CS);

	shader_loader_obj->getUniformLocation(&SSBO_PRG, &UNF_SSBO_numPoint, "numPoint");

	int maxGroup[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxGroup[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxGroup[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxGroup[2]);


	printf("max workgroup size %d %d %d\n", maxGroup[0], maxGroup[1], maxGroup[2]);
}




void mainController::drawGL()
{
	// swapbuffer and wait VSYNC
	SwapBuffers(opengl_initializer_obj->OpenGL_HDC);
	glFinish();


	LARGE_INTEGER freq; // counter's frequency
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);


	// run compute shader
	glUseProgram(SSBO_PRG);
	glUniform1i(UNF_SSBO_numPoint, numPoint);
	glDispatchCompute(numPoint / 128, 1, 1);

	//glFlush();
	glFinish();


	QueryPerformanceCounter(&end);
	LONGLONG span = end.QuadPart - start.QuadPart;
	double sec = (double)span / (double)freq.QuadPart;
	//printf("compute time %f ms\n", sec*1000.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glViewport(0, 0, 1280, 960);


	// set matrix
	scale_mat_obj->initMatrix();
	scale_mat_obj->scale_XYZ(0.01, 0.01, 0.01);
	scale_mat_obj->translate_XYZ(-0.5, 0.0, 0.0);


	// mvp matrix
	matrix_obj->initMatrix();
	matrix_obj->lookAt(0.0, 0.0, 2.0 - sin(debugRadian),
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	matrix_obj->perspective(80.0 + sin(debugRadian)*50.0,
							1280 / 960.0,
							0.1,
							5.0);


	debugRadian += 0.01;
	if (debugRadian > 6.2831853)
	{
		debugRadian -= 6.2831853;
	}



	// update UBO
	//float* matPtr = matrix_obj->getMatrix();

	//for (int i = 0; i < 16; i++)
	//{
	//	struct_uniform.mat[i] = *matPtr;
	//	matPtr++;
	//}
	matrix_obj->copy4x4MatrixToPointer(&(struct_uniform.mat[0]));
	struct_uniform.coef = (rand() % 10)*0.1;

	// bind UBO & updata buffer
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_name);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(sharedUniform), &struct_uniform);
	glBindBufferBase(GL_UNIFORM_BUFFER, 5, UBO_name);



	// draw object
	long numTriangle = *STL_obj->num_triangles;
	debugCounter++;
	if (debugCounter > numTriangle)
	{
		debugCounter = 0;
	}


	// Update Vertex Buffer
	glBindVertexArray(VAO_name);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name[0]);
	// get pointer
	float* vertPtr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	//vertPtr[debugCounter * 3] = (rand() % 2000 - 1000)*0.02;
	//vertPtr[debugCounter * 3 +1] = (rand() % 2000 - 1000)*0.02;
	//vertPtr[debugCounter * 3 +2] = (rand() % 2000 - 1000)*0.02;

	glUnmapBuffer(GL_ARRAY_BUFFER);


	glBindBuffer(GL_ARRAY_BUFFER, VBO_name[1]);
	float* normPtr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	//normPtr[debugCounter * 3] = (rand() % 100)*0.01;
	//normPtr[debugCounter * 3 +1] = (rand() % 100)*0.01;
	//normPtr[debugCounter * 3 +2] = (rand() % 100)*0.01;

	// unmap buffer before use
	glUnmapBuffer(GL_ARRAY_BUFFER);



	// 1st draw
	glUseProgram(NORMAL_PRG);
	glUniformMatrix4fv(UNF_NORMAL_scaleMatrix, 1, GL_FALSE, scale_mat_obj->getMatrix());
	glDrawArrays(GL_TRIANGLES, 0, numTriangle * 3);
	


	// 2nd draw
	scale_mat_obj->translate_XYZ(1.0, 0.0, 0.0);
	glUseProgram(SECOND_PRG);
	glUniformMatrix4fv(UNF_SECOND_scaleMatrix, 1, GL_FALSE, scale_mat_obj->getMatrix());
	glDrawArrays(GL_TRIANGLES, 0, numTriangle * 3);



	// draw SSBO particle
	glUseProgram(POINT_PRG);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glDrawArrays(GL_POINTS, 0, numPoint);


	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);










}