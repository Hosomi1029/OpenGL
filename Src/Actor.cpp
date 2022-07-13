/**
* @file Actor.cpp
*/
#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>


Actor::Actor(
	const std::string& name, 
	const Primitive& prim, 
	std::shared_ptr<Texture> tex, 
	const glm::vec3& position, 
	const glm::vec3& scale, 
	float rotation, 
	const glm::vec3& adjustment) :
	name(name),
	prim(prim),
	tex(tex),
	position(position),
	scale(scale),
	rotation(rotation),
	adjustment(adjustment)
{
}

/**
* �A�N�^�[�̏�Ԃ��X�V����
* 
* @param deltaTime �O��̍X�V����̌o�ߎ��ԁi�b�j
*/
void Actor::OnUpdate(float deltaTime)
{
	//�Ȃɂ����Ȃ�
}

/**
* �Փ˂���������
*
* @param contact �Փˏ��
*/
void Actor::OnCollision(const Contact& contact)
{
	//�������Ȃ�
}


/**
* ���̂�`�悷��
*/

void Draw(const Actor& actor, const ProgramPipeline& pipeline,
	const glm::mat4& matProj, const glm::mat4& matView)
{
	const glm::mat4 matModel =
		glm::translate(glm::mat4(1), actor.position) *
		glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), actor.scale) *
		glm::translate(glm::mat4(1), actor.adjustment);
	const glm::mat4 matMVP = matProj * matView * matModel;

	//uniform�ϐ��̈ʒu.
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;//���f���s��p���j�t�H�[���ϐ��̈ʒu
	pipeline.SetUniform(locMatTRS, matMVP);
	pipeline.SetUniform(locMatModel, matModel);

	actor.tex->Bind(0);
	actor.prim.Draw();
}

/**
* �A�N�^�[�z�񂩂疼�O�̈�v����A�N�^�[����������
* 
* @param actors �����Ώۂ̔z��D
* @param name   ��������A�N�^�[�̖��O�D
* 
* @retval nullptr�ȊO	�ŏ���name�Ɩ��O����v�����A�N�^�[�̃A�h���X�D
* @retval nullptr		actors�̒��ɖ��O����v����A�N�^�[���Ȃ��D
*/
Actor* Find(std::vector<std::shared_ptr<Actor>>& actors, const char* name)
{
	for (int i = 0; i < actors.size(); ++i) {
		if (actors[i]->name == name)
		{
			return actors[i].get();
		}
	}

	return nullptr;
	
}


