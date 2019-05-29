//
// File: Camera.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include <iostream>
#include <math.h>
#include "renderer/Camera.h"
#include "include.h"
#include "Config.h"
#include "Geometry.h"

using namespace std;

Camera * pCamera = NULL;

Camera::Camera()
{
  Reset();
  zoom = 8.0f;
}

Camera::~Camera()
{
}

void Camera::Reset(void)
{
  x = 0;
  y = 0;
  z = 0;
  anglex = 0;
  angley = 0.0f;
  anglez = 225.0f;
  blockx = 0;
  blocky = 0;
  camhaschanged = true;
}

void Camera::PlaceGLRotationMatrix(void)
{
}

void Camera::PlaceGLMatrix(void)
{
}

void Camera::Rotate(float anglex, float angley, float anglez)
{
  this->anglex += anglex;
  this->angley += angley;
  this->anglez += anglez;
  camhaschanged = true;
}

int Camera::Move(float value)
{
  int result = false;

  this->x -= sin(anglez / 180 * PI) * value;
  this->y -= cos(anglez / 180 * PI) * value;
  if(x > 8.0) {
    x -= 8.0;
    blockx--;
    result = true;
  }

  if(x < -8.0) {
    x += 8.0;
    blockx++;
    result = true;
  }

  if(y > 8.0) {
    y -= 8.0;
    blocky--;
    result = true;
  }

  if(y < -8.0) {
    y += 8.0;
    blocky++;
    result = true;
  }

  camhaschanged = true;
  return result;
}

float Camera::GetX(void)
{
  return x;
}

float Camera::GetY(void)
{
  return y;
}

float Camera::GetZ(void)
{
  return z;
}

float Camera::GetAngleX(void)
{
  return anglex;
}

float Camera::GetAngleY(void)
{
  return angley;
}

float Camera::GetAngleZ(void)
{
  return anglez;
}

int Camera::GetBlockX(void)
{
  return blockx;
}

int Camera::GetBlockY(void)
{
  return blocky;
}

void Camera::SetX(float px)
{
  x = px;
  camhaschanged = true;
}

void Camera::SetY(float py)
{
  y = py;
  camhaschanged = true;
}

void Camera::SetZ(float pz)
{
  z = pz;
  camhaschanged = true;
}

void Camera::SetAngleX(float anglex)
{
  this->anglex = anglex;
  camhaschanged = true;
}

void Camera::SetAngleY(float angley)
{
  this->angley = angley;
  camhaschanged = true;
}

void Camera::SetAngleZ(float anglez)
{
  this->anglez = anglez;
  camhaschanged = true;
}

void Camera::SetBlockX(int px)
{
  blockx = px;
  camhaschanged = true;
}

void Camera::SetBlockY(int py)
{
  blocky = py;
  camhaschanged = true;
}

int Camera::CamHasChanged(void)
{
  int result = camhaschanged;

  camhaschanged = false;
  return result;
}

void Camera::GetGamePosition(unsigned int &x, unsigned int &y, int &z)
{
  x = GetBlockX() * 8 - (int) GetX();
  y = GetBlockY() * 8 - (int) GetY();
  z = -(int) GetZ();
}

void Camera::GetGamePosition(float &x, float &y, float &z)
{
  x = GetBlockX() * 8 - GetX();
  y = GetBlockY() * 8 - GetY();
  z = -(int) GetZ();
}

void Camera::GetCameraState(struct CameraState &state)
{
  state.x = x;
  state.y = y;
  state.z = z;
  state.anglex = anglex;
  state.angley = angley;
  state.anglez = anglez;
  state.blockx = blockx;
  state.blocky = blocky;
}

void Camera::SetCameraState(struct CameraState state)
{
  if((x != state.x) || (y != state.y) || (z != state.z)
     || (anglex != state.anglex) || (angley != state.angley)
     || (anglez != state.anglez) || (x != state.x) || (y != state.y))
    camhaschanged = true;

  x = state.x;
  y = state.y;
  z = state.z;
  anglex = state.anglex;
  angley = state.angley;
  anglez = state.anglez;
  blockx = state.blockx;
  blocky = state.blocky;
}

void Camera::ChangeZoom(float value)
{
	zoom += value;
	if (zoom > 8.0f)
		zoom = 8.0f;
	if (zoom < 0.0f)
		zoom = 0.0f;
		camhaschanged  = true;
}

void Camera::CreatePickRay(int mousex, int mousey, float vecPickRayOrigin[3], float vecPickRayDir[3])
{
	float vecTemp[3];
	float projmatrix[16];
	float matrix[16];
	glMatrixMode(GL_PROJECTION_MATRIX); //
	PlaceGLMatrix();
	glGetFloatv(GL_PROJECTION_MATRIX, projmatrix);

	vecTemp[0] =  ( ( ( 2.0f * mousex ) / nConfig::width  ) - 1 ) / projmatrix[0];
	vecTemp[1] = -( ( ( 2.0f * mousey ) / nConfig::height ) - 1 ) / projmatrix[5];
	vecTemp[2] =  -1.0f;

// Matrix stuff

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	InvertMatrix(matrix, matrix);

// Transform Screenspace-PickRay into Worldspace-PickRay

	Vec3TransformCoord(vecTemp, matrix, vecPickRayDir);
	vecPickRayOrigin[0]=matrix[12];
	vecPickRayOrigin[1]=matrix[13];
	vecPickRayOrigin[2]=matrix[14];
	for(int i=0; i<3; i++) vecPickRayDir[i]-=vecPickRayOrigin[i];
	NormalizeVector(vecPickRayDir);

// PickRay Intersection with NearPlane is the Origin of the PickRay
	for(i=0; i<3; i++) vecPickRayOrigin[i]+=vecPickRayDir[i]*0.2f; // 0.2f = NEAR_Z
}

