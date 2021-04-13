#pragma once

#if defined(OGL)
#include <GLFW/glfw3.h>
#endif
#include <map>
class ATextura
{
public:

	ATextura();
	~ATextura();
#if defined(OGL)
	bool LoadTexture(const char* filename,	//where to load the file from
		const unsigned int texID,			//arbitrary id you will reference the texture by
											//does not have to be generated with glGenTextures
		GLenum image_format = GL_RGB,		//format the image is in
		GLint internal_format = GL_RGB,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);
#endif

};

