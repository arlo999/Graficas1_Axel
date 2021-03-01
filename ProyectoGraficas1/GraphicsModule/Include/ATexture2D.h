#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include "ADescriptionBuffer.h"
#endif


/**
	 * @brief   Interfaz del buffer sobre esta se generan los demas buffer
	 * @param   #float: numero a summar.
	 * @param   #float: numero a summar.
	 * @bug     si metes negativos vale gorro :C.
	 * @return  #float: el numero sumado
	 */


class ATexture2D
{
public:

	ATexture2D()=default;
	~ATexture2D()=default;
	ID3D11Texture2D*& getAtexture2D() {

	return m_texture2d;
	}
private:
	ID3D11Texture2D * m_texture2d;
};

