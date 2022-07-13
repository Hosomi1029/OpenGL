/**
* @file T34TankActor.h
*/
#ifndef T34TANKACTOR_H_INCLUDED
#define T34TANKACTOR_H_INCLUDED
#include "../Actor.h"

class T34TankActor : public Actor
{
public:
	T34TankActor(
		const std::string& name,			//�A�N�^�[�̖��O
		const Primitive& prim,				//�`�悷��v���~�e�B�u
		std::shared_ptr<Texture> tex,		//�`��Ɏg���e�N�X�`��
		const glm::vec3& position,			//���̂̈ʒu
		const glm::vec3& scale,				//���̂̊g��k����
		float rotation,						//���̂̉�]�p�x
		const glm::vec3& adjustment,		//���̂����_�Ɉړ����邽�߂̋���
		const std::shared_ptr<Actor>& target
	);

	virtual ~T34TankActor() = default;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnCollision(const struct Contact& contact) override;

private:
	std::shared_ptr<Actor> target;	//�ǂ�������Ώۂ̃A�N�^�[
};

#endif //T34TANKACTOR_H_INCLUDED