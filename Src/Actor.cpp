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
* アクターの状態を更新する
* 
* @param deltaTime 前回の更新からの経過時間（秒）
*/
void Actor::OnUpdate(float deltaTime)
{
	//なにもしない
}

/**
* 衝突を処理する
*
* @param contact 衝突情報
*/
void Actor::OnCollision(const Contact& contact)
{
	//何もしない
}


/**
* 物体を描画する
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

	//uniform変数の位置.
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;//モデル行列用ユニフォーム変数の位置
	pipeline.SetUniform(locMatTRS, matMVP);
	pipeline.SetUniform(locMatModel, matModel);

	actor.tex->Bind(0);
	actor.prim.Draw();
}

/**
* アクター配列から名前の一致するアクターを検索する
* 
* @param actors 検索対象の配列．
* @param name   検索するアクターの名前．
* 
* @retval nullptr以外	最初にnameと名前が一致したアクターのアドレス．
* @retval nullptr		actorsの中に名前が一致するアクターがない．
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
* 衝突を検出する．
* 
* @param actorA  衝突しているか調べるアクター
* @param actorB	 衝突しているか調べるアクター
* @param contact 衝突情報
* 
* @retval ture  衝突している
* @retval false 衝突していない
*/
bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact)
{
	//動かせない物体同士は衝突しない
	if (actorA.isStatic && actorB.isStatic) {
		return false;
	}

	//ワールド座標系のコライダーを計算する
	BOX a = actorA.collider;
	a.min += actorA.position;
	a.max += actorA.position;

	BOX b = actorB.collider;
	b.min += actorB.position;
	b.max += actorB.position;

	//aの左側面がbの右側面より右にあるなら、衝突していない
	const float dx0 = b.max.x - a.min.x;
	if (dx0 <= 0) { return false; }
	//aの右側面がbの左側面より左にあるなら、衝突していない
	const float dx1 = a.max.x - b.min.x;
	if (dx1 <= 0) { return false; }

	//aの上側面がbの下側面より上にあるなら、衝突していない
	const float dy0 = b.max.y - a.min.y;
	if (dy0 <= 0) { return false; }
	//aの下側面がbの上側面より下にあるなら、衝突していない
	const float dy1 = a.max.y - b.min.y;
	if (dy1 <= 0) { return false; }

	//aの奥側面がbの手前側面より奥にあるなら、衝突していない
	const float dz0 = b.max.z - a.min.z;
	if (dz0 <= 0) { return false; }
	//aの手前側面がbの奥側面より手前にあるなら、衝突していない
	const float dz1 = a.max.z - b.min.z;
	if (dz1 <= 0) { return false; }

	//XYZの各軸について「浸透距離（重なっている部分の長さ）」が短い方向を選択する
	glm::vec3 normal;
	glm::vec3 penetration;	//浸透距離と方向

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

	//浸透距離の絶対値
	glm::vec3 absPenetration = glm::abs(penetration);

	//衝突面になる可能性の高さ
	glm::vec3 score = glm::vec3(0);

	//浸透距離が短い方向のほうが衝突面である可能性が高い(はず)
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

	//相対ベロシティを計算する
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	//浸透が始まった時間を計算する
	glm::vec3 t = glm::vec3(-FLT_MAX);
	for (int i = 0; i < 3; ++i) {
		if (rv[i]) {
			t[i] = penetration[i] / rv[i];
		}
	}

	//浸透が始まった時間ｔが大きいほど、より早い時点で浸透が始まったと考えられる
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

	//より可能性が低い方向を除外する
	//値が等しい場合、Z,X,Yの順で優先的に除外する
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

	//XYZ軸のうち、浸透距離がもっとも短い軸の成分だけを残す
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

	//衝突情報を設定する
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;

	//衝突している
	return true;
}


