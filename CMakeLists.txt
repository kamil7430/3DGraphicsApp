cmake_minimum_required(VERSION 4.0)
project(3DGraphicsApp)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# GLAD (generating with Python)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import jinja2"
        RESULT_VARIABLE JINJA_FOUND
        OUTPUT_QUIET
        ERROR_QUIET
)

if(NOT JINJA_FOUND EQUAL 0)
    message(FATAL_ERROR "Library 'jinja2' not found. Try to install manually using: pip install jinja2")
endif()

FetchContent_Declare(
        glad_repo
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.8
)
FetchContent_MakeAvailable(glad_repo)

set(GLAD_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/glad_auto")
file(MAKE_DIRECTORY ${GLAD_OUT_DIR})

execute_process(
        COMMAND ${Python3_EXECUTABLE} -m glad --api gl:core=4.6 --out-path "${GLAD_OUT_DIR}" c
        WORKING_DIRECTORY "${glad_repo_SOURCE_DIR}"
        RESULT_VARIABLE GLAD_GEN_RESULT
)

if(NOT GLAD_GEN_RESULT EQUAL 0)
    message(FATAL_ERROR "GLAD generation error.")
endif()

add_library(glad STATIC "${GLAD_OUT_DIR}/src/gl.c")
target_include_directories(glad PUBLIC "${GLAD_OUT_DIR}/include")

if (UNIX)
    target_link_libraries(glad PRIVATE dl)
endif()

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY  https://github.com/g-truc/glm.git
        GIT_TAG         8f6213d379a904f5ae910e09a114e066e25faf57
)
FetchContent_MakeAvailable(glm)

# Assimp
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        d1e6bcff6b01c5da9856a2b6ed52ae9a06dfe8da
)
FetchContent_MakeAvailable(assimp)

# ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.92.5
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw glad)

# APP
add_executable(3DGraphicsApp src/main.cpp
        src/shaders/shader.cpp
        src/shaders/shader.h
        src/models/objects/sphere.h
        src/models/objects/sports_car.h
        src/models/model_initializer.h
        src/models/objects/rubber_ducky.h
        src/camera.h
        src/models/object_base.cpp
        src/models/object_base.h
        src/models/object_opengl_model.cpp
        src/models/object_opengl_model.h
        src/light.h
        src/models/objects/floor.h
        src/models/objects/pine_tree.h
        src/models/objects/mirror_surface.h
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)

target_link_libraries(3DGraphicsApp PRIVATE
        glm::glm
        glad
        glfw
        OpenGL::GL
        assimp::assimp
        imgui
)