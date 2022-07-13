/**
* @file GameEngine.h
*/

#ifndef GAMEENGINE_H_INCLUDED
#define GAMEENGINE_H_INCLUDED
#include "Primitive.h"
#include "Texture.h"
#include "Actor.h"
#include <unordered_map>
#include <random>

using ActorList = std::vector<std::shared_ptr<Actor>>;
using TextureBuffer = std::unordered_map<std::string, std::shared_ptr<Texture>>;

/**
*  �Q�[���G���W��
*/
class GameEngine
{
public:
	static bool Initialize();
	static void Finalize();
	static GameEngine& Get();

	ActorList& GetActors() { return actors; }
	void AddActor(std::shared_ptr<Actor>actor) { newActors.push_back(actor); }
	void UpdateActors();
	 
	PrimitiveBuffer& GetPrimitiveBuffer() { return *primitiveBuffer; }
	const Primitive& GetPrimitive(int n) { return primitiveBuffer->Get(n); }

	std::shared_ptr<Texture> LoadTexture(const char* filename);

private:
	GameEngine() = default;
	~GameEngine() = default;
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator = (const GameEngine&) = delete;

	ActorList actors;		//�A�N�^�[�z��
	ActorList newActors;	//�ǉ�����A�N�^�[�̔z��
	std::shared_ptr<PrimitiveBuffer> primitiveBuffer;	//�v���~�e�B�u�z��
	TextureBuffer textureBuffer;						//�e�N�X�`���z��
};

#endif // !GAMEENGINE_H_INCLUDED
