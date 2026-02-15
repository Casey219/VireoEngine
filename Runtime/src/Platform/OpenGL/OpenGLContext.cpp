#include<virpch.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Core/Logger.h"
#include "Core/Base.h"
#include "OpenGLContext.h"


namespace Vireo {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VIR_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		VIR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VIR_CORE_ASSERT(status, "Failed to initialize Glad!");

		VIR_CORE_INFO("OpenGL Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		VIR_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));

		VIR_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "VireoEngine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		VIR_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
