/**
* @file GLContext.h
*/

#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED

#include <glad/glad.h>

/// 三次元座標型.
struct Position
{
	float x, y, z;
};

/// RGBAカラー型.
struct Color 
{
	float r, g, b, a;
};

namespace GLContext {

	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor,GLuint vboTexcood,GLuint vboNormal, GLuint ibo);
	GLuint CreateProgram(GLenum type, const GLchar* code);
	GLuint CreateProgramFromFile(GLenum type, const char* filename);
	GLuint CreatePipeline(GLuint vp, GLuint fp);
	GLuint CreateSampler(GLenum wrapMode);
	GLuint CreateImage2D(const char* path);
	GLuint CreateImage2D(GLsizei width, GLsizei height, const void* data,GLenum pixelFormat,GLenum type);

}//namespace GLContext

#endif GLCONTEXT_H_INCLUDED