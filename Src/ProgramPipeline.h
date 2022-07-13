/**
* @file ProgramPipeline.h
*/

#ifndef PROGTAMPIPELINE_H_INCLUDED
#define PROGTAMPIPELINE_H_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>

/**
* �V�F�[�_�[�v���O�������Ǘ�����N���X�D
*/
class ProgramPipeline
{
public:
	ProgramPipeline(const char* vsCode, const char* fsCode);
	~ProgramPipeline();

	//�I�u�W�F�N�g�̗L�����𔻒肷��
	bool IsValid() const;

	//���j�t�H�[���ϐ��̐ݒ�
	bool SetUniform(GLint, const glm::mat4&)const;

	//�o�C���h�Ǘ�
	void Bind()const;
	void Unbind()const;

private:
	GLuint vp = 0;		//�v���O�����E�I�u�W�F�N�g�i���_�V�F�[�_�[�j
	GLuint fp = 0;		//�v���O�����E�I�u�W�F�N�g�i�t���O�����g�V�F�[�_�[�j
	GLuint pipeline = 0;//�v���O�����E�p�C�v���C���E�I�u�W�F�N�g

};



#endif //PROGTAMPIPELINE_H_INCLUDED