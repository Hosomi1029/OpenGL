/**
* @file ElevatorActor.cpp
*/
#include "ElevatorActor.h"

/**
* �R���X�g���N�^
*/
ElevatorActor::ElevatorActor(
	const std::string& name,			//�A�N�^�[�̖��O
	const Primitive& prim,				//�`�悷��v���~�e�B�u
	std::shared_ptr<Texture> tex,		//�`��Ɏg���e�N�X�`��
	const glm::vec3& position,			//���̂̈ʒu
	const glm::vec3& scale,				//���̂̊g��k����
	float rotation,						//���̂̉�]�p�x
	const glm::vec3& adjustment) :		//���̂����_�Ɉړ����邽�߂̋���
	Actor(name, prim, tex, position, scale, rotation, adjustment)	//���N���X��������
{
}

/**
* �A�N�^�[�̏�Ԃ��X�V����
*
* @param deltaTime	�O��̍X�V����̌o�ߎ��ԁi�b�j
*/
void ElevatorActor::OnUpdate(float deltaTime)
{
	//�G���x�[�^�[�̈ړ�������؂�ւ���
		//����4m�ɓ��B������A�ړ����������ɂ���
	if (position.y >= 4) {
		position.y = 4;
		velocity.y = -1;
	}
	//����-1m�ɓ��B������A�ړ���������ɂ���
	else if (position.y <= -1) {
		position.y = -1;
		velocity.y = 1;
	}
}