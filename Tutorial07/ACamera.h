#pragma once
#include "AVector.h"
#include "AMatriz4.h"


#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource.h"

class ACamera
{
public:

	ACamera();

	AVector getUp() { return up; }
	AVector getEye() { return eye; }
	AVector getAt() { return at; }

	//
	void setUp(AVector Up) {
		up.setX(Up.getX());
		up.setY(Up.getY());
		up.setZ(Up.getZ());
		up.setM(Up.getM());
	}
	void setEye(AVector Eye) {
		eye.setX(Eye.getX());
		eye.setY(Eye.getY());
		eye.setZ(Eye.getZ());
		eye.setM(Eye.getM());
	}

	void setAt(AVector At) {
		at.setX(At.getX());
		at.setY(At.getY());
		at.setZ(At.getZ());
		at.setM(At.getM());
	}

	//
	XMMATRIX viewMLookL(AVector, AVector, AVector);
	XMMATRIX ViewPerspective(float fov, float aspectRatio, float cerca, float lejos);
	XMMATRIX ViewOrtographic(float, float, float, float);
private:
	AVector up, eye, at, resul;
	
};

