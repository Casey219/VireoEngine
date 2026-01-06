#include<Vireo.h>

class EditorApplication :public Vireo::Application {
public:
	EditorApplication() {

	}
	~EditorApplication() {

	}

};


Vireo::Application* Vireo::CreateApplication() {
	return new EditorApplication();
}