/**
* @file Primitive.h
*/

#ifndef PRIMITIVE_H_INCLUDED
#define PRIMITIVE_H_INCLUDED
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

/**
* �v���~�e�B�u�f�[�^�D
*/

class Primitive {
public:
	Primitive() = default;
	Primitive(GLenum m, GLsizei c, size_t o, GLint b) :
		mode(m), count(c), indices(reinterpret_cast<GLvoid*>(o)), baseVertex(b)
	{}
	~Primitive() = default;

	void Draw() const;

private:
	GLenum mode = GL_TRIANGLES; ///<�v���~�e�B�u�̎�ށD
	GLsizei count = 0;			///<�`�悷��C���f�b�N�X���D
	const GLvoid* indices = 0;  ///<�`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g�D
	GLint baseVertex = 0;		///<�C���f�b�N�X0�ԂƂ݂Ȃ���钸�_�z����̈ʒu
};

/**
* �����̃v���~�e�B�u���Ǘ�����N���X
*/
class PrimitiveBuffer
{
public:
	PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount);
	~PrimitiveBuffer();

	//�v���~�e�B�u�̒ǉ�
	bool Add(size_t vertexCount, const glm::vec3* pPosition, const glm::vec4* pColor,
		const glm::vec2* pTexcoord, const glm::vec3* pNormal,
		size_t indexCount, const GLushort* pindex);

	bool AddFromObjFile(const char* filename);

	//�v���~�e�B�u�̎擾
	const Primitive& Get(size_t n) const;

	//VAO�o�C���h�Ǘ�
	void BindVertexArray() const;
	void UnbindvertexArray() const;

private:
	std::vector<Primitive> primitives;	//�`��f�[�^�̔z��

	//�o�b�t�@ID
	GLuint vboPosition = 0;
	GLuint vboColor = 0;
	GLuint vboNormal = 0;
	GLuint vboTexcoord = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	GLsizei maxVertexCount = 0; //�i�[�o����ő咸�_��
	GLsizei curVertexCount = 0; //�i�[�ςݒ��_��
	GLsizei maxIndexCount = 0;	//�i�[�o����ő�C���f�b�N�X��
	GLsizei curIndexCount = 0;	//�i�[�ς݃C���f�b�N�X��
};

#endif//PRIMITIVE_H_INCLUDED