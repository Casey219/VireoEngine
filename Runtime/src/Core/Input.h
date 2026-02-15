#pragma once

#include"Core/Base.h"
#include <utility>
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <glm/glm.hpp>
namespace Vireo {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
