# Nova - a very streamlined build file
# (c) 2025 Lunarwing Games
#
# Dependencies:
#
# GLFW      -> 3.4
# GLEW      -> 2.2.0
# stb_image -> 2.30
# cglm      -> 0.9.6
# imgui     -> 1.92.4
# SteamAPI  -> 1.62

cmake_minimum_required(VERSION 3.20)
project(nova
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE NOVA_SRC CONFIGURE_DEPENDS "src/*.c")
add_executable(nova ${NOVA_SRC})

# Including all the cool libraries
target_include_directories(nova
    PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/vendor"
        "${CMAKE_SOURCE_DIR}/vendor/stb"
        "${CMAKE_SOURCE_DIR}/vendor/cglm/include"
        "${CMAKE_SOURCE_DIR}/vendor/glfw/include"
        "${CMAKE_SOURCE_DIR}/vendor/glew/include"
        "${CMAKE_SOURCE_DIR}/vendor/imgui"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/backends"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_include_directories(nova PRIVATE "C:/steamworks/sdk/public")
else()
    target_include_directories(nova PRIVATE "$ENV{HOME}/sdk/public")
endif()

# Compile flags and definitions
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/MP /permissive- /W3 /Od)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/Ot)
    endif()
else()
    add_compile_options(
        -Wall -Wextra -Wshadow -Wundef -Wmissing-declarations
        -Wpointer-arith
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3)
    else()
        add_compile_options(-O1)
    endif()
endif()

# Linking important libraries!
if(WIN32)
    set(STEAMWORKS_LIB_DIR "C:/steamworks/sdk/redistributable_bin/win64")
    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw")
    set(GLEW_LIB_DIR "${CMAKE_SOURCE_DIR}/vendor/glew")

    link_directories(
        ${STEAMWORKS_LIB_DIR}
        ${GLFW_LIB_DIR}
        ${GLEW_LIB_DIR}
    )

    target_link_libraries(nova PRIVATE
        glfw3dll.lib
        steam_api64.lib
        opengl32.lib
        winmm.lib
        kernel32.lib
        user32.lib
        gdi32.lib
        shell32.lib
        glew32.lib
        
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Come back to this
else()
    # This as well
endif()

message(STATUS "Compiler ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "Compiler version: ${CMAKE_C_COMPILER_VERSION}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET nova POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${STEAMWORKS_LIB_DIR}/steam_api64.dll"
            $<TARGET_FILE_DIR:nova>
        COMMENT "Copied steam_api64.dll to output directory."
    )
endif()