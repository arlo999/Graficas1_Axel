#pragma once
#if defined(OGL)

#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class AShader
{
public:
	unsigned int ID;
	AShader(const char* vertexPath, const char* fragmentPath);
	
	// activate the shader
  // ------------------------------------------------------------------------
	void Use()
	{
#if defined(OGL)
		glUseProgram(ID);
	#endif
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
#if defined(OGL)
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
#endif
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
#if defined(OGL)
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
#endif
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
#if defined(OGL)
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	#endif
	}
#if defined(OGL)
	void setMat4(const std::string& name,glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(value) );
	}
#endif
	void setFloat4(const std::string& name, float* value) const
	{
#if defined(OGL)
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
#endif
	}
private:
		void CheckCompileErrors(unsigned int shader, std::string type);
};

