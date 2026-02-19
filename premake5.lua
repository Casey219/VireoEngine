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

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir={}
IncludeDir["GLFW"]="ThirdParty/GLFW/include"
IncludeDir["GLAD"]="ThirdParty/GLAD/include"
IncludeDir["imgui"]="ThirdParty/imgui"
IncludeDir["glm"]="ThirdParty/glm"
IncludeDir["stb_image"]="ThirdParty/stb_image"
IncludeDir["entt"] = "ThirdParty/entt/include"
IncludeDir["yaml_cpp"] = "ThirdParty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "ThirdParty/ImGuizmo"
IncludeDir["shaderc"] = "ThirdParty/shaderc/include"
IncludeDir["SPIRV_Cross"] = "ThirdParty/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["Box2D"] = "ThirdParty/Box2D/include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
--Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

group "ThirdParty"
	include "ThirdParty/GLFW"
	include "ThirdParty/GLAD"
	include "ThirdParty/imgui"
	include "ThirdParty/yaml-cpp"
	include "ThirdParty/Box2D"
group ""


project "Runtime"
	location "Runtime"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{IncludeDir.glm}/glm.glm/**.inl",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.h",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.cpp",
	}

	-- defines
	-- {
	-- 	"_CRT_SECURE_NO_WARNINGS"
	-- }

	includedirs
	{
		"%{prj.name}/src",
		"ThirdParty/spdlog/include",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
	}
	links
	{
		"Box2D",
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp"
	}
	filter "files:ThirdParty/ImGuizmo/**.cpp"
	flags { "NoPCH" }

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
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "VIR_RELEASE"
		runtime "Release"
		optimize "On"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "VIR_DIST"
		runtime "Release"
		optimize "On"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
    
    filter "action:vs*" --add utf-8 for spdlog
        buildoptions { "/utf-8" }
project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Panels/**.h",
		"%{prj.name}/Panels/**.cpp"
	}

	includedirs
	{
		"ThirdParty/spdlog/include", 
		"Runtime/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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