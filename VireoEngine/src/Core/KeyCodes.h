#pragma once

namespace Vireo
{
	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			// From glfw3.h
			Space = 32,
			ApostropVIR = 39, /* ' */
			Comma = 44, /* , */
			Minus = 45, /* - */
			Period = 46, /* . */
			Slash = 47, /* / */

			D0 = 48, /* 0 */
			D1 = 49, /* 1 */
			D2 = 50, /* 2 */
			D3 = 51, /* 3 */
			D4 = 52, /* 4 */
			D5 = 53, /* 5 */
			D6 = 54, /* 6 */
			D7 = 55, /* 7 */
			D8 = 56, /* 8 */
			D9 = 57, /* 9 */

			Semicolon = 59, /* ; */
			Equal = 61, /* = */

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,  /* [ */
			Backslash = 92,  /* \ */
			RightBracket = 93,  /* ] */
			GraveAccent = 96,  /* ` */

			World1 = 161, /* non-US #1 */
			World2 = 162, /* non-US #2 */

			/* Function keys */
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			/* Keypad */
			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348
		};
	}
}


// From glfw3.h
#define VIR_KEY_SPACE           ::Vireo::Key::Space
#define VIR_KEY_APOSTROPVIR     ::Vireo::Key::ApostropVIR    /* ' */
#define VIR_KEY_COMMA           ::Vireo::Key::Comma         /* , */
#define VIR_KEY_MINUS           ::Vireo::Key::Minus         /* - */
#define VIR_KEY_PERIOD          ::Vireo::Key::Period        /* . */
#define VIR_KEY_SLASH           ::Vireo::Key::Slash         /* / */
#define VIR_KEY_0               ::Vireo::Key::D0
#define VIR_KEY_1               ::Vireo::Key::D1
#define VIR_KEY_2               ::Vireo::Key::D2
#define VIR_KEY_3               ::Vireo::Key::D3
#define VIR_KEY_4               ::Vireo::Key::D4
#define VIR_KEY_5               ::Vireo::Key::D5
#define VIR_KEY_6               ::Vireo::Key::D6
#define VIR_KEY_7               ::Vireo::Key::D7
#define VIR_KEY_8               ::Vireo::Key::D8
#define VIR_KEY_9               ::Vireo::Key::D9
#define VIR_KEY_SEMICOLON       ::Vireo::Key::Semicolon     /* ; */
#define VIR_KEY_EQUAL           ::Vireo::Key::Equal         /* = */
#define VIR_KEY_A               ::Vireo::Key::A
#define VIR_KEY_B               ::Vireo::Key::B
#define VIR_KEY_C               ::Vireo::Key::C
#define VIR_KEY_D               ::Vireo::Key::D
#define VIR_KEY_E               ::Vireo::Key::E
#define VIR_KEY_F               ::Vireo::Key::F
#define VIR_KEY_G               ::Vireo::Key::G
#define VIR_KEY_H               ::Vireo::Key::H
#define VIR_KEY_I               ::Vireo::Key::I
#define VIR_KEY_J               ::Vireo::Key::J
#define VIR_KEY_K               ::Vireo::Key::K
#define VIR_KEY_L               ::Vireo::Key::L
#define VIR_KEY_M               ::Vireo::Key::M
#define VIR_KEY_N               ::Vireo::Key::N
#define VIR_KEY_O               ::Vireo::Key::O
#define VIR_KEY_P               ::Vireo::Key::P
#define VIR_KEY_Q               ::Vireo::Key::Q
#define VIR_KEY_R               ::Vireo::Key::R
#define VIR_KEY_S               ::Vireo::Key::S
#define VIR_KEY_T               ::Vireo::Key::T
#define VIR_KEY_U               ::Vireo::Key::U
#define VIR_KEY_V               ::Vireo::Key::V
#define VIR_KEY_W               ::Vireo::Key::W
#define VIR_KEY_X               ::Vireo::Key::X
#define VIR_KEY_Y               ::Vireo::Key::Y
#define VIR_KEY_Z               ::Vireo::Key::Z
#define VIR_KEY_LEFT_BRACKET    ::Vireo::Key::LeftBracket   /* [ */
#define VIR_KEY_BACKSLASH       ::Vireo::Key::Backslash     /* \ */
#define VIR_KEY_RIGHT_BRACKET   ::Vireo::Key::RightBracket  /* ] */
#define VIR_KEY_GRAVE_ACCENT    ::Vireo::Key::GraveAccent   /* ` */
#define VIR_KEY_WORLD_1         ::Vireo::Key::World1        /* non-US #1 */
#define VIR_KEY_WORLD_2         ::Vireo::Key::World2        /* non-US #2 */

