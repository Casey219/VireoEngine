#pragma once
#include "Core/PlatformDetection.h"

#ifdef HZ_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif


#include<iostream>
#include<memory>
#include<utility>
#include<algorithm>
#include<functional>

#include<string>
#include<sstream>
#include<vector>

#include "Core/Base.h"
#include"Core/Logger.h"

#include "Debug/Instrumentor.h"

#ifdef VIR_PLATFORM_WINDOWS
	#include<Windows.h>
#endif
