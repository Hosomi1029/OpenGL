/**
* @file main.cpp
*/
#include <glad/glad.h> //gl.h�̏�ʌ݊��i
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

///�F�f�[�^.

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



///�}�b�v�f�[�^.
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

///�I�u�W�F�N�g�̔z�u
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

//�A�N�^�[�̔z��
std::vector<std::shared_ptr<Actor>> actors;

/**
* OpenGL����̃��b�Z�[�W����������B
* 
* @param source			���b�Z�[�W�̔��M��(OpenGL,Windows,�V�F�[�_�[��)
* @param type			���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)
* @param id				���b�Z�[�W����ʂɎ��ʂ���l
* @param severity		���b�Z�[�W�̏d�v��(���A���A��A�Œ�)
* @param length			���b�Z�[�W�̕������A�����Ȃ烁�b�Z�[�W�͂O�ɏI�[����Ă���
* @param message		���b�Z�[�W�{��
* @param userParam		�R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
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
* �G���g���[�|�C���g.
*/
int main()
{
	// GLFW�̏�����.
	if (glfwInit() != GLFW_TRUE) {
		return 1;
	}

	//�`��E�B���h�E�̍쐬
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GLFW_TRUE);
	GLFWwindow* window =
		glfwCreateWindow(1280, 720, "OpenGLGame", nullptr, nullptr); //OpenGLGame:�Q�[���������܂�ΕύX�\

	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	//// openGL�֐��̃A�h���X���擾����.
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	glDebugMessageCallback(DebugCallback, nullptr);
	//VAO���쐬����,
	PrimitiveBuffer primitiveBuffer(100'000, 300'000);

	//�`��f�[�^��ǉ�����
	primitiveBuffer.AddFromObjFile("Res/Ground.obj");
	primitiveBuffer.AddFromObjFile("Res/Cube.obj");
	primitiveBuffer.AddFromObjFile("Res/Tree.obj");
	primitiveBuffer.AddFromObjFile("Res/Warehouse.obj");
	primitiveBuffer.AddFromObjFile("Res/Tiger_I.obj");
	primitiveBuffer.AddFromObjFile("Res/T34.obj");
	primitiveBuffer.AddFromObjFile("Res/BrickHouse.obj");
	primitiveBuffer.AddFromObjFile("Res/Bullet.obj");

	//�p�C�v���C���E�I�u�W�F�N�g���쐬����
	ProgramPipeline pipeline("Res/FragmentLighting.vert","Res/FragmentLighting.frag");
	if(!pipeline.IsValid()){
		return 1;
	}

	//uniform�ϐ��̈ʒu.
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;//���f���s��p���j�t�H�[���ϐ��̈ʒu

	//���W�ϊ��s��̉�]�p�x�D
	float degree = 0;

	//�g��E�k����
	float scale = 2.0;

	//�e�N�X�`�����쐬.
	std::shared_ptr<Texture> texGround (new Texture("Res/Ground.tga"));
	std::shared_ptr<Texture> texGreen(new Texture("Res/siba.tga"));
	std::shared_ptr<Texture> texRoad(new Texture("Res/asu.tga"));
	std::shared_ptr<Texture> texTree(new Texture("Res/tree.TGA"));
	std::shared_ptr<Texture> texWarehouse(new Texture("Res/Warehouse.tga"));
	std::shared_ptr<Texture> texTank(new Texture("Res/Tiger_I.tga"));
	std::shared_ptr<Texture> texTankT34(new Texture("Res/T34.tga"));
	std::shared_ptr<Texture> texBrickHouse(new Texture("Res/BrickHouse.tga"));
	std::shared_ptr<Texture> texBullet(new Texture("Res/Bullet.tga"));
	
	//�T���v�����쐬�D
	std::shared_ptr<Sampler> sampler(new Sampler(GL_REPEAT));

	//�}�b�v�ɔz�u���镨�̂̕\���f�[�^�D
	struct ObjectData {
		const char* name;
		Primitive prim;	//�\������v���~�e�B�u�D
		const std::shared_ptr<Texture> tex;		//�v���~�e�B�u�ɓ\��e�N�X�`���D
	};

	//�`�悷�镨�̂̃��X�g�D
	const ObjectData objectList[] = {
		{"",Primitive(),0},	//�Ȃ�.
		{"Tree",primitiveBuffer.Get(2),texTree},	//��
		{"warehouse",primitiveBuffer.Get(3),texWarehouse},//��
	};


	//�I�u�W�F�N�g�̔z�u
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			// �`�悷�镨�̂̔ԍ����擾.
			const int objectNo = objectMapData[y][x];
			if (objectNo <= 0 || objectNo >= std::size(objectList)) {
				continue;
			}

			const ObjectData p = objectList[objectNo];

			// �l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������.
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

	//�}�b�v���i-20,-20)-(-20,20)�͈̔͂ɕ`�悷��D
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
		    const int mapNo = mapData[y][x];
			if (mapNo < 0 || mapNo >= std::size(objectList)) {
				continue;
			}
			const ObjectData m = mapList[mapNo];

			//�l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������D
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			actors.push_back(std::shared_ptr<Actor>(new Actor{m.name,m.prim,m.tex,
				position,glm::vec3(1),0.0f,glm::vec3(0) }));
			actors.back()->collider = BOX{ glm::vec3(-2,-10,-2),glm::vec3(2,0,2) };
			actors.back()->isStatic = true;
		}
	}

	//�G���x�[�^�[�̃p�����[�^
	{
		const glm::vec3 position(-4, -1, -4);
		actors.push_back(std::shared_ptr<Actor>(new ElevatorActor{ 
			"Elevator",primitiveBuffer.Get(0),texGround,position,glm::vec3(1),0.0f,glm::vec3(0) }));
		
		actors.back()->velocity.y = 1;
		actors.back()->collider = BOX{ glm::vec3(-2,-10,-2),glm::vec3(2,0,2) };
		actors.back()->isStatic = true;
	}


	//��Ԃ̃p�����[�^
	std::shared_ptr<Actor> playerTank(new Actor{ "Tiger-I",primitiveBuffer.Get(4),texTank,
	glm::vec3(0),glm::vec3(1),0.0f,glm::vec3(0) });
	actors.back()->collider = BOX{ glm::vec3(-2,0,-2),glm::vec3(2,3,2) };
	actors.push_back(playerTank);

	//T340�̃p�����[�^
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(-5, 0, 0),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34��ԂɏՓ˔��������
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };

	//T341�̃p�����[�^
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(1, 0, 8),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34��ԂɏՓ˔��������
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };

	//T342�̃p�����[�^
	actors.push_back(std::shared_ptr<Actor>(new T34TankActor{ "T-34",primitiveBuffer.Get(5),texTankT34,
	glm::vec3(10, 0, 0),glm::vec3(1),0.0f,glm::vec3(0),playerTank }));
	//T-34��ԂɏՓ˔��������
	actors.back()->collider = BOX{ glm::vec3(-1.5f,0,-1.5f),glm::vec3(1.5f,2.5f,1.5) };


	//�����̃p�����[�^
	actors.push_back(std::shared_ptr<Actor>(new Actor{ "BrickHouse",primitiveBuffer.Get(6),texBrickHouse,
	glm::vec3(-1, 0, -8),glm::vec3(3),0.0f,glm::vec3(-2.6f, 2.0f, 0.8f) }));
	actors.back()->collider = BOX{ glm::vec3(-3.0f,0,-2.0f),glm::vec3(3.0f,2.5f,2.0f) };
	actors.back()->isStatic = true;



	//�o�ߎ��Ԃ����ɂ���
	double loopTime = glfwGetTime();			//1/60�b�Ԋu�Ń��[�v�������邽�߂̎����ϐ�
	double diffLoopTime = 0;					//�����̍���
	const float deltaTime = 1.0f / 60.0f;		//�X�V�Ԋu
	int oldShotButton = 0;						//�O��̃V���b�g�{�^���̏��
	glm::vec3 cameraPosition = glm::vec3(0, 20, 20);	//�J�����̍��W	
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);		//�J�����̒����_�̍��W

	//���C�����[�v
	while (!glfwWindowShouldClose(window)) {
		//���ݎ������擾
		const double curLoopTime = glfwGetTime();
		//���ݎ����ƑO�񎞍��̍����A�����̍����ɉ��Z
		diffLoopTime += curLoopTime - loopTime;
		//�O�񎞍������ݎ����ɍX�V
		loopTime = curLoopTime;
		//�����̍�����1/60�b�����Ȃ�A���[�v�̐擪�ɖ߂�
		if (diffLoopTime < deltaTime) {
			continue;
		}

		//
		//�Q�[����Ԃ��X�V����
		//

		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime) {
			//��Ԃ��ړ�������
			Actor* tank = Find(actors, "Tiger-I");
			if (tank) {
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
					tank->rotation += glm::radians(90.0f) * deltaTime;
				}
				else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
					tank->rotation -= glm::radians(90.0f) * deltaTime;
				}

				//tank.rotation��0�̎��̐�Ԃ̌����x�N�g��
				glm::vec3 tankFront(0, 0, 1);
				//tank.rotation���W�A��������]�������]�s������
				const glm::mat4 matRot = glm::rotate(glm::mat4(1), tank->rotation, glm::vec3(0, 1, 0));
				//�����x�N�g����rotTank������]������
				tankFront = matRot * glm::vec4(tankFront, 1);

				float tankAccel = 0.2f;	//��Ԃ̉����x
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
					tank->velocity += tankFront * tankAccel;
				}
				else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
					tank->velocity -= tankFront * tankAccel;
				}

				//�}�E�X���{�^���̏�Ԃ��擾����
				int shotButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

				//�}�E�X���{�^����������Ă�����e�A�N�^�[��ǉ�����
				if (shotButton == GLFW_PRESS && oldShotButton != GLFW_PRESS) {

					//���ˈʒu��C�̐�[�ɐݒ�
					glm::vec3 position = tank->position + tankFront * 6.0f;
					position.y += 2.0f;

					std::shared_ptr<Actor> bullet(new Actor{ "Bullet",primitiveBuffer.Get(7),texBullet,
					position,glm::vec3(0.25f),tank->rotation,glm::vec3(0) });

					//1.5�b��ɒe������
					bullet->lifespan = 1.5f;

					//��Ԃ̌����Ă�����ɁA30m/s�̑��x�ňړ�������
					bullet->velocity = tankFront * 30.0f;

					//�e�ɏՓ˔����t����
					bullet->collider = BOX{ glm::vec3(-0.25f),glm::vec3(0.25f) };


					actors.push_back(bullet);
				}

				//�u�O��̃V���b�g�{�^���̏�ԁv���X�V����
				oldShotButton = shotButton;
			}
		
			//�A�N�^�[�̏�Ԃ��X�V����
			for (int i = 0; i < actors.size(); ++i) {
				//�A�N�^�[�̎��������炷
				if (actors[i]->lifespan > 0) {
					actors[i]->lifespan -= deltaTime;

					//�����̐s�����A�N�^�[���u�폜�҂��v��Ԃɂ���
					if (actors[i]->lifespan <= 0) {
						actors[i]->isDead = true;
						continue;	//�폜�҂��A�N�^�[�͍X�V���X�L�b�v
					}
				}

				actors[i]->OnUpdate(deltaTime);

			
				//���x�ɏd�͉����x��������
				if (!actors[i]->isStatic) {
					actors[i]->velocity.y += -9.8f * deltaTime;
				}

				//�A�N�^�[�̈ʒu���X�V����
				actors[i]->position += actors[i]->velocity * deltaTime;
			}

			//�Փˏ��̔z���p�ӂ���
			std::vector<Contact> contacts;
			contacts.reserve(actors.size());

			//�A�N�^�[�̏Փ˔�����s��
			for (int a = 0; a < actors.size(); ++a) {
				for (int b = a + 1; b < actors.size(); ++b) {
					//�폜�҂��A�N�^�[�͏Փ˂��Ȃ�
					if (actors[a]->isDead) { break; }
					else if (actors[b]->isDead) { continue; }

					Contact contact;

					if (DetectCollision(*actors[a], *actors[b],contact)) {
						//�z��̒��ɁA�쐬�����R���^�N�g�\���̂Ǝ��Ă�����̂����邩���ׂ�
						auto itr = std::find_if(contacts.begin(), contacts.end(),
							[&contact](const Contact& c) {return Equal(contact, c); });

						//���Ă���R���^�N�g�\���̂�������Ȃ���΁A�쐬�����\���̂�z��ɒǉ�����
						if (itr == contacts.end()) {
							contacts.push_back(contact);
						}
						else {
							//���Ă���\���̂��������ďꍇ�A�Z�������������ق����c��
							if (contact.penLength > itr->penLength) {
								*itr = contact;
							}
						}

					}
				}
			}

			//�d�Ȃ����������
			for (int i = 0; i < contacts.size(); ++i) {
				Contact& c = contacts[i];

				//�Փ˕������t�ɂ����Փˏ������
				Contact contactBtoA;
				contactBtoA.a = c.b;
				contactBtoA.b = c.a;
				contactBtoA.velocityA = c.velocityB;
				contactBtoA.velocityB = c.velocityA;
				contactBtoA.penetration = -c.penetration;
				contactBtoA.normal = -c.normal;
				contactBtoA.position = c.position;
				contactBtoA.penLength = c.penLength;

				//�Փˏ����֐����Ăяo��
				c.a->OnCollision(c);
				c.b->OnCollision(contactBtoA);

				SolveContact(c);
			}

			//�폜�҂��̃A�N�^�[���폜����
			actors.erase(
				std::remove_if(
					actors.begin(), actors.end(), 
					[](const std::shared_ptr<Actor>& a) {return a->isDead; }),
				actors.end());

			//�J�����f�[�^���X�V����
			{
				Actor* target = Find(actors, "Tiger-I");
				if (target) {
					cameraPosition = target->position + glm::vec3(0, 20, 20);
					cameraTarget = target->position;
				}
			}
		}
		//
		//�Q�[����Ԃ�`�悷��
		//

		glEnable(GL_DEPTH_TEST);			//�[�x�o�b�t�@��L���ɂ���D
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

		//�v���_�N�V�����s����쐬�D
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		const glm::mat4 matProj =
			glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 200.0f);

		//�r���[�s����쐬�D
		const glm::mat4 matView =
			glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0, 3, 0));

		//�s����V�F�[�_�ɓ]������.
		const glm::mat4 matModel = glm::mat4(1);
		const glm::mat4 matMVP = matProj * matView * matModel;
		pipeline.SetUniform(locMatTRS, matMVP);
		pipeline.SetUniform(locMatModel, matModel);


		//�A�N�^�[��`�悷��
		for (int i = 0; i < actors.size(); ++i)
		{
			Draw(*actors[i], pipeline, matProj, matView);
		}

		// �e�N�X�`���̊��蓖�Ă�����.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		sampler->Unbind(0);
		pipeline.Unbind();
		primitiveBuffer.BindVertexArray();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFW�̏I��
	glfwTerminate();

	return 0;
}