workspace "OpenGL"
    configurations {"Debug", "Release", "Dist"}
    architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies/GLFW"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/vendor/stb_image/stb_image.cpp"
    }

    includedirs {
        "Dependencies/GLFW/include",
        "Dependencies/GLEW/include",
        "Sandbox/include",
        "Sandbox/vendor/**"
    }

    -- buildoptions { "'pkg-config --cflags glfw3'" }

    libdirs { "Dependencies/GLEW/lib" }

    links {
        "GLFW", "GL", "dl", ":libGLEW.a" -- prefer static linking GLEW
    }

    linkoptions { 
        "-pthread"
    }