/**
* �Փ˂����o����D
* 
* @param actorA  �Փ˂��Ă��邩���ׂ�A�N�^�[
* @param actorB	 �Փ˂��Ă��邩���ׂ�A�N�^�[
* @param contact �Փˏ��
* 
* @retval ture  �Փ˂��Ă���
* @retval false �Փ˂��Ă��Ȃ�
*/
bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact)
{
	//�������Ȃ����̓��m�͏Փ˂��Ȃ�
	if (actorA.isStatic && actorB.isStatic) {
		return false;
	}

	//���[���h���W�n�̃R���C�_�[���v�Z����
	BOX a = actorA.collider;
	a.min += actorA.position;
	a.max += actorA.position;

	BOX b = actorB.collider;
	b.min += actorB.position;
	b.max += actorB.position;

	//a�̍����ʂ�b�̉E���ʂ��E�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dx0 = b.max.x - a.min.x;
	if (dx0 <= 0) { return false; }
	//a�̉E���ʂ�b�̍����ʂ�荶�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dx1 = a.max.x - b.min.x;
	if (dx1 <= 0) { return false; }

	//a�̏㑤�ʂ�b�̉����ʂ���ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy0 = b.max.y - a.min.y;
	if (dy0 <= 0) { return false; }
	//a�̉����ʂ�b�̏㑤�ʂ�艺�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy1 = a.max.y - b.min.y;
	if (dy1 <= 0) { return false; }

	//a�̉����ʂ�b�̎�O���ʂ�艜�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dz0 = b.max.z - a.min.z;
	if (dz0 <= 0) { return false; }
	//a�̎�O���ʂ�b�̉����ʂ���O�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dz1 = a.max.z - b.min.z;
	if (dz1 <= 0) { return false; }

	//XYZ�̊e���ɂ��āu�Z�������i�d�Ȃ��Ă��镔���̒����j�v���Z��������I������
	glm::vec3 normal;
	glm::vec3 penetration;	//�Z�������ƕ���

	if (dx0 <= dx1) { 
		penetration.x = -dx0;
		normal.x = 1;
	}
	else { 
		penetration.x = dx1; 
		normal.x = -1;
	}

	if (dy0 <= dy1) {  
		penetration.y = -dy0; 
		normal.y = 1;
	}
	else { 
		penetration.y = dy1; 
		normal.y = -1;
	}

	if (dz0 <= dz1) { 
		penetration.z = -dz0; 
		normal.z = 1;
	}
	else { 
		penetration.z = dz1;
		normal.z = -1;
	}

	//�Z�������̐�Βl
	glm::vec3 absPenetration = glm::abs(penetration);

	//�Փ˖ʂɂȂ�\���̍���
	glm::vec3 score = glm::vec3(0);

	//�Z���������Z�������̂ق����Փ˖ʂł���\��������(�͂�)
	for (int a = 0; a < 2; ++a) {
		for (int b = a + 1; b < 3; ++b) {
			if (absPenetration[a] < absPenetration[b]) {
				++score[a];
			}
			else {
				++score[b];
			}
		}
	}

	//���΃x���V�e�B���v�Z����
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	//�Z�����n�܂������Ԃ��v�Z����
	glm::vec3 t = glm::vec3(-FLT_MAX);
	for (int i = 0; i < 3; ++i) {
		if (rv[i]) {
			t[i] = penetration[i] / rv[i];
		}
	}

	//�Z�����n�܂������Ԃ����傫���قǁA��葁�����_�ŐZ�����n�܂����ƍl������
	const float deltaTime = 1.0f / 60.0f;
	for (int a = 0; a < 2; ++a) {
		for (int b = a + 1; b < 3; ++b){
			int i = a;
			if (t[a] < t[b]) {
				i = b;
			}
			if (t[i] > 0 && t[i] <= deltaTime) {
				score[i] += 1.5f;
			}
		}
	}

	//���\�����Ⴂ���������O����
	//�l���������ꍇ�AZ,X,Y�̏��ŗD��I�ɏ��O����
	if (score.x <= score.y) {
		normal.x = 0;
		if (score.z <= score.y) {
			normal.z = 0;
		}else {
			normal.y = 0;
		}
	}else {
		normal.y = 0;
		if (score.z <= score.x) {
			normal.z = 0;
		}		else {
			normal.x = 0;
		}
	}

	//XYZ���̂����A�Z�������������Ƃ��Z�����̐����������c��
	if (absPenetration.x >= absPenetration.y) {
		penetration.x = 0;
		if (absPenetration.z >= absPenetration.y) {
			penetration.z = 0;
		}
		else {
			penetration.y = 0;
		}
	}
	else {
		penetration.y = 0;
		if (absPenetration.x >= absPenetration.z) {
			penetration.x = 0;
		}
		else {
			penetration.z = 0;
		}
	}

	//�Փˏ���ݒ肷��
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;

	//�Փ˂��Ă���
	return true;
}


