#include<Vireo.h>

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
		VIR_INFO("exeample layer is OnUpdate()");
	}

	void OnEvent(Vireo::Event& e) override
	{
		VIR_INFO(e.ToString());
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