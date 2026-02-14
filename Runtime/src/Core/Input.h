#pragma once

#include"Core/Base.h"
#include <utility>
#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Vireo {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}