/**
* @file GameEngine.cpp
*/
#include "GameEngine.h"

namespace {

	GameEngine* engine = nullptr;

}//namespace

/**
* ゲームエンジンの初期化
*/
bool GameEngine::Initialize()
{
	if (!engine) {
		engine = new GameEngine;
		if (!engine) {
			return false;
		}

		engine->actors.reserve(1000);
		engine->newActors.reserve(1000);
		engine->primitiveBuffer.reset(new PrimitiveBuffer(1'000'000, 4'000'000));
		engine->textureBuffer.reserve(1000);
	}
	return true;
}

/**
* ゲームエンジンの終了
*/
void GameEngine::Finalize()
{
	delete engine;
	engine = nullptr;
}

/**
* ゲームエンジンを取得
*/
GameEngine& GameEngine::Get() {
	return *engine;
}