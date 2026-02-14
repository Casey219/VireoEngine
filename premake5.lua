workspace "VireoEngine"
	architecture "x86_64"
	startproject "Editor"

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
IncludeDir["glm"]="ThirdParty/glm"
IncludeDir["stb_image"]="ThirdParty/stb_image"
IncludeDir["entt"] = "ThirdParty/entt/include"

group "ThirdParty"
	include "ThirdParty/GLFW"
	include "ThirdParty/GLAD"
	include "ThirdParty/imgui"
group ""


project "Runtime"
	location "Runtime"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "virpch.h"
	pchsource "Runtime/src/virpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp",
		"%{IncludeDir.glm}/glm/glm/**.hpp",
		"%{IncludeDir.glm}/glm.glm/**.inl"

	}

	-- defines
	-- {
	-- 	"_CRT_SECURE_NO_WARNINGS"
	-- }

	includedirs
	{
		"%{prj.name}/src",
		"ThirdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest" 

		defines
		{
			"VIR_PLATFORM_WINDOWS",
			"VIR_BUILD_DLL"
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
project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Runtime/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Runtime"
	}

	filter "system:windows"
		cppdialect "C++17"

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