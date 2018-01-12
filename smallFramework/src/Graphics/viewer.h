#ifndef __VIEWER_H__
#define __VIEWER_H__

#include "glm/glm.hpp"
#include <glfw3.h>

using glm::vec3;
using glm::mat4;

class CCamera;
class CViewer
{
public:
	static CViewer* Instance();
	virtual ~CViewer();
	void camReset();
	float  getCameraRadius();

	vec3  getCameraPostion();
	void aim(GLFWwindow* window, mat4* pViewMat, mat4* pProjMat);
	void aimFromLightPos(GLFWwindow* window, mat4* pViewMat, mat4* pProjMat, vec3 lightPos);


	bool mouseMoved(int xPos, int yPos);
	bool mousePressed(int button, int mods);
	bool mouseReleased(int button, int mods);
	bool scroll(int xOffset, int yOffset);

	void setZoomSpeed(const float vSpeed){ m_zoomSpeed = vSpeed; }
	void setMoveSpeed(const float vSpeed){ m_moveSpeed = vSpeed; }
	void setTumbSpeed(const float vSpeed){ m_tumblingSpeed = vSpeed; }
	void setSpeed(const float& zoom, const float& move, const float& tumbling)
	{
		m_zoomSpeed = zoom;
		m_moveSpeed = move;
		m_tumblingSpeed = tumbling;
	}

protected:
	CViewer();
	CViewer(const CViewer&){}

	CCamera* m_pCamera;

private:
	float m_tumblingSpeed, m_zoomSpeed, m_moveSpeed;
	vec3  m_forward, m_up, m_side;

	int m_preMouseXPos;
	int m_preMouseYPos;

	int m_preWheelPos;

	bool m_mouseDown, m_startTumbling, m_zoomMode;
};

#endif // !__VIEWER_H__