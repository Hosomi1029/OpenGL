/**
* @file GameEngine.cpp
*/
#include "GameEngine.h"

namespace {

	GameEngine* engine = nullptr;

}//namespace

/**
* �Q�[���G���W���̏�����
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
* �Q�[���G���W���̏I��
*/
void GameEngine::Finalize()
{
	delete engine;
	engine = nullptr;
}

/**
* �Q�[���G���W�����擾
*/
GameEngine& GameEngine::Get() {
	return *engine;
}