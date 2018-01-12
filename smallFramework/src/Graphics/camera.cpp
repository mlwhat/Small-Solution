#include "camera.h"

CCamera::CCamera()
{
	reset();
};

void CCamera::reset() 
{
	vec3 camLookat = vec3(0.0f);
	vec3 camView = vec3(0.0f, 0.0f, 4.0f);
	vec3 camProj = vec3(30.0f, 0.01f, 10000.0f);

	m_lookX = camLookat[0];
	m_lookY = camLookat[1];
	m_lookZ = camLookat[2];

	m_head = camView[0];
	m_pitch = camView[1];
	m_radius = camView[2];

	m_fov = camProj[0];
	m_nearPlane = camProj[1];
	m_farPlane = camProj[2];

	m_focal.x = m_lookX;
	m_focal.y = m_lookY;
	m_focal.z = m_lookZ;
}