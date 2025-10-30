cmake_minimum_required(VERSION 3.30)

project(test_dll LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 仅在使用 MSVC 时添加 MSVC 特定的选项
if(MSVC)
  add_compile_options(/utf-8 /D_UNICODE /DUNICODE)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
    add_definitions(-D_UNICODE -DUNICODE)
endif()

# 主测试程序
add_executable(test_dll test_dll.cpp)

# 简单测试程序
add_executable(simple_test simple_test.cpp)

# 链接 Windows 库
if(WIN32)
    target_link_libraries(test_dll PRIVATE kernel32 user32)
    target_link_libraries(simple_test PRIVATE kernel32 user32)
endif()

# 复制 native.dll 到构建目录 (Release 版本)
set(MAIN_DLL_PATH "${CMAKE_SOURCE_DIR}/../build/Release/native.dll")
if(EXISTS "${MAIN_DLL_PATH}")
    add_custom_command(TARGET test_dll POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MAIN_DLL_PATH}"
         "$<TARGET_FILE_DIR:test_dll>/native.dll"
        COMMENT "Copying native.dll to test_dll directory"
    )
    add_custom_command(TARGET simple_test POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MAIN_DLL_PATH}"
         "$<TARGET_FILE_DIR:simple_test>/native.dll"
        COMMENT "Copying native.dll to simple_test directory"
    )
    message(STATUS "Found native.dll at: ${MAIN_DLL_PATH}")
else()
    message(WARNING "native.dll not found at: ${MAIN_DLL_PATH}")
    message(WARNING "Please build main project first, then rebuild test project")
endif()