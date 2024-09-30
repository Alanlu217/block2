cmake_minimum_required(VERSION 3.20)
project(block)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("third_party/raylib")

set(imgui_SOURCE_DIR "third_party/imgui")
set(rlimgui_SOURCE_DIR "third_party/rlImGui")

add_library(
    imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_link_libraries(rlimgui PUBLIC imgui raylib)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})

IF (WIN32 AND CMAKE_BUILD_TYPE MATCHES Release)
  add_executable(${PROJECT_NAME} WIN32)
ELSE()
  add_executable(${PROJECT_NAME})
ENDIF()

target_sources(${PROJECT_NAME} PRIVATE
    "src/cpp/main.cpp"
    "src/cpp/app.cpp"
    "src/cpp/window.cpp"

    "src/cpp/managers/resource_manager.cpp"
    "src/cpp/managers/view_manager.cpp"
    "src/cpp/managers/event_manager.cpp"
    "src/cpp/managers/physics_manager.cpp"
    "src/cpp/managers/save_manager.cpp"
    "src/cpp/managers/input_manager.cpp"

    "src/cpp/views/start_view.cpp"
    "src/cpp/views/game_view.cpp"
    "src/cpp/views/editor_view.cpp"
    "src/cpp/views/death_view.cpp"

    "src/cpp/entities/squircle.cpp"
    "src/cpp/entities/dragger.cpp"
    "src/cpp/entities/background.cpp"
    "src/cpp/entities/hud.cpp"

    "src/cpp/entities/objects/objects.cpp"
    "src/cpp/entities/objects/basic_platform.cpp"
    "src/cpp/entities/objects/spiky_platform.cpp"
    "src/cpp/entities/objects/icy_platform.cpp"
    "src/cpp/entities/objects/ghost_platform.cpp"
    "src/cpp/entities/objects/text_object.cpp"
    "src/cpp/entities/objects/double_jump_item.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "src/include"
)

target_link_libraries(${PROJECT_NAME}
    "rlimgui"
    "imgui"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE ASSETS_DIR="assets/"

    PRIVATE VERSION_MAJOR="1"
    PRIVATE VERSION_MINOR="7"
    PRIVATE VERSION_PATCH="1"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
    DEPENDS ${PROJECT_NAME}
)

if(APPLE)
  add_custom_target(app
    COMMAND ${PROJECT_SOURCE_DIR}/util/create_app.sh ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)
endif()

