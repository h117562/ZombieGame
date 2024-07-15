////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;

}



PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z)
{
	ConfineCheck();

	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 2.0f;
		if(m_forwardSpeed > (m_frameTime * 30.0f))
		{
			m_forwardSpeed = m_frameTime * 30.0f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 2.0f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 2.0f;

		if(m_backwardSpeed > (m_frameTime * 30.0f))
		{
			m_backwardSpeed = m_frameTime * 30.0f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 2.0f;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	
	return;
}


void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 1.5f;

		if(m_upwardSpeed > (m_frameTime * 15.0f))
		{
			m_upwardSpeed = m_frameTime * 15.0f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.5f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}


void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 1.5f;

		if(m_downwardSpeed > (m_frameTime * 15.0f))
		{
			m_downwardSpeed = m_frameTime * 15.0f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.5f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}


void PositionClass::MoveLeft(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_leftwardSpeed += m_frameTime * 2;

		if (m_leftwardSpeed > (m_frameTime * 30.0f))
		{
			m_leftwardSpeed = m_frameTime * 30.0f;
		}
	}
	else
	{
		m_leftwardSpeed -= m_frameTime * 2;

		if (m_leftwardSpeed < 0.0f)
		{
			m_leftwardSpeed = 0.0f;
		}
	}


	radians = m_rotationY * 0.0174532925f;

	m_positionX -= cosf(radians) * m_leftwardSpeed;
	m_positionZ += sinf(radians) * m_leftwardSpeed;


	return;

}

void PositionClass::MoveRight(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_RightwardSpeed += m_frameTime * 2;

		if (m_RightwardSpeed > (m_frameTime * 30.0f))
		{
			m_RightwardSpeed = m_frameTime * 30.0f;
		}
	}
	else
	{
		m_RightwardSpeed -= m_frameTime * 2;

		if (m_RightwardSpeed < 0.0f)
		{
			m_RightwardSpeed = 0.0f;
		}
	}

	radians = m_rotationY * 0.0174532925f;

	m_positionX += cosf(radians) * m_RightwardSpeed;
	m_positionZ -= sinf(radians) * m_RightwardSpeed;

}



void PositionClass::SpaceMove(float x, float y) 
{
	m_rotationX += y;
	m_rotationY += x;

	
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}else if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}
	
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}else if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}
	
}

void PositionClass::ConfineCheck()
{

	if (m_positionX < -40)
	{
		m_positionX = -40;
	}
	else if (m_positionX > 135)
	{
		m_positionX = 135;
	}

	if (m_positionZ < -60)
	{
		m_positionZ = -60;
	}
	else if (m_positionZ > 0)
	{
		m_positionZ = 0;
	}

}