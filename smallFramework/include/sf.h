/*
	Created by malei@outlook.com in 2018/1/9
	Revised code from my old smallFrameWork project.
	Aimed to a simple framework project 
	External Projects:
		glm
		imgui
		GLFW
		GLEW
		Assimp
*/
#pragma once




#include "sf_common.h"

namespace smallFramework
{

	/** Sample configuration.
	*/
	struct AppConfig
	{
		uint32_t width = 1280;           ///< The width of the client area size
		uint32_t height = 768;          ///< The height of the client area size
		bool fullScreen = false;               ///< Set to true to run the sample in full-screen mode
		std::string title = "Small App Sample @ Lei Ma";    ///< Window title
		bool resizableWindow = false;          ///< Allow the user to resize the window.
		bool sync = false;
		float timeScale = 1.0f;                                     ///< A scaling factor for the time elapsed between frames.
		float fixedTimeDelta = 0.0f;                                ///< If non-zero, specifies a fixed simulation time step per frame, which is further affected by time scale.
		bool freezeTimeOnStartup = false;                           ///< Control whether or not to start the clock when the sample start running.
		bool enableVR = false;   ///< If you need VR support, set it to true to let Sample control the VR calls. Alternatively, if you want better control, you can call the VRSystem yourself
		//std::function<void(void)> deviceCreatedCallback = nullptr; ///< Callback function which will be called after the device is created
	};


	class smallApp
	{
		static void errorCallback(int error, const char* description);
	
		static void framebufferSizeChangeCallback(GLFWwindow* pGlfwWindow, int w, int h);
		
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void charCallback(GLFWwindow* window, unsigned int codepoint);
		static void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void cursorEnterCallback(GLFWwindow* window, int entered);
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void dropCallback(GLFWwindow* window, int count, const char** paths);

	public:
		smallApp();

		virtual int run(const AppConfig& config);

		void reset(uint32_t flags = 0);
		void resize(int w, int h);
		uint32_t getWidth();
		uint32_t getHeight();

		virtual void initialize(int _argc, char** _argv) {};
		virtual void update(float dt) {};

		/** Called on each frame render.
		*/
		virtual void onLoad(){};
		virtual void onFrameRender() {};
		/** Called right before the context is destroyed*/
		virtual void onGuiRender() {};
		virtual void onShutdown() {};
		/** Called every time the swap-chain is resized. You can query the default FBO for the new size and sample count of the window*/
		virtual int shutdown() { return 0; };
		

		virtual void onReset() {};
		virtual void onResize(int w, int h) {};
		virtual void onKey(int key, int scancode, int action, int mods) {}
		virtual void onChar(unsigned int codepoint) {}
		virtual void onCharMods(int codepoint, unsigned int mods) {}
		virtual void onMouseButton(int button, int action, int mods) {}
		virtual void onCursorPos(double xpos, double ypos) {}
		virtual void onCursorEnter(int entered) {}
		virtual void onScroll(double xoffset, double yoffset) {}
		virtual void onDrop(int count, const char** paths) {}

		void toggleUI(bool showUI) { mShowUI = showUI; }

	protected:
		GLFWwindow* mWindow;	
		void renderFrame();
	private:
		uint32_t mReset;
		uint32_t mWidth;
		uint32_t mHeight;
		bool  mMousePressed[3];
		float mMouseWheel;

	private:

		void imguiEvents(float dt);// only caculate the elsp time for now.
		void initUI();
		void renderGUI();
		bool mShowUI;// = true;
	};
}