#pragma once


class shaderLoaderClass
{
public:




	shaderLoaderClass();
	~shaderLoaderClass();


	bool loadShaderSource_And_CompileShader(const char* name, int type, GLuint* toObj);
	bool loadShaderSource_And_CompileShader_fromPtr(char* srcPtr, int type, GLuint* toObj);
	bool createProgram_And_AttachShader(GLuint* prgPtr, GLuint* vsPtr, GLuint* gsPtr, GLuint* fsPtr );
	bool createComputeProgram_And_AttachShader(GLuint* prgPtr, GLuint* csPtr);

	void getUniformLocation(GLuint* prgObj, GLint* uniform, const char* name);
};

