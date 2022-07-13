/**
* @file ElevatorActor.cpp
*/
#include "ElevatorActor.h"

/**
* コンストラクタ
*/
ElevatorActor::ElevatorActor(
	const std::string& name,			//アクターの名前
	const Primitive& prim,				//描画するプリミティブ
	std::shared_ptr<Texture> tex,		//描画に使うテクスチャ
	const glm::vec3& position,			//物体の位置
	const glm::vec3& scale,				//物体の拡大縮小率
	float rotation,						//物体の回転角度
	const glm::vec3& adjustment) :		//物体を原点に移動するための距離
	Actor(name, prim, tex, position, scale, rotation, adjustment)	//基底クラスを初期化
{
}

/**
* アクターの状態を更新する
*
* @param deltaTime	前回の更新からの経過時間（秒）
*/
void ElevatorActor::OnUpdate(float deltaTime)
{
	//エレベーターの移動方向を切り替える
		//高さ4mに到達したら、移動方向を下にする
	if (position.y >= 4) {
		position.y = 4;
		velocity.y = -1;
	}
	//高さ-1mに到達したら、移動方向を上にする
	else if (position.y <= -1) {
		position.y = -1;
		velocity.y = 1;
	}
}