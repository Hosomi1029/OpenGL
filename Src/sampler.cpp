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
		std::cout << "[情報]Samplerを作成. \n";
	}
	else {
		std::cout << "[エラー]Samplerの作成に失敗.\n";
	}
}

Sampler::~Sampler()
{
	if (id) {
		std::cout << "[情報]Samplerを削除.\n";
	}
	glDeleteSamplers(1, &id);
}

/**
* オブジェクトが使える状態かどうかを調べる．
*
* @retval true  使える．
* @retval false 使えない（初期化に失敗している）．
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

