#include "viewer.h"

#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "camera.h"

using std::cout;
using std::endl;

void mouseButtonCallbackDispatcher(GLFWwindow* pWindow, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		CViewer::Instance()->mousePressed(button, mods);
	}
	else if (action == GLFW_RELEASE) {
		CViewer::Instance()->mouseReleased(button, mods);
	}
}

void cursorPosCallbackDispatcher(GLFWwindow* pWindow, double x, double y)
{
	CViewer::Instance()->mouseMoved((int)x, (int)y);
}

void scrollCallbackDispatcher(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	CViewer::Instance()->scroll(xOffset, yOffset);
}

CViewer::CViewer() : m_mouseDown(false), m_startTumbling(false), m_zoomMode(false)
{
	m_pCamera = new CCamera();

	vec3 camAdjust = vec3(0.01f);
	m_tumblingSpeed = camAdjust[0];
	m_zoomSpeed = camAdjust[1];
	m_moveSpeed = camAdjust[2];
	m_preMouseXPos = m_preMouseYPos = 0;
	m_preWheelPos = 0;
}

CViewer::~CViewer()
{
	delete m_pCamera;
}

CViewer* CViewer::Instance()
{
	static CViewer s_viewer;

	return &s_viewer;
}

void CViewer::camReset()
{
	m_pCamera->reset();
}

float CViewer::getCameraRadius()
{

		return m_pCamera->getRadius();
}

vec3 CViewer::getCameraPostion()
{
	return m_pCamera->getPosition();
}

void CViewer::aim(GLFWwindow* window, mat4* pViewMat, mat4* pProjMat)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	height = height < 1 ? 1 : height;

	// Window aspect (assumes square pixels)
	float aspectRatio = (float)width / (float)height;

	vec3 camPos(m_pCamera->m_lookX + m_pCamera->m_radius * cos(glm::radians(m_pCamera->m_head)) * sin(glm::radians(m_pCamera->m_pitch)),
		m_pCamera->m_lookY + m_pCamera->m_radius * sin(glm::radians(m_pCamera->m_head)),
		m_pCamera->m_lookZ + m_pCamera->m_radius * cos(glm::radians(m_pCamera->m_head)) * cos(glm::radians(m_pCamera->m_pitch)));

	m_forward = vec3(m_pCamera->m_lookX - camPos.x, m_pCamera->m_lookY - camPos.y, m_pCamera->m_lookZ - camPos.z);
	m_forward = glm::normalize(m_forward);
	vec3 up = vec3(0.0f, (cos(glm::radians(m_pCamera->m_head)) > 0.0f) ? 1.0f : -1.0f, 0.0f);
	m_side = glm::cross(m_forward, up);
	m_up = glm::cross(m_side, m_forward);

	*pViewMat = glm::lookAt(vec3(camPos.x, camPos.y, camPos.z),
		vec3(m_pCamera->m_lookX, m_pCamera->m_lookY, m_pCamera->m_lookZ),
		vec3(up.x, up.y, up.z));

	*pProjMat = glm::perspective(glm::radians(m_pCamera->m_fov), (float)width / height, m_pCamera->m_nearPlane, m_pCamera->m_farPlane);
}

//void CViewer::aimFromLightPos(GLFWwindow * window, mat4 * pViewMat, mat4 * pProjMat, vec3 lightPos)
//{
//	int width, height;
//	glfwGetWindowSize(window, &width, &height);
//	height = height < 1 ? 1 : height;
//
//	// Window aspect (assumes square pixels)
//	float aspectRatio = (float)width / (float)height;
//
//	vec3 camPos = lightPos;
//
//	vec3 _m_forward = vec3(m_pCamera->m_lookX - camPos.x, m_pCamera->m_lookY - camPos.y, m_pCamera->m_lookZ - camPos.z);
//	_m_forward = glm::normalize(_m_forward);
//	vec3 up = vec3(0.0f, (cos(glm::radians(m_pCamera->m_head)) > 0.0f) ? 1.0f : -1.0f, 0.0f);
//	vec3 _m_side = glm::cross(_m_forward, up);
//	vec3 _m_up = glm::cross(m_side, _m_forward);
//
//	*pViewMat = glm::lookAt(vec3(camPos.x, camPos.y, camPos.z),
//		vec3(m_pCamera->m_lookX, m_pCamera->m_lookY, m_pCamera->m_lookZ),
//		vec3(up.x, up.y, up.z));
//
//	*pProjMat = glm::perspective(glm::radians(m_pCamera->m_fov), (float)width / height, m_pCamera->m_nearPlane, m_pCamera->m_farPlane);
//}


