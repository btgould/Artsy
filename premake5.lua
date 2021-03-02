workspace "OpenGL"
    configurations {"Debug", "Release", "Dist"}
    architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies/GLFW"
include "Sandbox/vendor/imgui"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"
    staticruntime "On"

    pchheader "artsypch.hpp"

    symbols "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/vendor/stb_image/stb_image.cpp",
    }

    -- removefiles "%{prj.location}/src/Drawing/**"

    includedirs {
        "Dependencies/GLFW/include",
        "Dependencies/GLEW/include",
        "Sandbox/src", 
        "Sandbox/vendor",
        "Sandbox/vendor/imgui"
    }

    libdirs { "Dependencies/GLEW/lib" }

    links {
        "GLFW", "GL", "dl", ":libGLEW.a", -- prefer static linking GLEW
        "ImGui"
    }

    linkoptions { 
        "-pthread"
    }
