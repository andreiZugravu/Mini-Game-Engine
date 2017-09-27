#include "stdafx.h"
#include "Camera.h"
#include "Math.h"
#include "Globals.h"
#include <iostream>

/*------------Constructors------------*/
Camera::Camera()
{
	position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	target = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	moveSpeed = 50.0f;
	rotateSpeed = 1.0f;
	near_ = 0.2f;
	far_ = 5000.0f;
	fov = 3.14f/4.0f;
	deltaTime = 0.016f;

	updateWorldView();

	GLfloat ratio = (GLfloat) ((GLfloat) Globals::screenWidth / (GLfloat) Globals::screenHeight);
	projectionMatrix.SetPerspective(fov, ratio, near_, far_);
}

/*------------Destructor--------------*/
Camera::~Camera()
{

}

Camera::Camera(const Camera & cam)
{
	position = cam.position;
	target = cam.target;
	up = cam.up;

	moveSpeed = cam.moveSpeed;
	rotateSpeed = cam.rotateSpeed;
	near_ = cam.near_;
	far_ = cam.far_;
	fov = cam.fov;
	deltaTime = cam.deltaTime;

	updateWorldView();

	GLfloat ratio = (GLfloat)((GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight);
	projectionMatrix.SetPerspective(fov, ratio, near_, far_);
}

/*------------Setters-----------------*/
void Camera::setPosition(Vector4 newPosition)
{
	position = newPosition;
	updateWorldView();
}

void Camera::setTarget(Vector4 newTarget)
{
	target = newTarget;
	updateWorldView();
}

void Camera::setUp(Vector4 newUp)
{
	up = newUp;
	updateWorldView();
}

void Camera::setMoveSpeed(GLfloat newMoveSpeed)
{
	moveSpeed = newMoveSpeed;
}

void Camera::setRotateSpeed(GLfloat newRotateSpeed)
{
	rotateSpeed = newRotateSpeed;
}

void Camera::setNear_(GLfloat newNear_)
{
	near_ = newNear_;
}

void Camera::setFar_(GLfloat newFar_)
{
	far_ = newFar_;
}

void Camera::setFov(GLfloat newFov)
{
	fov = newFov;
}

void Camera::setDeltaTime(GLfloat newDeltaTime)
{
	deltaTime = newDeltaTime;
}

void Camera::setXAxis(Vector3 newXAxis)
{
	xAxis = newXAxis;
}

void Camera::setYAxis(Vector3 newYAxis)
{
	yAxis = newYAxis;
}

void Camera::setZAxis(Vector3 newZAxis)
{
	zAxis = newZAxis;
}

void Camera::setViewMatrix(Matrix newViewMatrix)
{
	viewMatrix = newViewMatrix;
}

void Camera::setWorldMatrix(Matrix newWorldMatrix)
{
	worldMatrix = newWorldMatrix;
}

/*------------Getters-----------------*/
Vector4 Camera::getPosition()
{
	return position;
}

Vector4 Camera::getTarget()
{
	return target;
}

Vector4 Camera::getUp()
{
	return up;
}

GLfloat Camera::getMoveSpeed()
{
	return moveSpeed;
}

GLfloat Camera::getRotateSpeed()
{
	return rotateSpeed;
}

GLfloat Camera::getNear_()
{
	return near_;
}

GLfloat Camera::getFar_()
{
	return far_;
}

GLfloat Camera::getFov()
{
	return fov;
}

GLfloat Camera::getDeltaTime()
{
	return deltaTime;
}

Vector3 Camera::getXAxis()
{
	return xAxis;
}

Vector3 Camera::getYAxis()
{
	return yAxis;
}

Vector3 Camera::getZAxis()
{
	return zAxis;
}

Matrix & Camera::getViewMatrix()
{
	return viewMatrix;
}

Matrix & Camera::getWorldMatrix()
{
	return worldMatrix;
}

Matrix & Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::moveOz(GLfloat directie)
{
	Vector4 forward = -(target - position).Normalize() * directie;
	Vector4 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOy(GLfloat directie)
{
	Vector4 forward = up.Normalize() * directie;
	Vector4 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOx(GLfloat directie)
{
	Vector4 forward = (zAxis.Cross(yAxis)).Normalize() * directie;
	Vector4 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::rotateOx(GLfloat directie)
{
	GLfloat angle = rotateSpeed * deltaTime * directie;
	Matrix mRotateOX = viewMatrix.SetRotationX(angle);

	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	up = rotatedLocalUp * worldMatrix;
	up = up.Normalize();

	Vector4	localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	target = rotatedTarget * worldMatrix;

	updateWorldView();
}

void Camera::rotateOy(GLfloat directie)
{
	GLfloat angle = rotateSpeed * deltaTime * directie;
	Matrix mRotateOY = viewMatrix.SetRotationY(angle);

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;
	target = rotatedTarget * worldMatrix;

	updateWorldView();
}

void Camera::rotateOz(GLfloat directie)
{
	GLfloat angle = rotateSpeed * deltaTime * directie;
	Matrix mRotateOz = viewMatrix.SetRotationZ(angle);
 
	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOz;

	up = rotatedLocalUp * worldMatrix;
	up = up.Normalize();

	updateWorldView();
}

void Camera::updateWorldView()
{
	Matrix R;
	Matrix T;

	zAxis = Vector3(-(target - position)).Normalize();
	yAxis = Vector3(up).Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0.0f;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0.0f;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f;	 R.m[3][1] = 0.0f;	  R.m[3][2] = 0.0f;	   R.m[3][3] = 1.0f;

	//T.SetTranslation(position.x, position.y, position.z);

	worldMatrix = R*T.SetTranslation(position.x, position.y, position.z);
	viewMatrix = T.SetTranslation(-position.x, -position.y, -position.z) * R.Transpose();

}