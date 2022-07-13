/**
* @file main.cpp
*/
#include <glad/glad.h> //gl.hの上位互換品
#include "GLContext.h"
#include "Primitive.h"
#include "ProgramPipeline.h"
#include "Texture.h"
#include "sampler.h"
#include "Actor.h"
#include "Actor/T34TankActor.h"
#include "Actor/ElevatorActor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <memory>
#pragma comment(lib,"opengl32.lib")

///色データ.

const glm::vec4 colGround[] = {
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
};

const glm::vec4 colCube[] = {
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
};

const glm::vec4 colTree[]={
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
};

const glm::vec4 colWarehouse[] = {
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
	{1.0f,1.0f,1.0f,1.0f},
};



///マップデータ.
int mapData[20][20] = {
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
	{0,0,0,1,2,2,1,0,0,0,0,0,0,1,2,2,1,0,0,0},
};

///オブジェクトの配置
int objectMapData[20][20] = {
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,2,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,2,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,2,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,2,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,2,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,2,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0},
};

//アクターの配列
std::vector<std::shared_ptr<Actor>> actors;

/**
* OpenGLからのメッセージを処理する。
* 
* @param source			メッセージの発信者(OpenGL,Windows,シェーダー等)
* @param type			メッセージの種類(エラー、警告など)
* @param id				メッセージを一位に識別する値
* @param severity		メッセージの重要性(高、中、低、最低)
* @param length			メッセージの文字数、負数ならメッセージは０に終端されている
* @param message		メッセージ本体
* @param userParam		コールバック設定時に指定したポインタ
*/

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (length < 0) {
		std::cerr << message << "\n";
	}
	else {
		const std::string s(message, message + length);
		std::cerr << s << "\n";
	}
}

