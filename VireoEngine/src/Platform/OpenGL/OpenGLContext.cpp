#include<virpch.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<Logger.h>
#include <Core.h>
#include "OpenGLContext.h"


namespace Vireo {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VIR_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VIR_CORE_ASSERT(status, "Failed to initialize Glad!");

		VIR_CORE_INFO("OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		VIR_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}