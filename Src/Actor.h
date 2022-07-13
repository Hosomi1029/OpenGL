/**
* @file Actor.h
*/

#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <glad/glad.h>
#include "Primitive.h"
#include "Texture.h"
#include "ProgramPipeline.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

/**
* ������
*/
struct BOX
{
	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
};

/**
* ���̂𐧌䂷��p�����[�^�D
*/
class Actor
{
public:
	Actor(
	const std::string& name,				//�A�N�^�[�̖��O
	const Primitive& prim,				//�`�悷��v���~�e�B�u
	std::shared_ptr<Texture> tex,		//�`��Ɏg���e�N�X�`��
	const glm::vec3& position,			//���̂̈ʒu
	const glm::vec3& scale,				//���̂̊g��k����
	float rotation,						//���̂̉�]�p�x
	const glm::vec3& adjustment			//���̂����_�Ɉړ����邽�߂̋���
	);
	virtual ~Actor() = default;
	virtual void OnUpdate(float deltaTime);
	virtual void OnCollision(const struct Contact& contact);

	std::string name;				//�A�N�^�[�̖��O
	Primitive prim;				//�`�悷��v���~�e�B�u
	std::shared_ptr<Texture> tex;		//�`��Ɏg���e�N�X�`��
	glm::vec3 position;			//���̂̈ʒu
	glm::vec3 scale;				//���̂̊g��k����
	float rotation;						//���̂̉�]�p�x
	glm::vec3 adjustment;			//���̂����_�Ɉړ����邽�߂̋���
	glm::vec3 velocity = glm::vec3(0);	//���x(���[�g�����b)
	float lifespan = 0;					//����(�b�A�O�ȉ��Ȃ�����Ȃ�)
	bool isDead = false;				//true=���S(�폜�҂�) false=������
	float health = 10;					//�ϋv�l

	BOX collider;					//�Փ˔���
	float mass = 1;					//����(kg)
	float cor = 0.7f;				//�����W��(0.0�`1.0)
	float friction = 0.7f;			//���C�W��(0.0�`1.0)
	bool isStatic = false;			//false = ��������@true = �������Ȃ�����
};

void Draw(const Actor& actor,						//���̂̐���p�����[�^
		  const ProgramPipeline& pipeline,			//�`��Ɏg���v���O�����p�C�v���C��
		  const glm::mat4& matProj,					//�`��Ɏg���v���W�F�N�V�����s��
		  const glm::mat4& matView);				//�`��Ɏg���r���[�s��

Actor* Find(std::vector < std::shared_ptr <Actor>>& actors, const char* name);

/**
* �Փˏ��
*/
struct Contact {
	Actor* a = nullptr;
	Actor* b = nullptr;
	glm::vec3 velocityA;	//�Փˎ��_�ł̃A�N�^�[A�̃x���V�e�B
	glm::vec3 velocityB;	//�Փˎ��_�ł̃A�N�^�[B�̃x���V�e�B
	glm::vec3 penetration;  //�Z������
	glm::vec3 normal;		//�Փ˖ʂ̖@��
	glm::vec3 position;		//�Փ˖ʂ̍��W
	float penLength;		//�Z�������̒���
};
bool DetectCollision(Actor& actorA, Actor& actorB,Contact& contact);
void SolveContact(Contact& contact);
bool Equal(const Contact& ca, const Contact& cb);


#endif//ACTOR_H_INCLUDED
