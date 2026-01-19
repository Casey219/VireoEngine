workspace "VireoEngine"
	architecture "x64"
	startproject "VireoEditor"

	configurations
	{

		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir={}
IncludeDir["GLFW"]="ThirdParty/GLFW/include"
IncludeDir["GLAD"]="ThirdParty/GLAD/include"
IncludeDir["imgui"]="ThirdParty/imgui"

include "ThirdParty/GLFW"
include "ThirdParty/GLAD"
include "ThirdParty/imgui"


project "VireoEngine"
	location "VireoEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "virpch.h"
	pchsource "VireoEngine/src/virpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"ThirdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}"
	}
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest" 

		defines
		{
			"VIR_PLATFORM_WINDOWS",
			"VIR_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/VireoEditor")
		}

	filter "configurations:Debug"
		defines "VIR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VIR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VIR_DIST"
		runtime "Release"
		optimize "On"
    
    filter "action:vs*" --add utf-8 for spdlog
        buildoptions { "/utf-8" }
project "VireoEditor"
	location "VireoEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"ThirdParty/spdlog/include", 
		"VireoEngine/src"
	}

	links
	{
		"VireoEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"VIR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "VIR_DEBUG"
		--buildoptions {"/MDd"}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "VIR_RELEASE"
		--buildoptions {"/MD"}
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "VIR_DIST"
		--buildoptions {"/MD"}
		runtime "Release"
		optimize "On"

    filter "action:vs*"
        buildoptions { "/utf-8" } --add utf-8 for spdlog