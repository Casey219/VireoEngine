#pragma once
#include <entt.hpp>
#include "Core/Timestep.h"
#include <string>

#include "Camera/EditorCamera.h"
#include "Core/UUID.h"
#include "Renderer/Renderer3D.h"

class b2World;
namespace Vireo
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		auto GetRegistry() { return &m_Registry; }

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();


		void AddPointLight(const glm::vec3& pos, const glm::vec3& color, float intensity);
		
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		LightSceneData m_LightData;
		Ref<UniformBuffer> m_LightUBO;
	};
}
