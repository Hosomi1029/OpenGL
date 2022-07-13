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
		const std::string& name,			//アクターの名前
		const Primitive& prim,				//描画するプリミティブ
		std::shared_ptr<Texture> tex,		//描画に使うテクスチャ
		const glm::vec3& position,			//物体の位置
		const glm::vec3& scale,				//物体の拡大縮小率
		float rotation,						//物体の回転角度
		const glm::vec3& adjustment,		//物体を原点に移動するための距離
		const std::shared_ptr<Actor>& target
	);

	virtual ~T34TankActor() = default;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnCollision(const struct Contact& contact) override;

private:
	std::shared_ptr<Actor> target;	//追いかける対象のアクター
};

#endif //T34TANKACTOR_H_INCLUDED