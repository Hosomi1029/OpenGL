/**
* @file ElevatorActor.h
*/
#ifndef ELEVATORACTOR_H_INCLOUDED
#define ELEVATORACTOR_H_INCLOUDED
#include "../Actor.h"

/**
* エレベーター
*/
class ElevatorActor : public Actor
{
public:
	ElevatorActor(
		const std::string& name,				//アクターの名前
		const Primitive& prim,				//描画するプリミティブ
		std::shared_ptr<Texture> tex,		//描画に使うテクスチャ
		const glm::vec3& position,			//物体の位置
		const glm::vec3& scale,				//物体の拡大縮小率
		float rotation,						//物体の回転角度
		const glm::vec3& adjustment			//物体を原点に移動するための距離
	);

	virtual ~ElevatorActor() = default;
	virtual void OnUpdate(float deltaTime);
};

#endif // !ELEVATORACTOR_H_INCLOUDED