void CViewer::aimFromLightPos(GLFWwindow * window, mat4 * pViewMat, mat4 * pProjMat, vec3 lightPos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	height = height < 1 ? 1 : height;

	// Window aspect (assumes square pixels)
	float aspectRatio = (float)width / (float)height;

	vec3 camPos = lightPos;

	vec3 _m_forward = vec3(m_pCamera->m_lookX - camPos.x, m_pCamera->m_lookY - camPos.y, m_pCamera->m_lookZ - camPos.z);
	_m_forward = glm::normalize(_m_forward);
	vec3 up = vec3(0.0f, (cos(glm::radians(m_pCamera->m_head)) > 0.0f) ? 1.0f : -1.0f, 0.0f);
	vec3 _m_side = glm::cross(_m_forward, up);
	vec3 _m_up = glm::cross(m_side, _m_forward);

	*pViewMat = glm::lookAt(vec3(camPos.x, camPos.y, camPos.z),
		vec3(m_pCamera->m_lookX, m_pCamera->m_lookY, m_pCamera->m_lookZ),
		vec3(up.x, up.y, up.z));

	*pProjMat = glm::perspective(glm::radians(m_pCamera->m_fov), (float)width / height, m_pCamera->m_nearPlane, m_pCamera->m_farPlane);
}



bool CViewer::mouseMoved(int xPos, int yPos)
{
	if (m_startTumbling) {
		m_preMouseXPos = xPos;
		m_preMouseYPos = yPos;
		m_startTumbling = false;
	}

	float dx, dy;
	dx = (float)xPos - m_preMouseXPos;
	dy = (float)yPos - m_preMouseYPos;

	if (m_mouseDown && !m_zoomMode) {
		m_pCamera->m_head += dy * m_tumblingSpeed;
		m_pCamera->m_pitch += dx * m_tumblingSpeed;
	}
	else if (m_mouseDown && m_zoomMode) {
		m_pCamera->m_lookX += (dy * m_up.x - dx * m_side.x) * m_moveSpeed;
		m_pCamera->m_lookY += (dy * m_up.y - dx * m_side.y) * m_moveSpeed;
		m_pCamera->m_lookZ += (dy * m_up.z - dx * m_side.z) * m_moveSpeed;
	}

	m_preMouseXPos = xPos;
	m_preMouseYPos = yPos;

	return true;
}

bool CViewer::mousePressed(int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_mouseDown = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_mouseDown = true;
		m_zoomMode = true;
	}

	m_startTumbling = true;
	return true;
}

bool CViewer::mouseReleased(int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_mouseDown = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_mouseDown = false;
		m_zoomMode = false;
	}

	m_mouseDown = false;

	m_startTumbling = true;

	return true;
}

bool CViewer::scroll(int xOffset, int yOffset)
{
	int vPos = m_preWheelPos + yOffset;
	float speed = (float)abs(yOffset);
	if (m_preWheelPos < vPos)
	{
		m_pCamera->m_radius += speed * m_zoomSpeed;
		setZoomSpeed(m_pCamera->m_radius * 0.01f);
		//setSpeed(m_pCamera->m_radius * 0.1f, m_pCamera->m_radius * 0.002f, m_pCamera->m_radius * 0.02f);
	}
	else
	{
		float temp = m_pCamera->m_radius - speed * m_zoomSpeed;
		if (temp > 0.0f)
			m_pCamera->m_radius = temp;
		setZoomSpeed(m_pCamera->m_radius * 0.01f);
		//setSpeed(m_pCamera->m_radius * 0.1f, m_pCamera->m_radius * 0.002f, m_pCamera->m_radius * 0.02f);
	}

	m_preWheelPos = vPos;

	//cout << m_pCamera->m_radius << endl;

	return true;
}