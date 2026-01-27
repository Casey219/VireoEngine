#include<Vireo.h>
#include<glm/glm.hpp>
#include <imgui.h>
#include <Events/KeyEvent.h>
#include <glm/ext/matrix_transform.hpp>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Vireo::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Vireo::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Vireo::Ref<Vireo::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Vireo::VertexBuffer::Create(vertices, sizeof(vertices)));
		Vireo::BufferLayout layout = {
			{ Vireo::ShaderDataType::Float3, "a_Position" },
			{ Vireo::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Vireo::Ref<Vireo::IndexBuffer> indexBuffer;
		indexBuffer.reset(Vireo::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Vireo::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};



		Vireo::Ref<Vireo::VertexBuffer> squareVB;
		squareVB.reset(Vireo::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Vireo::ShaderDataType::Float3, "a_Position" },
			{ Vireo::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Vireo::Ref<Vireo::IndexBuffer> squareIB;
		squareIB.reset(Vireo::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform* vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Vireo::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform* vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 u_Color;

			void main()
			{
				//color = vec4(0.2, 0.3, 0.8, 1.0);
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Vireo::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Vireo::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));


		m_Texture = Vireo::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Vireo::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);


	}

	void OnUpdate(Vireo::Timestep ts) override
	{
		if (Vireo::Input::IsKeyPressed(VIR_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Vireo::Input::IsKeyPressed(VIR_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Vireo::Input::IsKeyPressed(VIR_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Vireo::Input::IsKeyPressed(VIR_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Vireo::Input::IsKeyPressed(VIR_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Vireo::Input::IsKeyPressed(VIR_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Vireo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vireo::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Vireo::Renderer::BeginScene(m_Camera);


		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.2f, y * 0.2f, 0.0f);
				 //glm::mat4 transform = glm::translate(scale,pos) ;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Vireo::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}


		m_Texture->Bind();
		Vireo::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Vireo::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Hazel::Renderer::Submit(m_Shader, m_VertexArray);



		Vireo::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

	}

	void OnEvent(Vireo::Event& event) override
	{
	}
private:
	Vireo::Ref<Vireo::Shader> m_Shader;
	Vireo::Ref<Vireo::VertexArray> m_VertexArray;

	Vireo::Ref<Vireo::Shader> m_FlatColorShader,m_TextureShader;
	Vireo::Ref<Vireo::VertexArray> m_SquareVA;
	Vireo::Ref<Vireo::Texture2D> m_Texture,m_ChernoLogoTexture;

	Vireo::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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