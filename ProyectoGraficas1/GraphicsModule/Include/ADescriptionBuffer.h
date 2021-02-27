#pragma once
/**
	 * @brief  es una estructura la cual te describe el buffer
	 */
struct _BUFFER_DESC
{
	UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;
};

/**
	 * @brief   Es un Enum el cual contiene banderas de como se va utilizar el buffer
	 */
enum _USAGE
{
	_USAGE_DEFAULT = 0,
	_USAGE_IMMUTABLE = 1,
	_USAGE_DYNAMIC = 2,
	_USAGE_STAGING = 3
};


/**
	 * @brief   es un enum que nos da el tipo de bind
	 */
enum _BIND_FLAG
{
	_BIND_VERTEX_BUFFER = 0x1L,
	_BIND_INDEX_BUFFER = 0x2L,
	_BIND_CONSTANT_BUFFER = 0x4L,
	_BIND_SHADER_RESOURCE = 0x8L,
	_BIND_STREAM_OUTPUT = 0x10L,
	_BIND_RENDER_TARGET = 0x20L,
	_BIND_DEPTH_STENCIL = 0x40L,
	_BIND_UNORDERED_ACCESS = 0x80L
};
