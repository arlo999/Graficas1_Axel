#pragma once
#include "AVector.h"
#include "AMatriz4.h"
#include <windows.h>
#include <iostream>
#if defined(DX11)
#include <xnamath.h>
#endif

class ACamera
{
public:


	ACamera();
	//get de matriz transformacion
	
	AVector getUp() { return up; }
	AVector getEye() { return eye; }
	AVector getAt() { return at; }

	
	//set matriz de transformacion
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
	//View Matrix de mano izquierda
#if defined(DX11)
	XMMATRIX getviewMLookLDirectX() { return viewMatrix; }
	
	
	//funcion que te entrega la vista De perspectiva
	
	XMMATRIX ViewPerspectiveDirectX(float fov, float aspectRatio, float cerca, float lejos);
	//Funcion que te entrega la vista Ortografica
	
	XMMATRIX ViewOrtographicDirectX(float, float, float, float);
#endif	
	//Funcion para Mover la camara 
	void move(float, float, float);

	//rotate camera
	void rotate(float , float, float);

	void setviewMLookL(AVector, AVector, AVector);

	float* getViewMLookL() { return m_viewMatrix;}

	float* ViewPerspective(float fov, float aspectRatio, float cerca, float lejos);

	float* ViewOrtographic(float, float, float, float);
private:

	//matriz de transformacion , cambiar por puntero de flotantes
#if defined(DX11)
	XMMATRIX viewMatrix;
	#endif
	float* m_viewMatrix;
	//vectores de transformacion
	
	AVector up, eye, at, w;
	//frot
	
	AVector zaxis;
	//right
	
	AVector xaxis;
	//up
	
	AVector yaxis;
	//bandera para identicar cambio de frame
	
	bool frame = false;
	
	int m_fx, m_fy;
};

