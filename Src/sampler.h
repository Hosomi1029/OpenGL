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

	// �I�u�W�F�N�g�̗L�����𔻒肷��		
	bool IsValid() const;

	// �o�C���h�Ǘ�		
	void Bind(GLuint) const;
	void Unbind(GLuint) const;

private:
	GLuint id = 0;    // �I�u�W�F�N�gID	

};


#endif // SAMPLER_H_INCLUDED
