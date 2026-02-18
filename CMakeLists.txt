cmake_minimum_required(VERSION 3.16)
project(bezier_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90.1
)
FetchContent_Populate(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_BACKEND_DIR ${imgui_SOURCE_DIR}/backends)

add_library(imgui STATIC
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})

add_library(imgui_backends STATIC
  ${IMGUI_BACKEND_DIR}/imgui_impl_sdl2.cpp
  ${IMGUI_BACKEND_DIR}/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(imgui_backends PUBLIC ${IMGUI_DIR} ${IMGUI_BACKEND_DIR})
target_link_libraries(imgui_backends PUBLIC imgui SDL2::SDL2)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(bezier_playground ${SOURCES})
target_include_directories(bezier_playground PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${IMGUI_DIR}
  ${IMGUI_BACKEND_DIR}
)
target_link_libraries(bezier_playground PRIVATE
  imgui_backends
  SDL2::SDL2
)

# macOS: bundle and link frameworks if needed
if(APPLE)
  target_link_libraries(bezier_playground PRIVATE "-framework Cocoa")
endif()
