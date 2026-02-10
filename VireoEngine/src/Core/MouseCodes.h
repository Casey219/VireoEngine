#pragma once


namespace Vireo
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// From glfw3.h
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}
}
// From glfw3.h
#define VIR_MOUSE_BUTTON_0      ::Vireo::Mouse::Button0
#define VIR_MOUSE_BUTTON_1      ::Vireo::Mouse::Button1
#define VIR_MOUSE_BUTTON_2      ::Vireo::Mouse::Button2
#define VIR_MOUSE_BUTTON_3      ::Vireo::Mouse::Button3
#define VIR_MOUSE_BUTTON_4      ::Vireo::Mouse::Button4
#define VIR_MOUSE_BUTTON_5      ::Vireo::Mouse::Button5
#define VIR_MOUSE_BUTTON_6      ::Vireo::Mouse::Button6
#define VIR_MOUSE_BUTTON_7      ::Vireo::Mouse::Button7
#define VIR_MOUSE_BUTTON_LAST   ::Vireo::Mouse::ButtonLast
#define VIR_MOUSE_BUTTON_LEFT   ::Vireo::Mouse::ButtonLeft
#define VIR_MOUSE_BUTTON_RIGHT  ::Vireo::Mouse::ButtonRight
#define VIR_MOUSE_BUTTON_MIDDLE ::Vireo::Mouse::ButtonMiddle