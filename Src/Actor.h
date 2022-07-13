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
* 直方体
*/
struct BOX
{
	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
};

/**
* 物体を制御するパラメータ．
*/
class Actor
{
public:
	Actor(
	const std::string& name,				//アクターの名前
	const Primitive& prim,				//描画するプリミティブ
	std::shared_ptr<Texture> tex,		//描画に使うテクスチャ
	const glm::vec3& position,			//物体の位置
	const glm::vec3& scale,				//物体の拡大縮小率
	float rotation,						//物体の回転角度
	const glm::vec3& adjustment			//物体を原点に移動するための距離
	);
	virtual ~Actor() = default;
	virtual void OnUpdate(float deltaTime);
	virtual void OnCollision(const struct Contact& contact);

	std::string name;				//アクターの名前
	Primitive prim;				//描画するプリミティブ
	std::shared_ptr<Texture> tex;		//描画に使うテクスチャ
	glm::vec3 position;			//物体の位置
	glm::vec3 scale;				//物体の拡大縮小率
	float rotation;						//物体の回転角度
	glm::vec3 adjustment;			//物体を原点に移動するための距離
	glm::vec3 velocity = glm::vec3(0);	//速度(メートル毎秒)
	float lifespan = 0;					//寿命(秒、０以下なら寿命なし)
	bool isDead = false;				//true=死亡(削除待ち) false=生存中
	float health = 10;					//耐久値

	BOX collider;					//衝突判定
	float mass = 1;					//質量(kg)
	float cor = 0.7f;				//反発係数(0.0～1.0)
	float friction = 0.7f;			//摩擦係数(0.0～1.0)
	bool isStatic = false;			//false = 動かせる　true = 動かせない物体
};

void Draw(const Actor& actor,						//物体の制御パラメータ
		  const ProgramPipeline& pipeline,			//描画に使うプログラムパイプライン
		  const glm::mat4& matProj,					//描画に使うプロジェクション行列
		  const glm::mat4& matView);				//描画に使うビュー行列

Actor* Find(std::vector < std::shared_ptr <Actor>>& actors, const char* name);

/**
* 衝突情報
*/
struct Contact {
	Actor* a = nullptr;
	Actor* b = nullptr;
	glm::vec3 velocityA;	//衝突時点でのアクターAのベロシティ
	glm::vec3 velocityB;	//衝突時点でのアクターBのベロシティ
	glm::vec3 penetration;  //浸透距離
	glm::vec3 normal;		//衝突面の法線
	glm::vec3 position;		//衝突面の座標
	float penLength;		//浸透距離の長さ
};
bool DetectCollision(Actor& actorA, Actor& actorB,Contact& contact);
void SolveContact(Contact& contact);
bool Equal(const Contact& ca, const Contact& cb);


#endif//ACTOR_H_INCLUDED
