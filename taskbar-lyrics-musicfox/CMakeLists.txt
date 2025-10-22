cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

add_compile_options(/utf-8 /D_UNICODE /DUNICODE)

project(native LANGUAGES CXX)

add_library(native SHARED)

target_sources(native
    PUBLIC
  FILE_SET CXX_MODULES FILES
"src/window/Renderer.cppm"
            "src/window/Window.cppm"
            "src/plugin/Plugin.cppm"
    PRIVATE
        "src/DllMain.cpp"
)

target_link_libraries(native PRIVATE d3d11)
target_link_libraries(native PRIVATE d2d1)
target_link_libraries(native PRIVATE dwrite)
target_link_libraries(native PRIVATE dxgi)
target_link_libraries(native PRIVATE dcomp)
target_link_libraries(native PRIVATE dwmapi)
