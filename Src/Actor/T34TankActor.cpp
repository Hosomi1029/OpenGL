/**
* @file T34TankActor.cpp
*/
#include "T34TankActor.h"
#include <glm/ext.hpp>

/**
* コンストラクタ
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
	Actor(name, prim, tex, position, scale, rotation, adjustment),	//基底クラスを初期化
	target(target)
{
}

/**
* アクターの状態を更新する
*
* @param deltaTime 前回の更新から経過時間（秒）
*/
void T34TankActor::OnUpdate(float deltaTime)
{
	//T-34戦車を移動させる
	//追跡対象アクターが認定されている場合の処理
	if (target) {
		//T-34戦車の正面方向のベクトルを計算
		glm::mat4 matR =
			glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		glm::vec3 t34Front = matR * glm::vec4(0, 0, 1, 1);

		//T-34戦車からタイガーI戦車へのベクトルｄを計算
		glm::vec3 d = target->position - position;

		//T-34戦車からタイガーＩ戦車への距離を計算
		float length = glm::length(d);

		//ベクトルdを正規化
		d = glm::normalize(d);

		//T-34戦車の正面ベクトルと、タイガーI戦車へのベクトルの角度差
		float r = std::acos(glm::dot(t34Front, d));

		//T-34戦車の正面とタイガーI戦車のいる方向の角度が10度未満の場合...
		if (r < glm::radians(10.0f)) {
			//タイガーI戦車までの距離が15mより遠い場合は前に加速
			if (length > 15.0f) {
				velocity += t34Front * 0.3f;
			}
			//15m未満の場合は減速→停止
			else {
				//ベロシティのt34Front方向の長さを計算
				float v = glm::dot(t34Front, velocity);
				//長さが0.2以上なら0.2を減速、それ以下なら長さ分を減速（＝停止）する
				velocity -= t34Front * glm::clamp(v, -0.2f, 0.2f);
			}
		}
		//角度が10度以上の場合…
		else {
			//T-34戦車の正面ベクトルと、タイガーI戦車へのベクトルの外積を計算
			glm::vec3 n = glm::cross(t34Front, d);
			//yが0以上なら反時計回り、0未満なら時計回りに回転するほうが近い
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
* 衝突を処理する
*
* @param contact 衝突情報
*/
void T34TankActor::OnCollision(const struct Contact& contact)
{
	//T-34戦車と弾の衝突を処理する
	if (contact.b->name == "Bullet") {
		//T-34戦車の耐久値を減らす
		health -= 1;
		//耐久値が0以下になったらT-34戦車を消去する
		if (health <= 0) {
			isDead = true;	//T-34戦車を消去する
		}
		contact.b->isDead = true;
	}

}
