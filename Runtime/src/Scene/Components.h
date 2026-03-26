#pragma once
#include "Core/UUID.h"
#include "SceneCamera.h"
//#include "ScriptableEntity.h"
#include "Renderer/Texture.h"

#include<glm/glm.hpp>
#include"glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <Model/Model.h>

namespace Vireo {
	
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	//add components name
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {
		}
	};

	struct TransformComponent
	{
		
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {
		}
		glm::mat4 GetTransform() const
		{
			/*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });*/

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {
		}
	};


	// 3D 模型组件
	struct MeshRendererComponent
	{
		Ref<Model> MeshModel;
		Ref<Shader> MeshShader;
		// 可以预留材质覆盖，如果不设置则使用模型自带材质
		// Ref<Material> MaterialOverride; 

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(const Ref<Model>& model,const Ref<Shader> shader)
			: MeshModel(model),MeshShader(shader){
		}
	};

	// 点光源组件
	struct PointLightComponent
	{
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		float Radius = 10.0f; // 用于延迟渲染或衰减计算

		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(const glm::vec3& color, float intensity, float radius)
			: Color(color), Intensity(intensity), Radius(radius) {
		}
		PointLightComponent(const glm::vec3& color, float intensity)
			: Color(color), Intensity(intensity) {
		}
		PointLightComponent(const glm::vec3& color)
			: Color(color) {
		}
		
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		
	};

	// Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};


	// Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};


}
