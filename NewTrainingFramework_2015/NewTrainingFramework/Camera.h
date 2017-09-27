#pragma once
#include "../Utilities/utilities.h"
class Camera
{
	Vector4 position;
	Vector4 target;
	Vector4 up;

	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat near_;
	GLfloat far_;
	GLfloat fov;
	GLfloat deltaTime;

	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix projectionMatrix;

public:
	Camera();
	~Camera();

	Camera(const Camera & cam);

	void setPosition(Vector4);
	void setTarget(Vector4);
	void setUp(Vector4);

	void setMoveSpeed(GLfloat);
	void setRotateSpeed(GLfloat);
	void setNear_(GLfloat);
	void setFar_(GLfloat);
	void setFov(GLfloat);
	void setDeltaTime(GLfloat);

	void setXAxis(Vector3);
	void setYAxis(Vector3);
	void setZAxis(Vector3);

	void setViewMatrix(Matrix);
	void setWorldMatrix(Matrix);


	Vector4 getPosition();
	Vector4 getTarget();
	Vector4 getUp();

	GLfloat getMoveSpeed();
	GLfloat getRotateSpeed();
	GLfloat getNear_();
	GLfloat getFar_();
	GLfloat getFov();
	GLfloat getDeltaTime();

	Vector3 getXAxis();
	Vector3 getYAxis();
	Vector3 getZAxis();

	Matrix & getViewMatrix();
	Matrix & getWorldMatrix();
	Matrix & getProjectionMatrix();

	void moveOz(GLfloat);
	void moveOy(GLfloat);
	void moveOx(GLfloat);

	void rotateOx(GLfloat);
	void rotateOy(GLfloat);
	void rotateOz(GLfloat);

	void updateWorldView();

};

