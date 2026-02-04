#include<Vireo.h>
#include"Core/EntryPoint.h"
#include<glm/glm.hpp>
#include <imgui.h>
#include <Events/KeyEvent.h>
#include <glm/ext/matrix_transform.hpp>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "ExampleLayer.h"


class EditorApplication :public Vireo::Application {
public:
	EditorApplication() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~EditorApplication() {

	}

};


Vireo::Application* Vireo::CreateApplication() {
	return new EditorApplication();
}