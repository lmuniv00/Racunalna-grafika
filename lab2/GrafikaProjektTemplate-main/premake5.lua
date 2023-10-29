workspace "GrafikaLabovi_LukaMunivrana"
    architecture "x64"

    configurations { "Debug", "Release"}
    outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GrafikaLabovi_LukaMunivrana"
    location "GrafikaLabovi_LukaMunivrana"
    kind "ConsoleApp"
    language "c++"

    targetdir("bin/bin/" ..outputdir.. "/%{prj.name}")
    objdir("bin/intermediates/" ..outputdir.. "/%{prj.name}")
    
    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.inl",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/vendor/stb_image/**.h",
        "%{prj.name}/src/vendor/stb_image/**.c",
        "%{prj.name}/src/vendor/glm/**.hpp",
        "%{prj.name}/src/vendor/glm/**.inl",
        "%{prj.name}/res/shaders/**.glsl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/src/vendor",
        "%{prj.name}/res/shaders",
        "Depend/Include"
    }

    links 
    {
        "opengl32.lib",
        "glfw3.lib"
    }

    if _ACTION == "vs2017" then
        libdirs 
        {
            "Depend/Libraries/vs2017"
        }
    elseif _ACTION == "vs2019" then
        libdirs
        {
            "Depend/Libraries/vs2019"
        }
    elseif _ACTION == "vs2022" then
        libdirs
        {
            "Depend/Libraries/vs2022"
        }
    end

    filter "system:windows"
        cppdialect "c++17"
        systemversion "latest"

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "on"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "on"