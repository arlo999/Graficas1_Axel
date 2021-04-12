#pragma once
#include "AVector.h"
#include "AMatriz4.h"
#include <windows.h>
#include <iostream>
#include <vector>
#if defined(OGL)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif
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


//////////////////////////////////////////
//			open gl						//
//										//
//////////////////////////////////////////
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// camera Attributes
	/*
	AVector Position;
	AVector Front;
	AVector Up;
	AVector Right;
	AVector WorldUp;
	*/
#if defined(OGL)
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

#endif	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	/*
	Camera(AVector position = AVector(0.0f, 0.0f, 0.0f), AVector up = AVector(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : 
	Front(AVector(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	*/
#if defined(OGL)
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	#endif
	// constructor with scalar values

	/*
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(AVector(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = AVector(posX, posY, posZ);
		WorldUp = AVector(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	*/

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
#if defined(OGL)
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);

	}
	#endif


	float* ViewPerspective(float fov, float aspectRatio, float cerca, float lejos)
	{

		float height = cos(fov * .5) / sin(fov * .5);
		float width = height / aspectRatio;
		return  new float[16]{
		  width, 0.0f, 0.0f, 0.0f,
		  0.0f, height, 0.0f, 0.0f,
		  0.0f, 0.0f, lejos / (lejos - cerca), 1.0f,
		  0.0f, 0.0f, (-lejos / (lejos - cerca)) * cerca, 0.0f
		};
	}
	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
#if defined(OGL)
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
			#endif
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

private:
	
	// calculates the front vector from the Camera's (updated) Euler Angles

	void updateCameraVectors()
	{
#if defined(OGL)
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		#endif
		/*
		// calculate the new Front vector
		AVector front;
		front.setX( cos((Yaw)) * cos((Pitch)) );
		front.setY( sin((Pitch)) );
		front.setZ(  sin((Yaw)) * cos((Pitch)) );
		front.normalize();
		Front = front;
		// also re-calculate the Right and Up vector
		Front.ProductoCruz(WorldUp);
		Front.normalize();

		Right =Front; // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Right.ProductoCruz(Front);
		Right.normalize();
		Up = Right;
		*/
	}
};