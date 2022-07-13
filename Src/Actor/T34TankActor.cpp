/**
* @file T34TankActor.cpp
*/
#include "T34TankActor.h"
#include <glm/ext.hpp>

/**
* �R���X�g���N�^
*/

T34TankActor::T34TankActor(
	const std::string& name,
	const Primitive& prim,
	std::shared_ptr<Texture> tex,
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	const glm::vec3& adjustment,
	const std::shared_ptr<Actor>& target):
	Actor(name, prim, tex, position, scale, rotation, adjustment),	//���N���X��������
	target(target)
{
}

/**
* �A�N�^�[�̏�Ԃ��X�V����
*
* @param deltaTime �O��̍X�V����o�ߎ��ԁi�b�j
*/
void T34TankActor::OnUpdate(float deltaTime)
{
	//T-34��Ԃ��ړ�������
	//�ǐՑΏۃA�N�^�[���F�肳��Ă���ꍇ�̏���
	if (target) {
		//T-34��Ԃ̐��ʕ����̃x�N�g�����v�Z
		glm::mat4 matR =
			glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		glm::vec3 t34Front = matR * glm::vec4(0, 0, 1, 1);

		//T-34��Ԃ���^�C�K�[I��Ԃւ̃x�N�g�������v�Z
		glm::vec3 d = target->position - position;

		//T-34��Ԃ���^�C�K�[�h��Ԃւ̋������v�Z
		float length = glm::length(d);

		//�x�N�g��d�𐳋K��
		d = glm::normalize(d);

		//T-34��Ԃ̐��ʃx�N�g���ƁA�^�C�K�[I��Ԃւ̃x�N�g���̊p�x��
		float r = std::acos(glm::dot(t34Front, d));

		//T-34��Ԃ̐��ʂƃ^�C�K�[I��Ԃ̂�������̊p�x��10�x�����̏ꍇ...
		if (r < glm::radians(10.0f)) {
			//�^�C�K�[I��Ԃ܂ł̋�����15m��艓���ꍇ�͑O�ɉ���
			if (length > 15.0f) {
				velocity += t34Front * 0.3f;
			}
			//15m�����̏ꍇ�͌�������~
			else {
				//�x���V�e�B��t34Front�����̒������v�Z
				float v = glm::dot(t34Front, velocity);
				//������0.2�ȏ�Ȃ�0.2�������A����ȉ��Ȃ璷�����������i����~�j����
				velocity -= t34Front * glm::clamp(v, -0.2f, 0.2f);
			}
		}
		//�p�x��10�x�ȏ�̏ꍇ�c
		else {
			//T-34��Ԃ̐��ʃx�N�g���ƁA�^�C�K�[I��Ԃւ̃x�N�g���̊O�ς��v�Z
			glm::vec3 n = glm::cross(t34Front, d);
			//y��0�ȏ�Ȃ甽���v���A0�����Ȃ玞�v���ɉ�]����ق����߂�
			if (n.y >= 0) {
				rotation += glm::radians(90.0f) * deltaTime;
			}
			else {
				rotation -= glm::radians(90.0f) * deltaTime;
			}
		}
	}
}

/**
* �Փ˂���������
*
* @param contact �Փˏ��
*/
void T34TankActor::OnCollision(const struct Contact& contact)
{
	//T-34��Ԃƒe�̏Փ˂���������
	if (contact.b->name == "Bullet") {
		//T-34��Ԃ̑ϋv�l�����炷
		health -= 1;
		//�ϋv�l��0�ȉ��ɂȂ�����T-34��Ԃ���������
		if (health <= 0) {
			isDead = true;	//T-34��Ԃ���������
		}
		contact.b->isDead = true;
	}

}
