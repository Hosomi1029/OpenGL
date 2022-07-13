/**
* @file Primitive.cpp
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Primitive.h"
#include "GLContext.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>

/**
* �f�[�^��GPU�������ɃR�s�[����D
* 
* @param writeBuffer	�R�s�[��̃o�b�t�@�I�u�W�F�N�g�D
* @param unitSize		�v�f�̃o�C�g���D
* @param offsetCount	�R�s�[��I�t�Z�b�g(�v�f�P��)�D
* @param count			�R�s�[����v�f���D
* @param data			�R�s�[����f�[�^�̃A�h���X�D
* 
* @retval true	�R�s�[�����D
* @retval false	�R�s�[���s�D
*/

bool CopyData(GLuint writeBuffer, GLsizei unitSize,
	GLsizei offsetCount, size_t count, const void* data)
{
	const GLsizei size = static_cast<GLsizei>(unitSize * count);
	const GLuint readBuffer = GLContext::CreateBuffer(size, data);
	if (!readBuffer) {
		std::cerr << "[�G���[]" << __func__ << ":�R�s�[���o�b�t�@�̍쐬�Ɏ��s(size=" <<
			size << ").\n";
		return false;
	}
	const GLsizei offset = static_cast<GLsizei>(unitSize* offsetCount);
	glCopyNamedBufferSubData(readBuffer, writeBuffer, 0, offset, size);
	glDeleteBuffers(1, &readBuffer);
	if (glGetError() != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":�f�[�^�̃R�s�[�Ɏ��s(size=" <<
			size << ",offset=" << offset << ").\n";
		return false;
	}

	return true;
}

/**
* �v���~�e�B�u��`�悷��
*/

void Primitive::Draw() const
{
	glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_SHORT, indices, baseVertex);
}

/**
* �v���~�e�B�u�p�̃��������m�ۂ���D
* 
* @param maxVertexCount �i�[�\�ȍő咸�_���D
* @param maxIndexCount	�i�[�\�ȍő�C���f�b�N�X��
*/

PrimitiveBuffer::PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount)
{
	//VBO,IBO,VAO���쐬�D
	vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboColor = GLContext::CreateBuffer(sizeof(glm::vec4) * maxVertexCount, nullptr);
	vboNormal = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec2) * maxVertexCount, nullptr);
	ibo = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
	vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, vboNormal,ibo);
	if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal|| !ibo || !vao)
	{
		std::cerr << "[�G���[]" << __func__ << ":vao�̍쐬�ɍ쐬�Ɏ��s�D\n";
	}

	//�`��f�[�^�p�̃�������1000���\��
	primitives.reserve(1000);

	//�Ǘ��\��̒��_���ƃC���f�b�N�X����ݒ�D
	this->maxVertexCount = maxVertexCount;
	this->maxIndexCount = maxIndexCount;

}

/**
* �f�X�g���N�^
*/
PrimitiveBuffer::~PrimitiveBuffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboTexcoord);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &vboPosition);
}

/**
* �`��f�[�^��ǉ�����D
* 
* @param vertexCount	�ǉ����钸�_�f�[�^�̐��D
* @param pPosition		���W�f�[�^�ւ̃|�C���^�D
* @param pColor			�F�f�[�^�ւ̃|�C���^�D
* @param pTexcoord		�e�N�X�`�����W�f�[�^�ւ̃|�C���^�D
* @param pNormal		�@���f�[�^�ւ̃|�C���^�D
* @param indexCount		�ǉ�����C���f�b�N�X�f�[�^�̐��D
* @param pIndex			�C���f�b�N�X�f�[�^�ւ̃|�C���^�D
* 
* @retval true			�ǉ��ɐ����D
* @retval false			�ǉ��Ɏ��s�D
*/

bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
	const glm::vec4* pColor, const glm::vec2* pTexcoord,const glm::vec3* pNormal,
	size_t indexCount, const GLushort* pIndex)
{
	//�G���[�`�F�b�N�D
	if (!vao) {
		std::cerr << "[�G���[]" << __func__ << ":VAO�̍쐬�Ɏ��s���Ă��܂��D\n";
		return false;
	}
	else if (vertexCount > static_cast<size_t>(maxVertexCount) - curVertexCount) {
		std::cerr << "[�x��]" << __func__ << ":VBO�����t�ł�. (max=" << maxVertexCount <<
			",cur=" << curVertexCount << ",add =" << vertexCount << ")\n";
		return false;
	}
	else if (indexCount > static_cast<size_t>(maxIndexCount) - curIndexCount) {
		std::cerr << "[�x��]" << __func__ << ":IBO�����t�ł�. (max=" << maxIndexCount <<
			",cur=" << curIndexCount << ",add =" << indexCount << ")\n";
		return false;
	}

	//GPU�������ɒ��_���W�f�[�^���R�s�[�D
	if (!CopyData(vboPosition, sizeof(glm::vec3), curVertexCount, vertexCount, pPosition))
	{
		return false;
	}

	//GPU�������ɐF�f�[�^���R�s�[�D
	if (!CopyData(vboColor, sizeof(glm::vec4), curVertexCount, vertexCount, pColor))
	{
		return false;
	}

	//GPU�������Ƀe�N�X�`�����W�f�[�^���R�s�[�D
	if (!CopyData(vboTexcoord, sizeof(glm::vec2), curVertexCount, vertexCount, pTexcoord))
	{
		return false;
	}

	//GPU�������ɖ@���f�[�^���R�s�[�D
	if (!CopyData(vboNormal, sizeof(glm::vec3), curVertexCount, vertexCount, pNormal))
	{
		return false;
	}


	//GPU�������ɃC���f�b�N�X�f�[�^���R�s�[�D
	if (!CopyData(ibo, sizeof(GLushort), curIndexCount, indexCount, pIndex))
	{
		return false;
	}

	//�`��f�[�^���쐬�D
	const Primitive prim(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
		sizeof(GLshort) * curIndexCount, curVertexCount);

	//�`��f�[�^��z��ɒǉ��D
	primitives.push_back(prim);

	//���݂̃f�[�^�����A�ǉ������f�[�^���������₷�D
	curVertexCount += static_cast<GLsizei>(vertexCount);
	curIndexCount += static_cast<GLsizei>(indexCount);


	return true;
}

