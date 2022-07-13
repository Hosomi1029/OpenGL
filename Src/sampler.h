/**
* @file sampler.h
*/
#ifndef SAMPLER_H_INCLUDED
#define SAMPLER_H_INCLUDED
#include <glad/glad.h>
#include "GLContext.h"
#include <string>

class Sampler {
public:
	Sampler(GLenum);
	~Sampler();

	// オブジェクトの有効性を判定する		
	bool IsValid() const;

	// バインド管理		
	void Bind(GLuint) const;
	void Unbind(GLuint) const;

private:
	GLuint id = 0;    // オブジェクトID	

};


#endif // SAMPLER_H_INCLUDED
