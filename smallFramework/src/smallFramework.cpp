#include "ImGui/imgui_impl_glfw_gl3.h"
#include "sf.h"

// application
namespace smallFramework
{
	void smallApp::errorCallback(int error, const char* description)
	{
		// Todo...
		puts(description);
	}

	void smallApp::framebufferSizeChangeCallback(GLFWwindow* window, int w, int h)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
		{
			return;
		}

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->resize(display_w, display_h);
	}


	void smallApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS)
		{
			io.KeysDown[key] = true;
		}
		if (action == GLFW_RELEASE)
		{
			io.KeysDown[key] = false;
		}
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		if (!io.WantCaptureKeyboard)
		{
			app->onKey(key, scancode, action, mods);
		}
	}

	void smallApp::charCallback(GLFWwindow* window, unsigned int codepoint)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		if (codepoint > 0 && codepoint < 0x10000)
		{
			io.AddInputCharacter((unsigned short)codepoint);
		}
		app->onChar(codepoint);
	}

	void smallApp::charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->onCharMods(codepoint, mods);
	}

	void smallApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS && button >= 0 && button < 3)
		{
			app->mMousePressed[button] = true;
		}
		if (!io.WantCaptureMouse)
		{
			app->onMouseButton(button, action, mods);
		}
	}

	void smallApp::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->onCursorPos(xpos, ypos);
	}

	void smallApp::cursorEnterCallback(GLFWwindow* window, int entered)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->onCursorEnter(entered);
	}

	void smallApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->mMouseWheel += (float)yoffset;
		app->onScroll(xoffset, yoffset);
	}

	void smallApp::dropCallback(GLFWwindow* window, int count, const char** paths)
	{
		smallApp* app = (smallApp*)glfwGetWindowUserPointer(window);
		app->onDrop(count, paths);
	}

	void smallApp::imguiEvents(float dt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = dt;
	}

	void smallApp::resize(int w, int h)
	{
		mWidth = w;
		mHeight = h;
		onResize(w,h);
	}

	smallApp::smallApp()
	{
		mWidth = 1280;
		mHeight = 768;
		mMousePressed[0] = false;
		mMousePressed[1] = false;
		mMousePressed[2] = false;
		mMouseWheel = 0.0f;

		mShowUI = true;
	}

	void smallApp::initUI()
	{
		ImGui_ImplGlfwGL3_Init(mWindow, false);
		// Load Fonts
		// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->AddFontDefault();
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("../extra_fonts/Cousine-Regular.ttf", 15.0f);
	}
	void smallApp::renderGUI()
	{
		ImGui_ImplGlfwGL3_NewFrame();
		//if (mVideoCapture.pUI)
		//{
		//	mVideoCapture.pUI->render();
		//}
		onGuiRender();
		ImGui::Render();
	}
	void smallApp::renderFrame()
	{
		onFrameRender();
	}


	// The main entry of program
	int smallApp::run(const AppConfig& config)
	{
		// Initialize the glfw
		// http://www.glfw.org/docs/latest/window_guide.html

		if (!glfwInit())
		{
			cerr << "Failed to initialize gflw" << endl;
			exit(1);
		}
		glfwSetErrorCallback(errorCallback);
		mWidth = config.width;
		mHeight = config.height;
		
		int major_ver = 4;
		int minor_ver = 5;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);		
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		const char * tile = config.title.c_str();
		mWindow = glfwCreateWindow(mWidth, mHeight, tile, NULL, NULL);
		if (!mWindow)
		{
			glfwTerminate();
			cerr << "Failed to create gflw window" << endl;
			return -1;
		}
		glfwMakeContextCurrent(mWindow);
		glViewport(0, 0, mWidth, mHeight);
		if (glewInit() != GLEW_OK)
		{
			cerr << "Failed to initialize GLEW" << endl;
			exit(1);
		}


		// Setup input callbacks
		glfwSetWindowUserPointer(mWindow, this);
		
		glfwSetFramebufferSizeCallback(mWindow, framebufferSizeChangeCallback);
		glfwSetKeyCallback(mWindow, keyCallback);
		glfwSetCharCallback(mWindow, charCallback);
		glfwSetCharModsCallback(mWindow, charModsCallback);
		glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
		glfwSetCursorPosCallback(mWindow, cursorPosCallback);
		glfwSetCursorEnterCallback(mWindow, cursorEnterCallback);
		glfwSetScrollCallback(mWindow, scrollCallback);
		glfwSetDropCallback(mWindow, dropCallback);
		// Negelect 


		initUI();
		onLoad();

		while (glfwWindowShouldClose(mWindow) == false)
		{
			glfwPollEvents();
			glGetError();

			renderFrame(); //render gui ,real stuff render frame.

			if (mShowUI)
				renderGUI();

			glfwSwapBuffers(mWindow);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		ImGui_ImplGlfwGL3_Shutdown();
		onShutdown();
		return 1;
	}

	uint32_t smallApp::getWidth()
	{
		return mWidth;
	}

	uint32_t smallApp::getHeight()
	{
		return mHeight;
	}
}
