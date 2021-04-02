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
    cppdialect "C++17"
    staticruntime "On"

    pchheader "artsypch.hpp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/vendor/stb_image/stb_image.cpp",
        "%{prj.location}/vendor/stb_image/stb_image_write.cpp"
    }

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

    filter "configurations:Debug"
        defines "ARTSY_ENABLE_ASSERTS"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter "configurations:Dist"
        optimize "On"
