/**
* @file sampler.cpp
*/

#include "sampler.h"
#include "GLContext.h"
#include <iostream>


Sampler::Sampler(GLenum wrapMode)
{
	id = GLContext::CreateSampler(wrapMode);
	if (id) {
		std::cout << "[���]Sampler���쐬. \n";
	}
	else {
		std::cout << "[�G���[]Sampler�̍쐬�Ɏ��s.\n";
	}
}

Sampler::~Sampler()
{
	if (id) {
		std::cout << "[���]Sampler���폜.\n";
	}
	glDeleteSamplers(1, &id);
}

/**
* �I�u�W�F�N�g���g�����Ԃ��ǂ����𒲂ׂ�D
*
* @retval true  �g����D
* @retval false �g���Ȃ��i�������Ɏ��s���Ă���j�D
*/
bool Sampler::IsValid() const
{
	return id;
}

void Sampler::Bind(GLuint unit) const
{
	glBindSampler(unit, id);
}

void Sampler::Unbind(GLuint unit) const
{
	glBindSampler(unit, 0);
}

