/**
* @file Texture.cpp
*/

#include "Texture.h"
#include "GLContext.h"
#include <iostream>

/**
* �R���X�g���N�^�D
* 
* ��param filename �摜�t�@�C��
*/

Texture::Texture(const char* filename)
{
	id = GLContext::CreateImage2D(filename);
	if (id) {
		name = filename;
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << filename << "���쐬�D\n";
	}
	else {
		std::cout << "[�G���[]" << __func__ << "�e�N�X�`��" << filename << "�̍쐬�Ɏ��s�D\n";
	}
}

/**
* �f�X�g���N�^�D
*/

Texture::~Texture()
{
	if (id) {
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << name << "���폜�D\n";
	}
	glDeleteTextures(1, &id);
}

/**
* �I�u�W�F�N�g���g�����Ԃ��ǂ����𒲂ׂ�D
* 
* @retval true  �g����D
* @retval false �g���Ȃ��i�������Ɏ��s���Ă���j�D
*/
bool Texture::IsValid() const
{
	return id;
}

/**
* �e�N�X�`�����O���t�B�b�N�X�p�C�v���C���Ɋ��蓖�Ă�D
* 
* @param unit ���蓖�Ă�e�N�X�`���C���[�W���j�b�g�̔ԍ��D
*/

void Texture::Bind(GLuint unit) const
{
	glBindTextureUnit(unit, id);
}

/**
* �e�N�X�`���̊��蓖�Ă���������D
* 
* @param unit ���蓖�ĉ�������e�N�X�`���C���[�W���j�b�g�̔ԍ��D
*/
void Texture::Unbind(GLuint unit) const {
	glBindTextureUnit(unit, 0);
}