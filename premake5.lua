workspace "VireoEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VireoEngine"
	location "VireoEngine"
	kind "SharedLib"
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
		"ThirdParty/spdlog/include"   
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
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
		symbols "On"

	filter "configurations:Release"
		defines "VIR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VIR_DIST"
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
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"VIR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "VIR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "VIR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "VIR_DIST"
		optimize "On"

    filter "action:vs*"
        buildoptions { "/utf-8" } --add utf-8 for spdlog