/**
* 重なりを解決する
* 
* @param contact 衝突情報
*/
void SolveContact(Contact& contact)
{

	Actor& actorA=  *contact.a;
	Actor& actorB = *contact.b;
	glm::vec3 penetration = contact.penetration;
	glm::vec3 normal = contact.normal;

	//衝突面の座標を計算する
	{
		//基本的にアクターBの座標を使うが、アクターBが静物の場合はアクターAの座標を使う
		Actor* target = &actorB;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic) {
			target = &actorA;
			targetNormal *= -1;	//法線の向きを反転する
		}
		//コライダーの半径を計算する
		glm::vec3 halfSize = (target->collider.max - target->collider.min) * 0.5f;
		//コライダーの中心座標を計算する
		glm::vec3 center = (target->collider.max + target->collider.min) * 0.5f;
		//衝突面の座標を計算する
		contact.position = target->position + center - halfSize * targetNormal;
	}

	//浸透距離の長さを計算する
	contact.penLength = glm::length(penetration);

	//反発係数の平均値を計算
	float cor = (actorA.cor + actorB.cor) * 0.5f;

	//摩擦係数の平均値を計算
	float friction = 1.0f - (actorA.friction + actorB.friction) * 0.5f;

	//「アクターAの相対ベロシティ」を計算
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	//衝突面と相対ベロシティに平行なベクトル（タンジェント）を計算
	glm::vec3 tangent = glm::cross(normal, glm::cross(normal, rv));

	//タンジェントを正規化
	if (glm::length(tangent) > 0.000001f) {
		tangent = glm::normalize(tangent);
	}
	else {
		tangent = glm::vec3(0);
	}

	//摩擦力
	float frictionForce = friction * 9.8f / 60.0f;

	//摩擦力の最大値を計算
	float maxForce = std::abs(glm::dot(tangent, rv));

	//摩擦力を最大値に制限
	frictionForce = std::min(frictionForce, maxForce);

	//タンジェント方向の摩擦力を計算
	glm::vec3 frictionVelocity = normal.y * frictionForce * tangent;

	//法線方向の速度を計算
	float ua = glm::dot(normal, actorA.velocity);
	float ub = glm::dot(normal, actorB.velocity);

	if (actorA.isStatic) {
		float vb = ua + cor * (ua - ub);		//衝突後の速度を計算
		actorB.velocity -= normal * ub;			//衝突前の速度を0にする
		actorB.velocity += normal * vb;			//衝突後の速度を加算
		actorB.velocity += frictionVelocity;	//摩擦による速度を加算する

		//重なりの解決:アクターAは動かせないので、アクターBだけ動かす
		actorB.position += penetration;
	}
	else if (actorB.isStatic) {
		float va = ub + cor * (ub - ua);		//衝突後の速度を計算
		actorA.velocity -= normal * ua;			//衝突前の速度を0にする
		actorA.velocity += normal * va;			//衝突後の速度を加算
		actorA.velocity += frictionVelocity;	//摩擦による速度を加算する

		//重なりの解決:アクターBは動かせないので、アクターAだけ動かす
		actorA.position -= penetration;
	}
	else {
		//衝突後の速度を計算
		float massAB = actorA.mass + actorB.mass;
		float c = actorA.mass * ua + actorB.mass * ub;
		float va = (c + cor * actorB.mass * (ub - ua)) / massAB;
		float vb = (c + cor * actorA.mass * (ua - ub)) / massAB;

		//衝突前の速度を0にする
		actorA.velocity -= normal * ua;
		actorB.velocity -= normal * ub;

		//衝突後の速度を加速する
		actorA.velocity += normal * va;
		actorB.velocity += normal * vb;

		//摩擦による速度を加算する
		actorA.velocity -= frictionVelocity;
		actorB.velocity += frictionVelocity;

		//重なりの解決
		actorA.position -= penetration * 0.5f;
		actorB.position += penetration * 0.5f;
	}
}

/**
* 2つのコンタクト構造体が似ているか調べる
* 
* @param ca 比較するコンタクト構造体A
* @parem cb 比較するコンタクト構造体B
* 
* @return true 　似ている
* @return false  似ていない
*/
bool Equal(const Contact& ca, const Contact& cb) {

	//衝突面の距離が離れている場合は似ていない
	if (glm::length(ca.penetration - cb.position) > 0.01f) {
		return false;	//似ていない
	}

	//動かないアクターの有無によって判定を分ける
	bool hasStaticA = ca.a->isStatic || ca.b->isStatic;
	bool hasStaticB = cb.a->isStatic || cb.b->isStatic;
	switch (hasStaticA + hasStaticB * 2)
	{
	case 0b00: //A,Bともに動くアクターのみ
		//アクターが両方ともに一致したら似ている
		if (ca.a == cb.a && ca.b == cb.b) {
			break;
		}
		if (ca.a == cb.b && ca.b == cb.a) {
			break;
		}
		return false;
	case 0b01: //A=動かないアクターを含む　B=動くアクターのみ
		//常に似ていないと判定する
		return false;
	case 0b10: //A＝動くアクターのみ　B=動かないアクターを含む
		//常に似ていないと判定する
		return false;
	case 0b11: //A,Bともに動かないアクターを含む
	{
		//動くアクター同士が一致したら似ている
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
	return true;	//似ている
}

