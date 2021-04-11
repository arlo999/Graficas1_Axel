#pragma once
#include <FreeImage/Dist/x64/FreeImage.h>
#include <GLFW/glfw3.h>
#include <map>
class ATextura
{
public:

	ATextura();
	~ATextura();
	bool LoadTexture(const char* filename,	//where to load the file from
		const unsigned int texID,			//arbitrary id you will reference the texture by
											//does not have to be generated with glGenTextures
		GLenum image_format = GL_RGB,		//format the image is in
		GLint internal_format = GL_RGB,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);

	//set the current texture
	unsigned int  BindTexture(const unsigned int texID);
	//free all texture memory
	void UnloadAllTextures();
	bool UnloadTexture(const unsigned int texID);

	std::map<unsigned int, GLuint> m_texID;
};

