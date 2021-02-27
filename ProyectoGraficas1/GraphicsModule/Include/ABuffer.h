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


class ABuffer
{
public:

	ABuffer()=default;
	~ABuffer(){
		Release();
		delete this;
	};
#if defined(DX11)
	 ID3D11Buffer*& getBufferDX11(){
	 return m_buffer;
	 };
	 void setBufferDX11(ID3D11Buffer* buffer) {
		 buffer=m_buffer;
	 }
	 void Release(){
	 m_buffer->Release();
	 };
#endif

private:
#if defined(DX11)
	ID3D11Buffer * m_buffer=NULL;
#endif
};