/**
* �d�Ȃ����������
* 
* @param contact �Փˏ��
*/
void SolveContact(Contact& contact)
{

	Actor& actorA=  *contact.a;
	Actor& actorB = *contact.b;
	glm::vec3 penetration = contact.penetration;
	glm::vec3 normal = contact.normal;

	//�Փ˖ʂ̍��W���v�Z����
	{
		//��{�I�ɃA�N�^�[B�̍��W���g�����A�A�N�^�[B���Õ��̏ꍇ�̓A�N�^�[A�̍��W���g��
		Actor* target = &actorB;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic) {
			target = &actorA;
			targetNormal *= -1;	//�@���̌����𔽓]����
		}
		//�R���C�_�[�̔��a���v�Z����
		glm::vec3 halfSize = (target->collider.max - target->collider.min) * 0.5f;
		//�R���C�_�[�̒��S���W���v�Z����
		glm::vec3 center = (target->collider.max + target->collider.min) * 0.5f;
		//�Փ˖ʂ̍��W���v�Z����
		contact.position = target->position + center - halfSize * targetNormal;
	}

	//�Z�������̒������v�Z����
	contact.penLength = glm::length(penetration);

	//�����W���̕��ϒl���v�Z
	float cor = (actorA.cor + actorB.cor) * 0.5f;

	//���C�W���̕��ϒl���v�Z
	float friction = 1.0f - (actorA.friction + actorB.friction) * 0.5f;

	//�u�A�N�^�[A�̑��΃x���V�e�B�v���v�Z
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	//�Փ˖ʂƑ��΃x���V�e�B�ɕ��s�ȃx�N�g���i�^���W�F���g�j���v�Z
	glm::vec3 tangent = glm::cross(normal, glm::cross(normal, rv));

	//�^���W�F���g�𐳋K��
	if (glm::length(tangent) > 0.000001f) {
		tangent = glm::normalize(tangent);
	}
	else {
		tangent = glm::vec3(0);
	}

	//���C��
	float frictionForce = friction * 9.8f / 60.0f;

	//���C�͂̍ő�l���v�Z
	float maxForce = std::abs(glm::dot(tangent, rv));

	//���C�͂��ő�l�ɐ���
	frictionForce = std::min(frictionForce, maxForce);

	//�^���W�F���g�����̖��C�͂��v�Z
	glm::vec3 frictionVelocity = normal.y * frictionForce * tangent;

	//�@�������̑��x���v�Z
	float ua = glm::dot(normal, actorA.velocity);
	float ub = glm::dot(normal, actorB.velocity);

	if (actorA.isStatic) {
		float vb = ua + cor * (ua - ub);		//�Փˌ�̑��x���v�Z
		actorB.velocity -= normal * ub;			//�ՓˑO�̑��x��0�ɂ���
		actorB.velocity += normal * vb;			//�Փˌ�̑��x�����Z
		actorB.velocity += frictionVelocity;	//���C�ɂ�鑬�x�����Z����

		//�d�Ȃ�̉���:�A�N�^�[A�͓������Ȃ��̂ŁA�A�N�^�[B����������
		actorB.position += penetration;
	}
	else if (actorB.isStatic) {
		float va = ub + cor * (ub - ua);		//�Փˌ�̑��x���v�Z
		actorA.velocity -= normal * ua;			//�ՓˑO�̑��x��0�ɂ���
		actorA.velocity += normal * va;			//�Փˌ�̑��x�����Z
		actorA.velocity += frictionVelocity;	//���C�ɂ�鑬�x�����Z����

		//�d�Ȃ�̉���:�A�N�^�[B�͓������Ȃ��̂ŁA�A�N�^�[A����������
		actorA.position -= penetration;
	}
	else {
		//�Փˌ�̑��x���v�Z
		float massAB = actorA.mass + actorB.mass;
		float c = actorA.mass * ua + actorB.mass * ub;
		float va = (c + cor * actorB.mass * (ub - ua)) / massAB;
		float vb = (c + cor * actorA.mass * (ua - ub)) / massAB;

		//�ՓˑO�̑��x��0�ɂ���
		actorA.velocity -= normal * ua;
		actorB.velocity -= normal * ub;

		//�Փˌ�̑��x����������
		actorA.velocity += normal * va;
		actorB.velocity += normal * vb;

		//���C�ɂ�鑬�x�����Z����
		actorA.velocity -= frictionVelocity;
		actorB.velocity += frictionVelocity;

		//�d�Ȃ�̉���
		actorA.position -= penetration * 0.5f;
		actorB.position += penetration * 0.5f;
	}
}

/**
* 2�̃R���^�N�g�\���̂����Ă��邩���ׂ�
* 
* @param ca ��r����R���^�N�g�\����A
* @parem cb ��r����R���^�N�g�\����B
* 
* @return true �@���Ă���
* @return false  ���Ă��Ȃ�
*/
bool Equal(const Contact& ca, const Contact& cb) {

	//�Փ˖ʂ̋���������Ă���ꍇ�͎��Ă��Ȃ�
	if (glm::length(ca.penetration - cb.position) > 0.01f) {
		return false;	//���Ă��Ȃ�
	}

	//�����Ȃ��A�N�^�[�̗L���ɂ���Ĕ���𕪂���
	bool hasStaticA = ca.a->isStatic || ca.b->isStatic;
	bool hasStaticB = cb.a->isStatic || cb.b->isStatic;
	switch (hasStaticA + hasStaticB * 2)
	{
	case 0b00: //A,B�Ƃ��ɓ����A�N�^�[�̂�
		//�A�N�^�[�������Ƃ��Ɉ�v�����玗�Ă���
		if (ca.a == cb.a && ca.b == cb.b) {
			break;
		}
		if (ca.a == cb.b && ca.b == cb.a) {
			break;
		}
		return false;
	case 0b01: //A=�����Ȃ��A�N�^�[���܂ށ@B=�����A�N�^�[�̂�
		//��Ɏ��Ă��Ȃ��Ɣ��肷��
		return false;
	case 0b10: //A�������A�N�^�[�̂݁@B=�����Ȃ��A�N�^�[���܂�
		//��Ɏ��Ă��Ȃ��Ɣ��肷��
		return false;
	case 0b11: //A,B�Ƃ��ɓ����Ȃ��A�N�^�[���܂�
	{
		//�����A�N�^�[���m����v�����玗�Ă���
		Actor* a = ca.a;
		if (ca.a->isStatic) {
			a = ca.b;
		}
		Actor* b = cb.a;
		if (cb.a->isStatic) {
			b = cb.b;
		}
		if (a == b) {
			break;
		}
	}
		return false;
	}
	return true;	//���Ă���
}