/**
* OBJ�t�@�C������v���~�e�B�u��ǉ�����D
* 
* @param filename ���[�h����OBJ�t�@�C�����D
* 
* @retval true	�ǉ�����
* @retval false �ǉ����s
*/
bool PrimitiveBuffer::AddFromObjFile(const char* filename)
{
	//�t�@�C�����J��
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cerr << "[�G���[]" << __func__ << ":'" << filename << "'���J���܂���D\n";
		return false;
	}

	//�f�[�^�ǂݎ��p�̔z�������
	std::vector<glm::vec3> objPositions; //OBJ�t�@�C���̒��_���W
	std::vector<glm::vec2> objTexcoords; //OBJ�t�@�C���̃e�N�X�`�����W
	std::vector<glm::vec3> objNormals;	 //OBJ�t�@�C���̖@��
	struct Index {
		int v = 0;  //���_���W�C���f�b�N�X
		int vt = 0; //�e�N�X�`�����W�C���f�b�N�X
		int vn = 0; //�@���C���f�b�N�X
	};
	std::vector<Index> objIndices;		//OBJ�t�@�C���̃C���f�b�N�X

	//�z��p�̃�������\��D
	objPositions.reserve(100'000);
	objTexcoords.reserve(100'000);
	objNormals.reserve(100'000);
	objIndices.reserve(100'000);

	//�t�@�C�����烂�f���̃f�[�^��ǂݍ���
	size_t lineNo = 0; 
	while (!ifs.eof()) {
		std::string line;
		std::getline(ifs, line);		//�t�@�C������1�s�ǂݍ���
		++lineNo;

		//�s�̐擪�ɂ���󔒂�ǂݔ�΂�
		const size_t posData = line.find_first_not_of("\t");
		if (posData != std::string::npos) {
			line = line.substr(posData);
		}

		//��s�܂��̓R�����g�s�Ȃ疳�����Ď��̍s�֐i�ށD
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//�f�[�^�̎�ނ��擾�D
		const size_t posEndofType = line.find_first_of(" \t");
		const std::string type = line.substr(0, posEndofType);
		const char* p = line.c_str() + posEndofType;	//�f�[�^�������w���|�C���^
			
		//�^�C�v�ʂ̃f�[�^�ǂݍ��ݏ���
		if (type == "v") {	//���_���W
			glm::vec3 v(0);
			if (sscanf(p, "%f %f %f", &v.x, &v.y, &v.z) != 3) {
				std::cerr << "[�x��]" << __func__ << ":���_���W�̓ǂݎ��Ɏ��s�D\n" <<
					" " << filename << "(" << lineNo << "�s��" << line << "\n";
			}
			objPositions.push_back(v);
		}
		else if (type == "vt") {	//�e�N�X�`�����W
			glm::vec2 vt(0);
			if (sscanf(p, "%f %f", &vt.x, &vt.y) != 2) {
				std::cerr << "[�x��]" << __func__ << ":�e�N�X�`�����W�̓ǂݍ��݂Ɏ��s�D\n" <<
					" " << filename << "(" << lineNo << "�s��):" << line << "\n";
			}
			objTexcoords.push_back(vt);
		}
		else if (type == "vn") {	//�@��
			glm::vec3 vn(0);
			if (sscanf(p, "%f %f %f", &vn.x, &vn.y, &vn.z) != 3) {
				std::cerr << "[�x��]" << __func__ << ":�@���̓ǂݍ��݂Ɏ��s�D\n" <<
					"  " << filename << "(" << lineNo << "�s��):" << line << "\n";
			}
			objNormals.push_back(vn);
		}
		else if (type == "f") {	//��
			std::vector<Index> f;//�ʂ̒��_�f�[�^�̔z��
			for (size_t i = 0; ; ++i) {
				int readBytes = 0;	//�ǂݍ��񂾕�����
				Index tmp;
				//[���_���W/�e�N�X�`�����W/�@��]�o�[�W�����̉�́D
				if (sscanf(p, "%d/%d/%d %n",&tmp.v,&tmp.vt,&tmp.vn,&readBytes)==3){
					f.push_back(tmp);
					p += readBytes;
				}
				//[���_���W/�e�N�X�`�����W]�o�[�W�����̉�́D
				else if (sscanf(p, "%d/%d %n",& tmp.v, &tmp.vt,&readBytes) == 2) {
					f.push_back(tmp);
					p += readBytes;
				}
				else {
					break; //�ǂݍ��݊���
				}
			}

			if (f.size() >= 3)
			{
				for (size_t i = 2; i < f.size(); ++i) {
					objIndices.push_back(f[0]);//��ڂ�0�ԖڂŌŒ�
					objIndices.push_back(f[i - 1]);
					objIndices.push_back(f[i]);
				}
			}else{
				std::cerr << "[�x��]" << __func__ << ":�ʃf�[�^�̓ǂݍ��݂Ɏ��s�D\n" <<
					" " << filename << "(" << lineNo << "�s��):" << line << "\n";
			}
		}
		else {//���Ή��̏���
			std::cerr << "[�x��]" << __func__ << ":���Ή��̌`���ł��D\n" <<
				" " << filename << "(" << lineNo << "�s��):" << line << "\n";
		}


	}

	//�f�[�^�ϊ��p�̔z�������
	std::vector<glm::vec3> positions;	//OpenGL�p�̒��_���W
	std::vector<glm::vec4> colors;		//OpenGL�p�̐F
	std::vector<glm::vec2> texcoords;	//OpenGL�p�̃e�N�X�`���f�[�^
	std::vector<glm::vec3> normals;		//OpenGL�p�̖@��
	std::vector<GLushort> indices;		//OpenGL�p�̃C���f�b�N�X

	//�f�[�^�ϊ��p�̃��������m�ہD
	const size_t indexCount = objIndices.size();
	positions.reserve(indexCount);
	texcoords.reserve(indexCount);
	normals.reserve(indexCount);
	indices.reserve(indexCount);

	//OBJ�t�@�C���̃f�[�^��OpenGL�̃f�[�^�ɕϊ��D
	for (size_t i = 0; i < indexCount; ++i) {
		//�C���f�b�N�X�f�[�^��ǉ��D
		indices.push_back(static_cast<GLushort>(i));

		//���_���W��ϊ��D
		const int v = objIndices[i].v - 1;
		if (v < static_cast<int>(objPositions.size())) {
			positions.push_back(objPositions[v]);
		}
		else {
			std::cerr << "[�x��]" << __func__ << ":���_���W�C���f�b�N�X" << v <<
				"�͔͈�[0," << objPositions.size() << ")�̊O���w���Ă��܂��D\n" <<
				" " << filename << "\n";
			positions.push_back(glm::vec3(0));
		}

		//�e�N�X�`�����W��ϊ��D
		const int vt = objIndices[i].vt - 1;
		if (vt < static_cast<int>(objTexcoords.size())) {
			texcoords.push_back(objTexcoords[vt]);
		}
		else {
			std::cerr << "[�x��]" << __func__ << ":�e�N�X�`�����W�C���f�b�N�X" << vt <<
				"�͔͈�[0," << objTexcoords.size() << ")�̊O���w���Ă��܂��D\n" <<
				" " << filename << "\n";
			texcoords.push_back(glm::vec2(0));
		}

		//�@���C���f�b�N�X���Ȃ��f�[�^�̏ꍇ�A�ʂ̒��_���W����@�����v�Z����D
		if (objIndices[i].vn == 0) {
			//�ʂ̒��_���W��z�񂐂Ɏ擾�D
			glm::vec3 p[3];
			const size_t n = (i / 3);
			for (size_t j = 0; j < 3; ++j) {
				const int v = objIndices[n * 3 + j].v - 1;
				p[j] = objPositions[v];
			}

			//��a�ƕ�b���v�Z�D
			const glm::vec3 a = p[1] - p[0];
			const glm::vec3 b = p[2] - p[0];

			//a��b�ɐ����ȃx�N�g�����v�Z�D
			glm::vec3 normal = glm::cross(a, b);

			//�����ȃx�N�g���̒������v�Z�D
			const float length = sqrt(
				normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

			//�����x�N�g����P�ʃx�N�g���ɕϊ��D
			normal = normal / length;
			
			normals.push_back(normal);
		}
		else {
			//�@����ϊ��D
			const int vn = objIndices[i].vn - 1;
			if (vn < static_cast<int>(objNormals.size())) {
				normals.push_back(objNormals[vn]);
			}
			else {
				std::cerr << "[�x��]" << __func__ << ":�@���C���f�b�N�X" << vn <<
					"�͔͈�[0," << objNormals.size() << ")�̊O���w���Ă��܂��D\n" <<
					" " << filename << "\n";
				normals.push_back(glm::vec3(0, 1, 0));
			}
		}

	}

	//�F�f�[�^��ݒ�D
	colors.resize(positions.size(), glm::vec4(1));

	//�v���~�e�B�u��ǉ�����
	const bool result = Add(positions.size(), positions.data(), colors.data(),
		texcoords.data(), normals.data(), indices.size(), indices.data());
	if (result) {
		std::cout << "[���]" << __func__ << ":" << filename << "(���_��=" <<
			positions.size() << "�C���f�b�N�X��=" << indices.size() << ")\n";
	}
	else {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "�̓ǂݍ��݂Ɏ��s.\n";
	}

	return result;
}

/**
* �v���~�e�B�u���擾����D
* 
* @pram n �v���~�e�B�u�̃C���f�b�N�X�D
* 
* @return n n�ɑΉ�����v���~�e�B�u�D
*/
const Primitive& PrimitiveBuffer::Get(size_t n)const {
	if (n > static_cast<int>(primitives.size())) {
		std::cerr << "[�x��]" << __func__ << ":" << n <<
			"�͖����ȃC���f�b�N�X�ł�(size=" << primitives.size() << ").\n";
		//���̕`��f�[�^��Ԃ��D
		static const Primitive dummy;
		return dummy;
	}
	return primitives[n];
}

/**
* VAO���O���t�B�b�N�X�p�C�v���C���Ƀo�C���h����D
*/

void PrimitiveBuffer::BindVertexArray() const
{
	glBindVertexArray(vao);
}

/**
* VAO�̃o�C���h����������D
*/

void PrimitiveBuffer::UnbindvertexArray() const
{
	glBindVertexArray(0);
}