/* Function keys */
#define VIR_KEY_ESCAPE          ::Vireo::Key::Escape
#define VIR_KEY_ENTER           ::Vireo::Key::Enter
#define VIR_KEY_TAB             ::Vireo::Key::Tab
#define VIR_KEY_BACKSPACE       ::Vireo::Key::Backspace
#define VIR_KEY_INSERT          ::Vireo::Key::Insert
#define VIR_KEY_DELETE          ::Vireo::Key::Delete
#define VIR_KEY_RIGHT           ::Vireo::Key::Right
#define VIR_KEY_LEFT            ::Vireo::Key::Left
#define VIR_KEY_DOWN            ::Vireo::Key::Down
#define VIR_KEY_UP              ::Vireo::Key::Up
#define VIR_KEY_PAGE_UP         ::Vireo::Key::PageUp
#define VIR_KEY_PAGE_DOWN       ::Vireo::Key::PageDown
#define VIR_KEY_HOME            ::Vireo::Key::Home
#define VIR_KEY_END             ::Vireo::Key::End
#define VIR_KEY_CAPS_LOCK       ::Vireo::Key::CapsLock
#define VIR_KEY_SCROLL_LOCK     ::Vireo::Key::ScrollLock
#define VIR_KEY_NUM_LOCK        ::Vireo::Key::NumLock
#define VIR_KEY_PRINT_SCREEN    ::Vireo::Key::PrintScreen
#define VIR_KEY_PAUSE           ::Vireo::Key::Pause
#define VIR_KEY_F1              ::Vireo::Key::F1
#define VIR_KEY_F2              ::Vireo::Key::F2
#define VIR_KEY_F3              ::Vireo::Key::F3
#define VIR_KEY_F4              ::Vireo::Key::F4
#define VIR_KEY_F5              ::Vireo::Key::F5
#define VIR_KEY_F6              ::Vireo::Key::F6
#define VIR_KEY_F7              ::Vireo::Key::F7
#define VIR_KEY_F8              ::Vireo::Key::F8
#define VIR_KEY_F9              ::Vireo::Key::F9
#define VIR_KEY_F10             ::Vireo::Key::F10
#define VIR_KEY_F11             ::Vireo::Key::F11
#define VIR_KEY_F12             ::Vireo::Key::F12
#define VIR_KEY_F13             ::Vireo::Key::F13
#define VIR_KEY_F14             ::Vireo::Key::F14
#define VIR_KEY_F15             ::Vireo::Key::F15
#define VIR_KEY_F16             ::Vireo::Key::F16
#define VIR_KEY_F17             ::Vireo::Key::F17
#define VIR_KEY_F18             ::Vireo::Key::F18
#define VIR_KEY_F19             ::Vireo::Key::F19
#define VIR_KEY_F20             ::Vireo::Key::F20
#define VIR_KEY_F21             ::Vireo::Key::F21
#define VIR_KEY_F22             ::Vireo::Key::F22
#define VIR_KEY_F23             ::Vireo::Key::F23
#define VIR_KEY_F24             ::Vireo::Key::F24
#define VIR_KEY_F25             ::Vireo::Key::F25

/* Keypad */
#define VIR_KEY_KP_0            ::Vireo::Key::KP0
#define VIR_KEY_KP_1            ::Vireo::Key::KP1
#define VIR_KEY_KP_2            ::Vireo::Key::KP2
#define VIR_KEY_KP_3            ::Vireo::Key::KP3
#define VIR_KEY_KP_4            ::Vireo::Key::KP4
#define VIR_KEY_KP_5            ::Vireo::Key::KP5
#define VIR_KEY_KP_6            ::Vireo::Key::KP6
#define VIR_KEY_KP_7            ::Vireo::Key::KP7
#define VIR_KEY_KP_8            ::Vireo::Key::KP8
#define VIR_KEY_KP_9            ::Vireo::Key::KP9
#define VIR_KEY_KP_DECIMAL      ::Vireo::Key::KPDecimal
#define VIR_KEY_KP_DIVIDE       ::Vireo::Key::KPDivide
#define VIR_KEY_KP_MULTIPLY     ::Vireo::Key::KPMultiply
#define VIR_KEY_KP_SUBTRACT     ::Vireo::Key::KPSubtract
#define VIR_KEY_KP_ADD          ::Vireo::Key::KPAdd
#define VIR_KEY_KP_ENTER        ::Vireo::Key::KPEnter
#define VIR_KEY_KP_EQUAL        ::Vireo::Key::KPEqual

#define VIR_KEY_LEFT_SHIFT      ::Vireo::Key::LeftShift
#define VIR_KEY_LEFT_CONTROL    ::Vireo::Key::LeftControl
#define VIR_KEY_LEFT_ALT        ::Vireo::Key::LeftAlt
#define VIR_KEY_LEFT_SUPER      ::Vireo::Key::LeftSuper
#define VIR_KEY_RIGHT_SHIFT     ::Vireo::Key::RightShift
#define VIR_KEY_RIGHT_CONTROL   ::Vireo::Key::RightControl
#define VIR_KEY_RIGHT_ALT       ::Vireo::Key::RightAlt
#define VIR_KEY_RIGHT_SUPER     ::Vireo::Key::RightSuper
#define VIR_KEY_MENU            ::Vireo::Key::Menu