#include<Vireo.h>
#include<glm/glm.hpp>
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
		PushOverlay(new Vireo::ImGuiLayer());
	}
	~EditorApplication() {

	}

};


Vireo::Application* Vireo::CreateApplication() {
	return new EditorApplication();
}