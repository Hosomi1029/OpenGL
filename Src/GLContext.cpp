/**
* @file GLContext.cpp
*/

#include "GLContext.h"
#include <glm/glm.hpp>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

/**
* OpenGL�R���e�L�X�g�Ɋւ���@�\���i�[���閼�O���.
*/

namespace GLContext {

	/**
	* �o�b�t�@�I�u�W�F�N�g���쐬����.
	* 
	* @param size �f�[�^�̃T�C�Y.
	* @param data �f�[�^�ւ̃|�C���^.
	* 
	* @return �쐬�����o�b�t�@�I�u�W�F�N�g.
	*/

	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/**
	*Vertex Arrey Object���쐬����.
	* 
	* @param vboPosition VAO�Ɋ֘A�t��������W�f�[�^.
	* @param vboColor    VAO�Ɋ֘A�t������J���[�f�[�^.
	* @param vboTexcoord VAO�Ɋ֘A�t������e�N�X�`�����W�f�[�^�D
	* @param ibo		 VAO�Ɋ֘A�t������C���f�b�N�X�f�[�^.
	* 
	* @return �쐬����VAO.
	*/

	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor,GLuint vboTexcoord,GLuint vboNormal, GLuint ibo)
	{
		if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal || !ibo) {
			std::cerr << "[�G���[]" << __func__ << ":�o�b�t�@�I�u�W�F�N�g��0�ł��B\n";
			return 0;
		}

		GLuint id = 0;
		glCreateVertexArrays(1, &id);

		const GLuint positionIndex = 0;
		const GLuint positionBindingIndex = 0;
		glEnableVertexArrayAttrib(id, positionIndex);
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, positionIndex, positionBindingIndex);
		glVertexArrayVertexBuffer(
			id, positionBindingIndex, vboPosition, 0, sizeof(Position));

		const GLuint colorIndex = 1;
		const GLuint colorBindingIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id,colorIndex, colorBindingIndex);
		glVertexArrayVertexBuffer(id, colorBindingIndex, vboColor, 0, sizeof(Color));

		//�e�N�X�`�����W�f�[�^���o�C���f�B���O�|�C���g�Ɋ��蓖�Ă�D
		const GLuint texcoordIndex = 2;
		const GLuint texcoordBindingIndex = 2;
		glEnableVertexArrayAttrib(id, texcoordIndex);
		glVertexArrayAttribFormat(id, texcoordIndex, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, texcoordIndex, texcoordBindingIndex);
		glVertexArrayVertexBuffer(id, texcoordBindingIndex, vboTexcoord, 0, sizeof(glm::vec2));

		//�@���f�[�^���o�C���f�B���O�|�C���g�Ɋ��蓖�Ă�D
		const GLuint normalIndex = 3;
		const GLuint normalBindingIndex = 3;
		glEnableVertexArrayAttrib(id, normalIndex);
		glVertexArrayAttribFormat(id, normalIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, normalIndex, normalBindingIndex);
		glVertexArrayVertexBuffer(id, normalBindingIndex, vboNormal, 0, sizeof(glm::vec3));


		glVertexArrayElementBuffer(id, ibo);

		return id;
	}


	/**
	* �V�F�[�_�[�E�v���O�������r���h����.
	* 
	* @param type �V�F�[�_�[�̎��.
	* @param code �V�F�[�_�[�E�v���O�����ւ̃|�C���^.
	* 
	* @retval 0���傫���@�쐬�����v���O�����E�I�u�W�F�N�g.
	* @retval 0			�@ �v���O�����E�I�u�W�F�N�g�̍쐬�Ɏ��s.
	*/
	GLuint CreateProgram(GLenum type, const GLchar* code)
	{
		GLuint program = glCreateShaderProgramv(type, 1, &code);

		GLint status = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				std::vector<char> buf;
				buf.resize(infoLen);
				if ((int)buf.size() >= infoLen) {
					glGetProgramInfoLog(program, infoLen, nullptr, buf.data());
					std::cerr << "[�G���[]" << __func__ <<
						":�V�F�[�_�[�̃r���h�Ɏ��s.\n" << buf.data() << "\n";
				}
			}
			glDeleteProgram(program);
			return 0;
		}
		return program;
	}

	/**
	* �t�@�C������V�F�[�_�[�E�v���O�������쐬����D
	* 
	* @param type		�V�F�[�_�[�̎�ށD
	* @param filename	�V�F�[�_�[�t�@�C�����D
	* 
	* @retval 0���傫���@	�쐬�����v���O�����E�I�u�W�F�N�g�D
	* @retval 0				�v���O�����E�I�u�W�F�N�g�̍쐬�Ɏ��s�D
	*/
	GLuint CreateProgramFromFile(GLenum type, const char* filename)
	{
		std::ifstream ifs(filename);
		if (!ifs) {
			std::cerr << "[�G���[]" << __func__ << ":" << filename <<
				"���J���܂���D\n";
			return 0;
		}
		std::stringstream ss;
		ss << ifs.rdbuf();
		return GLContext::CreateProgram(type, ss.str().c_str());
	}


	/**
	* �p�C�v���C���E�I�u�W�F�N�g���쐬����.
	* 
	* @param vp		���_�V�F�[�_�[�E�v���O����.
	* @param fp		�t���O�����g�V�F�[�_�[�E�v���O����.
	* 
	* @retval 0���傫���@�@�쐬�����p�C�v���C���E�I�u�W�F�N�g.
	* @retval 0				 �p�C�v���C���E�I�u�W�F�N�g�̍쐬�Ɏ��s.
	*/
	GLuint CreatePipeline(GLuint vp, GLuint fp)
	{
		glGetError();//�G���[��Ԃ����Z�b�g.

		GLuint id;
		glCreateProgramPipelines(1, &id);
		glUseProgramStages(id, GL_VERTEX_SHADER_BIT, vp);
		glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, fp);
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "[�G���[]" << __func__ << ":�v���O�����p�C�v���C���̍쐬�Ɏ��s.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}

		GLint testVp = 0;
		glGetProgramPipelineiv(id, GL_VERTEX_SHADER, &testVp);
		if (testVp != vp) {
			std::cerr << "[�G���[]" << __func__ << ":���_�V�F�[�_�̐ݒ�Ɏ��s.\n";
			glDeleteProgramPipelines(1, &id);
			return 0;
		}
		GLint testFp = 0;
		glGetProgramPipelineiv(id, GL_FRAGMENT_SHADER, &testFp);
		if(testFp != fp){
			std::cerr << "[�G���[]" << __func__ << ":�t���O�����g�V�F�[�_�̐ݒ�Ɏ��s.\n";
		glDeleteProgramPipelines(1, &id);
		return 0;
		}


		return id;

	}


	/**
	* �T���v���E�I�u�W�F�N�g���쐬����
	* 
	* @param wrapMode ���b�v���[�h�D
	* 
	* @retval 0�ȊO�@�쐬�����T���v���E�I�u�W�F�N�g�D
	* @retval 0		 �T���v���E�I�u�W�F�N�g�̍쐬�Ɏ��s�D
	*/
	GLuint CreateSampler(GLenum wrapMode)
	{
		glGetError();//�G���[��Ԃ����Z�b�g�D

		//�T���v���E�I�u�W�F�N�g���쐬����D
		GLuint id;
		glCreateSamplers(1, &id);
		if (glGetError() != GL_NO_ERROR)
		{
			std::cerr << "[�G���[]" << __func__ << "�T���v���̍쐬�Ɏ��s\n";
			glDeleteSamplers(1, &id);
			return 0;
		}

		//���b�v�E���[�h��ݒ肷��D
		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrapMode);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrapMode);
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "[�G���[]" << __func__ << ":���b�v���[�h�ł͂Ȃ��l���w�肳�ꂽ�D";
			glDeleteSamplers(1, &id);
			return 0;
		}

		// �t�B���^��ݒ肷��.
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "[�G���[]" << __func__ << ":�t�B���^�ł͂Ȃ��l���w�肳�ꂽ.\n";
			glDeleteSamplers(1, &id);
			return 0;

		}

		return id;
	}

	/**
	* 2D�e�N�X�`�����쐬����D
	*
	* @param width		�摜�̕��i�s�N�Z�����j�D
	* @param height		�摜�̍����i�s�N�Z�����j�D
	* @param data		�摜�f�[�^�̃A�h���X�D
	* @param pixelFormat �摜�f�[�^�`���iGL_BGRA�Ȃǁj.
	* @param type		�摜�f�[�^�̌^
	*
	* @retval 0�ȊO		�쐬�����e�N�X�`���E�I�u�W�F�N�g��ID�D
	* @retval 0			�e�N�X�`���̍쐬�Ɏ��s�D
	*/

	GLuint CreateImage2D(GLsizei width, GLsizei height, const void* data,
						 GLenum pixelFormat,GLenum type)
	{
		glGetError();	//�G���[��Ԃ����Z�b�g

		//�e�N�X�`���E�I�u�W�F�N�g���쐬���AGPU���������m�ۂ���D
		GLuint id;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, GL_RGBA8, width, height);

		//���݂̃A���C�������g���L�^���Ă���A�A���C�������g���P�ɂ���B
		GLint alignment;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//GP�������Ƀf�[�^��]������D
		glTextureSubImage2D(id, 0, 0, 0, width, height, pixelFormat, type, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);//�A���C�������g�����ɖ߂�
		const GLenum result = glGetError();
		if (result != GL_NO_ERROR) {
			std::cerr << "[�G���[]" << __func__ << "�e�N�X�`���̍쐬�Ɏ��s\n";
			glDeleteTextures(1, &id);
			return 0;
		}

		//�����摜�̏ꍇ�A�iR,R,R,1�j�Ƃ��ēǂݎ����悤�ɐݒ肷��D
		if (pixelFormat == GL_RED) {
			glTextureParameteri(id, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTextureParameteri(id, GL_TEXTURE_SWIZZLE_B, GL_RED);
		}

		return id;
	}

	/**
	* �t�@�C������2D�e�N�X�`����ǂݍ��ށD
	* 
	* @param filename 2D�e�N�X�`���Ƃ��ēǂݍ��ރt�@�C�����D
	* 
	* @retval 0�ȊO�@�쐬�����e�N�X�`���E�I�u�W�F�N�g��ID�D
	* 
	*		�@0		�@�e�N�X�`���̍쐬�Ɏ��s�D
	*/
	GLuint CreateImage2D(const char* filename)
	{
		//�t�@�C�����J���D
		std::ifstream ifs;
		ifs.open(filename, std::ios_base::binary);
		if (!ifs) {
			std::cerr << "[�G���[]" << __func__ << ":'" << filename << "���J���܂���B\n";
			return 0;
		}

		//TGA�w�b�_��ǂݍ��ށD
		uint8_t tgaHeader[18];//TGA�w�b�_��18�o�C�g�D
		ifs.read(reinterpret_cast<char*>(tgaHeader), 18);

		//�C���[�WID��ǂݔ�΂��D
		ifs.ignore(tgaHeader[0]);

		//�J���[�}�b�v��ǂݔ�΂��D
		if (tgaHeader[1]) {
			const int colorMapLength = tgaHeader[5] + tgaHeader[6] * 0x100;
			const int colorMapEntrySize = tgaHeader[7];
			//�G���g���[�T�C�Y�̓r�b�g���Ȃ̂ŁA8�Ŋ����ăo�C�g���ɕϊ�����D
			const int colorMapSize = (colorMapLength * colorMapEntrySize + 7) / 8;
			ifs.ignore(colorMapSize);
		}

		//�摜�f�[�^��ǂݍ��ށD
		const int width = tgaHeader[12] + tgaHeader[13] * 0x100;
		const int height = tgaHeader[14] + tgaHeader[15] * 0x100;
		const int pixelDepth = tgaHeader[16];
		const int imageSize = width * height * pixelDepth / 8;
		std::vector<uint8_t>buf(imageSize);
		ifs.read(reinterpret_cast<char*>(buf.data()), imageSize);

		//TGA�w�b�_18�o�C�g�ڂ̑�5�r�b�g�́A�摜�f�[�^�̊i�[������\���B
		//0�̏ꍇ:�������
		//1�̏ꍇ:�ォ�牺
		//OpenGL�͉摜�f�[�^���u�������v�Ɋi�[���郋�[���ɂȂ��Ă���̂ŁA
		//TGA���u�ォ�牺�v�Ɋi�[����Ă���ꍇ�͉摜���㉺���]����B
		if (tgaHeader[17] & 0x20) {
			const size_t lineSize = width * pixelDepth / 8;
			std::vector<uint8_t> tmp(imageSize);
			std::vector<uint8_t>::iterator source = buf.begin();
			std::vector<uint8_t>::iterator destination = tmp.end();
			for (size_t i = 0; i < height; ++i) {
				destination -= lineSize;
				std::copy(source, source + lineSize, destination);
				source += lineSize;
			}
			buf.swap(tmp);
		}

		//�f�[�^�̌^��I��.
		GLenum type = GL_UNSIGNED_BYTE;
		if (tgaHeader[16] == 16) {
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		}

		//�s�N�Z���`����I�ԁD
		GLenum pixelFormat = GL_BGRA;
		if (tgaHeader[2] == 3) {//���k�Ȃ��A�����摜�D
			pixelFormat = GL_RED;
		}
		if (tgaHeader[16] == 24) {
			pixelFormat = GL_BGR;
		}

		//�ǂݍ��񂾉摜�f�[�^����e�N�X�`�����쐬����D
		return CreateImage2D(width, height, buf.data(),pixelFormat,type);
	}
}//namespace GLContext