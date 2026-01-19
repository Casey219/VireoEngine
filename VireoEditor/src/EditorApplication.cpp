#include<Vireo.h>
#include<glm/glm.hpp>
#include <imgui.h>
#include <Events/KeyEvent.h>
class ExampleLayer :public Vireo::Layer {
public:
	ExampleLayer():Layer("example") {

	}
	~ExampleLayer() override
	{
		VIR_INFO("ExampleLayer DESTRUCTED");
	}
	void OnUpdate() override
	{
		//VIR_INFO("exeample layer is OnUpdate()");
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();*/
	}


	void OnEvent(Vireo::Event& event) override
	{
		//VIR_INFO(e.ToString());
		if (event.GetEventType() == Vireo::EventType::KeyPressed)
		{
			Vireo::KeyPressedEvent& e = (Vireo::KeyPressedEvent&)event;
			if (e.GetKeyCode() == VIR_KEY_TAB)
				VIR_TRACE("Tab key is pressed (event)!");
			VIR_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class EditorApplication :public Vireo::Application {
public:
	EditorApplication() {
		PushLayer(new ExampleLayer());
	}
	~EditorApplication() {

	}

};


Vireo::Application* Vireo::CreateApplication() {
	return new EditorApplication();
}