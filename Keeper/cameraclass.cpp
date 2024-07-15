////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render(bool third)
{
	if (!third)
	{
		XMFLOAT3 up, position, lookAt;
		XMVECTOR upVector, positionVector, lookAtVector;
		float yaw, pitch, roll;
		XMMATRIX rotationMatrix;


		// Setup the vector that points upwards.
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		// Load it into a XMVECTOR structure.
		upVector = XMLoadFloat3(&up);

		// Setup the position of the camera in the world.
		position.x = m_positionX;
		position.y = m_positionY+13;
		position.z = m_positionZ;

		// Load it into a XMVECTOR structure.
		positionVector = XMLoadFloat3(&position);

		// Setup where the camera is looking by default.
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		// Load it into a XMVECTOR structure.
		lookAtVector = XMLoadFloat3(&lookAt);

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		pitch = m_rotationX * 0.0174532925f;
		yaw = m_rotationY * 0.0174532925f;
		roll = m_rotationZ * 0.0174532925f;

		// Create the rotation matrix from the yaw, pitch, and roll values.
		rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
		upVector = XMVector3TransformCoord(upVector, rotationMatrix);

		// Translate the rotated camera position to the location of the viewer.
		lookAtVector = XMVectorAdd(positionVector, lookAtVector);

		// Finally create the view matrix from the three updated vectors.
		m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	}
	else
	{


	XMVECTOR up, eye, lookAt, position;
	

	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	eye = XMVectorSet(10 * sin(t_X) * cos(t_Y), 10 * sin(t_Y), 10 * cos(t_X) * cos(t_Y), 1.0f);
	lookAt = XMVectorSet(0, 0, 0, 0.0f);
	position = XMVectorSet(m_positionX, m_positionY+13, m_positionZ, 1.0f);
	
	m_viewMatrix = XMMatrixLookAtLH(eye+ position, lookAt+ position, up);

	}

	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void CameraClass::RenderBaseViewMatrix()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_baseViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void CameraClass::GetBaseViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_baseViewMatrix;
	return;
}


void CameraClass::SetRound(float x, float y)
{

	t_X += x  * 3.14159 / 180;
	t_Y += y  * 3.14159 / 180;
	
	if (t_Y > (3.14f / 2))
	{
		t_Y = 3.14f / 2;
	}

	if (t_Y < (-3.14f / 2))
	{
		t_Y =- 3.14f / 2;
	}

	return;
}