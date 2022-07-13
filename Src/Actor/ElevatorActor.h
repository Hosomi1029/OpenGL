/**
* @file ElevatorActor.h
*/
#ifndef ELEVATORACTOR_H_INCLOUDED
#define ELEVATORACTOR_H_INCLOUDED
#include "../Actor.h"

/**
* �G���x�[�^�[
*/
class ElevatorActor : public Actor
{
public:
	ElevatorActor(
		const std::string& name,				//�A�N�^�[�̖��O
		const Primitive& prim,				//�`�悷��v���~�e�B�u
		std::shared_ptr<Texture> tex,		//�`��Ɏg���e�N�X�`��
		const glm::vec3& position,			//���̂̈ʒu
		const glm::vec3& scale,				//���̂̊g��k����
		float rotation,						//���̂̉�]�p�x
		const glm::vec3& adjustment			//���̂����_�Ɉړ����邽�߂̋���
	);

	virtual ~ElevatorActor() = default;
	virtual void OnUpdate(float deltaTime);
};

#endif // !ELEVATORACTOR_H_INCLOUDED
