#pragma once
#include<glm/glm.hpp>
#include<assimp/Quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/gtc/quaternion.hpp>


class AHelper
{
public:
	

	static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static  float* GetGLMVec(const aiVector3D& vec)
	{
		float out[3];
		out[0]=vec.x;
		out[1]=vec.y;
		out[2]=vec.z;
		return out;
	}
	static inline glm::vec3 GetGLMVec2(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}

	static inline glm::quat assimpToGlmQuat(aiQuaternion quat) {
		glm::quat q;
		q.x = quat.x;
		q.y = quat.y;
		q.z = quat.z;
		q.w = quat.w;

		return q;
	}

	static inline glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat) {
		glm::mat4 m;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m[x][y] = mat[y][x];
			}
		}
		return m;
	}

	static glm::mat4  InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
	{
		return glm::mat4(glm::vec4(ScaleX,0,0,0),
					
					glm::vec4(0,ScaleY,0,0), 
		
					glm::vec4(0,0,ScaleZ,0),
		
					glm::vec4(0,0,0,1.0f));

		
	}

	static glm::mat4 InitTranslationTransform(float x, float y, float z)
	{


		return glm::mat4(glm::vec4(1, 0, 0, x),

			glm::vec4(0, 1, 0, y),

			glm::vec4(0, 0, 1, z),

			glm::vec4(0, 0, 0, 1.0f));
		
	}
	static glm::mat4 Initmat3x3Asim(const aiMatrix3x3& AssimpMatrix) {
		return glm::mat4(glm::vec4(AssimpMatrix.a1, AssimpMatrix.a2, AssimpMatrix.a3, 0),

			glm::vec4(AssimpMatrix.b1, AssimpMatrix.b2, AssimpMatrix.b3, 0),

			glm::vec4(AssimpMatrix.c1, AssimpMatrix.c2, AssimpMatrix.c3, 0),

			glm::vec4(0, 0, 0, 1.0f));

	}
};