/*
* エントリーポイント.
*/
int main()
{
	// GLFWの初期化.
	if (glfwInit() != GLFW_TRUE) {
		return 1;
	}

	//描画ウィンドウの作成
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GLFW_TRUE);
	GLFWwindow* window =
		glfwCreateWindow(1280, 720, "OpenGLGame", nullptr, nullptr); //OpenGLGame:ゲーム名が決まれば変更可能

	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	//// openGL関数のアドレスを取得する.
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	glDebugMessageCallback(DebugCallback, nullptr);
	//VAOを作成する,
	PrimitiveBuffer primitiveBuffer(100'000, 300'000);

	//描画データを追加する
	primitiveBuffer.AddFromObjFile("Res/Ground.obj");
	primitiveBuffer.AddFromObjFile("Res/Cube.obj");
	primitiveBuffer.AddFromObjFile("Res/Tree.obj");
	primitiveBuffer.AddFromObjFile("Res/Warehouse.obj");
	primitiveBuffer.AddFromObjFile("Res/Tiger_I.obj");
	primitiveBuffer.AddFromObjFile("Res/T34.obj");
	primitiveBuffer.AddFromObjFile("Res/BrickHouse.obj");
	primitiveBuffer.AddFromObjFile("Res/Bullet.obj");

	//パイプライン・オブジェクトを作成する
	ProgramPipeline pipeline("Res/FragmentLighting.vert","Res/FragmentLighting.frag");
	if(!pipeline.IsValid()){
		return 1;
	}

	//uniform変数の位置.
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;//モデル行列用ユニフォーム変数の位置

	//座標変換行列の回転角度．
	float degree = 0;

	//拡大・縮小率
	float scale = 2.0;

	//テクスチャを作成.
	std::shared_ptr<Texture> texGround (new Texture("Res/Ground.tga"));
	std::shared_ptr<Texture> texGreen(new Texture("Res/siba.tga"));
	std::shared_ptr<Texture> texRoad(new Texture("Res/asu.tga"));
	std::shared_ptr<Texture> texTree(new Texture("Res/tree.TGA"));
	std::shared_ptr<Texture> texWarehouse(new Texture("Res/Warehouse.tga"));
	std::shared_ptr<Texture> texTank(new Texture("Res/Tiger_I.tga"));
	std::shared_ptr<Texture> texTankT34(new Texture("Res/T34.tga"));
	std::shared_ptr<Texture> texBrickHouse(new Texture("Res/BrickHouse.tga"));
	std::shared_ptr<Texture> texBullet(new Texture("Res/Bullet.tga"));
	
	//サンプラを作成．
	std::shared_ptr<Sampler> sampler(new Sampler(GL_REPEAT));

	//マップに配置する物体の表示データ．
	struct ObjectData {
		const char* name;
		Primitive prim;	//表示するプリミティブ．
		const std::shared_ptr<Texture> tex;		//プリミティブに貼るテクスチャ．
	};

	//描画する物体のリスト．
	const ObjectData objectList[] = {
		{"",Primitive(),0},	//なし.
		{"Tree",primitiveBuffer.Get(2),texTree},	//木
		{"warehouse",primitiveBuffer.Get(3),texWarehouse},//家
	};


	//オブジェクトの配置
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			// 描画する物体の番号を取得.
			const int objectNo = objectMapData[y][x];
			if (objectNo <= 0 || objectNo >= std::size(objectList)) {
				continue;
			}

			const ObjectData p = objectList[objectNo];

			// 四角形が4x4mなので、xとyを4倍した位置に表示する.
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			actors.push_back(std::shared_ptr<Actor>(new Actor{ p.name,p.prim,p.tex,
				position,glm::vec3(1),0.0f,glm::vec3(0) }));
		}

	}

	const ObjectData mapList[] = {
		{"Green",primitiveBuffer.Get(0),texGreen},
		{"Ground",primitiveBuffer.Get(0),texGround},
		{"Road",primitiveBuffer.Get(0),texRoad}
	};

	//マップを（-20,-20)-(-20,20)の範囲に描画する．
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
		    const int mapNo = mapData[y][x];
			if (mapNo < 0 || mapNo >= std::size(objectList)) {
				continue;
			}
			const ObjectData m = mapList[mapNo];

			//四角形が4x4mなので、xとyを4倍した位置に表示する．
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			actors.push_back(std::shared_ptr<Actor>(new Actor{m.name,m.prim,m.tex,
				position,glm::vec3(1),0.0f,glm::vec3(0) }));
			actors.back()->collider = BOX{ glm::vec3(-2,-10,-2),glm::vec3(2,0,2) };
			actors.back()->isStatic = true;
		}
	}

	//エレベーターのパラメータ
	{
		const glm::vec3 position(-4, -1, -4);
		actors.push_back(std::shared_ptr<Actor>(new ElevatorActor{ 
			"Elevator",primitiveBuffer.Get(0),texGround,position,glm::vec3(1),0.0f,glm::vec3(0) }));
		
		actors.back()->velocity.y = 1;
		actors.back()->collider = BOX{ glm::vec3(-2,-10,-2),glm::vec3(2,0,2) };
		actors.back()->isStatic = true;
	}


	//戦車のパラメータ
	std::shared_ptr<Actor> playerTank(new Actor{ "Tiger-I",primitiveBuffer.Get(4),texTank,
	glm::vec3(0),glm::vec3(1),0.0f,glm::vec3(0) });
	actors.back()->collider = BOX{ glm::vec3(-2,0,-2),glm::vec3(2,3,2) };
	actors.push_back(playerTank);

	//T340のパラメータ
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(-5, 0, 0),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34戦車に衝突判定をつける
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };

	//T341のパラメータ
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(1, 0, 8),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34戦車に衝突判定をつける
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };

	//T342のパラメータ
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(10, 0, 0),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34戦車に衝突判定をつける
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };


	//建物のパラメータ
	actors.push_back(std::shared_ptr<Actor>(new Actor{ "BrickHouse",primitiveBuffer.Get(6),texBrickHouse,
	glm::vec3(-1, 0, -8),glm::vec3(3),0.0f,glm::vec3(-2.6f, 2.0f, 0.8f) }));
	actors.back()->collider = BOX{ glm::vec3(-3.0f,0,-2.0f),glm::vec3(3.0f,2.5f,2.0f) };
	actors.back()->isStatic = true;



	//経過時間を一定にする
	double loopTime = glfwGetTime();			//1/60秒間隔でループ処理するための時刻変数
	double diffLoopTime = 0;					//時刻の差分
	const float deltaTime = 1.0f / 60.0f;		//更新間隔
	int oldShotButton = 0;						//前回のショットボタンの状態
	glm::vec3 cameraPosition = glm::vec3(0, 20, 20);	//カメラの座標	
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);		//カメラの注視点の座標

	//メインループ
	while (!glfwWindowShouldClose(window)) {
		//現在時刻を取得
		const double curLoopTime = glfwGetTime();
		//現在時刻と前回時刻の差を、時刻の差分に加算
		diffLoopTime += curLoopTime - loopTime;
		//前回時刻を現在時刻に更新
		loopTime = curLoopTime;
		//時刻の差分が1/60秒未満なら、ループの先頭に戻る
		if (diffLoopTime < deltaTime) {
			continue;
		}

		//
		//ゲーム状態を更新する
		//

		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime) {
			//戦車を移動させる
			Actor* tank = Find(actors, "Tiger-I");
			if (tank) {
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
					tank->rotation += glm::radians(90.0f) * deltaTime;
				}
				else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
					tank->rotation -= glm::radians(90.0f) * deltaTime;
				}

				//tank.rotationが0の時の戦車の向きベクトル
				glm::vec3 tankFront(0, 0, 1);
				//tank.rotationラジアンだけ回転させる回転行列を作る
				const glm::mat4 matRot = glm::rotate(glm::mat4(1), tank->rotation, glm::vec3(0, 1, 0));
				//向きベクトルをrotTankだけ回転させる
				tankFront = matRot * glm::vec4(tankFront, 1);

				float tankAccel = 0.2f;	//戦車の加速度
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
					tank->velocity += tankFront * tankAccel;
				}
				else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
					tank->velocity -= tankFront * tankAccel;
				}

				//マウス左ボタンの状態を取得する
				int shotButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

				//マウス左ボタンが押されていたら弾アクターを追加する
				if (shotButton == GLFW_PRESS && oldShotButton != GLFW_PRESS) {

					//発射位置を砲の先端に設定
					glm::vec3 position = tank->position + tankFront * 6.0f;
					position.y += 2.0f;

					std::shared_ptr<Actor> bullet(new Actor{ "Bullet",primitiveBuffer.Get(7),texBullet,
					position,glm::vec3(0.25f),tank->rotation,glm::vec3(0) });

					//1.5秒後に弾を消す
					bullet->lifespan = 1.5f;

					//戦車の向いてる方向に、30m/sの速度で移動させる
					bullet->velocity = tankFront * 30.0f;

					//弾に衝突判定を付ける
					bullet->collider = BOX{ glm::vec3(-0.25f),glm::vec3(0.25f) };


					actors.push_back(bullet);
				}

				//「前回のショットボタンの状態」を更新する
				oldShotButton = shotButton;
			}
		
			//アクターの状態を更新する
			for (int i = 0; i < actors.size(); ++i) {
				//アクターの寿命を減らす
				if (actors[i]->lifespan > 0) {
					actors[i]->lifespan -= deltaTime;

					//寿命の尽きたアクターを「削除待ち」状態にする
					if (actors[i]->lifespan <= 0) {
						actors[i]->isDead = true;
						continue;	//削除待ちアクターは更新をスキップ
					}
				}

				actors[i]->OnUpdate(deltaTime);

			
				//速度に重力加速度を加える
				if (!actors[i]->isStatic) {
					actors[i]->velocity.y += -9.8f * deltaTime;
				}

				//アクターの位置を更新する
				actors[i]->position += actors[i]->velocity * deltaTime;
			}

			//衝突情報の配列を用意する
			std::vector<Contact> contacts;
			contacts.reserve(actors.size());

			//アクターの衝突判定を行う
			for (int a = 0; a < actors.size(); ++a) {
				for (int b = a + 1; b < actors.size(); ++b) {
					//削除待ちアクターは衝突しない
					if (actors[a]->isDead) { break; }
					else if (actors[b]->isDead) { continue; }

					Contact contact;

					if (DetectCollision(*actors[a], *actors[b],contact)) {
						//配列の中に、作成したコンタクト構造体と似ているものがあるか調べる
						auto itr = std::find_if(contacts.begin(), contacts.end(),
							[&contact](const Contact& c) {return Equal(contact, c); });

						//似ているコンタクト構造体が見つからなければ、作成した構造体を配列に追加する
						if (itr == contacts.end()) {
							contacts.push_back(contact);
						}
						else {
							//似ている構造体が見つかって場合、浸透距離が長いほうを残す
							if (contact.penLength > itr->penLength) {
								*itr = contact;
							}
						}

					}
				}
			}

			//重なりを解決する
			for (int i = 0; i < contacts.size(); ++i) {
				Contact& c = contacts[i];

				//衝突方向を逆にした衝突情報を作る
				Contact contactBtoA;
				contactBtoA.a = c.b;
				contactBtoA.b = c.a;
				contactBtoA.velocityA = c.velocityB;
				contactBtoA.velocityB = c.velocityA;
				contactBtoA.penetration = -c.penetration;
				contactBtoA.normal = -c.normal;
				contactBtoA.position = c.position;
				contactBtoA.penLength = c.penLength;

				//衝突処理関数を呼び出す
				c.a->OnCollision(c);
				c.b->OnCollision(contactBtoA);

				SolveContact(c);
			}

			//削除待ちのアクターを削除する
			actors.erase(
				std::remove_if(
					actors.begin(), actors.end(), 
					[](const std::shared_ptr<Actor>& a) {return a->isDead; }),
				actors.end());

			//カメラデータを更新する
			{
				Actor* target = Find(actors, "Tiger-I");
				if (target) {
					cameraPosition = target->position + glm::vec3(0, 20, 20);
					cameraTarget = target->position;
				}
			}
		}
		//
		//ゲーム状態を描画する
		//

		glEnable(GL_DEPTH_TEST);			//深度バッファを有効にする．
		glDisable(GL_CULL_FACE);
		glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		primitiveBuffer.BindVertexArray();
		pipeline.Bind();
		sampler->Bind(0);

		float s = sin(glm::radians(degree));
		float c = cos(glm::radians(degree));
		//degree += 0.01f;

		glm::mat4 matT = glm::mat4(1);
		matT[3] = glm::vec4(-0.3, -0.5, 0.0, 1.0);
		glm::mat4 matS = glm::mat4(1);
		matS[0][0] = scale;
		matS[1][1] = scale;
		//scale -= 0.001f;

		glm::mat4 matR = glm::mat4(1);
		matR[0][0] = c;
		matR[0][1] = -s;
		matR[1][0] = s;
		matR[1][1] = c;

		//プロダクション行列を作成．
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj =
			glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		//ビュー行列を作成．
		const glm::mat4 matView =
			glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0, 3, 0));

		//行列をシェーダに転送する.
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		pipeline.SetUniform(locMatTRS, matMVP);
		pipeline.SetUniform(locMatModel, matModel);


		//アクターを描画する
		for (int i = 0; i < actors.size(); ++i)
		{
			Draw(*actors[i], pipeline, matProj, matView);
		}

		// テクスチャの割り当てを解除.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		sampler->Unbind(0);
		pipeline.Unbind();
		primitiveBuffer.BindVertexArray();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFWの終了
	glfwTerminate();

	return 0;
}