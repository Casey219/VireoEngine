#include "virpch.h"
#include "Entity.h"

namespace Vireo {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}