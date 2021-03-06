/**
* @file ProgramPipeline.h
*/

#ifndef PROGTAMPIPELINE_H_INCLUDED
#define PROGTAMPIPELINE_H_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>

/**
* シェーダープログラムを管理するクラス．
*/
class ProgramPipeline
{
public:
	ProgramPipeline(const char* vsCode, const char* fsCode);
	~ProgramPipeline();

	//オブジェクトの有効性を判定する
	bool IsValid() const;

	//ユニフォーム変数の設定
	bool SetUniform(GLint, const glm::mat4&)const;

	//バインド管理
	void Bind()const;
	void Unbind()const;

private:
	GLuint vp = 0;		//プログラム・オブジェクト（頂点シェーダー）
	GLuint fp = 0;		//プログラム・オブジェクト（フラグメントシェーダー）
	GLuint pipeline = 0;//プログラム・パイプライン・オブジェクト

};



#endif //PROGTAMPIPELINE_H_INCLUDED