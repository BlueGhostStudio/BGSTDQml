cmake_minimum_required(VERSION 3.16)

project(
    demo-BGControls
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appDemo-BGControls main.cpp)

qt_add_qml_module(
    appDemo-BGControls
    URI demo-BGControls
    VERSION 1.0
    IMPORTS QtQuick.Controls
    QML_FILES Main.qml
              Customization.qml
              DemoCtrlGroup.qml
              ButtonsDemo.qml
              SwitchsDemo.qml
              InputsDemo.qml
              ColorMixer.qml
              PanesDemo.qml
              GroupBoxsDemo.qml
              FramesDemo.qml
              MenuDemo.qml
              DialogDemo.qml
              SlidersDemo.qml
              ColorSelectorButton.qml
              PaletteDemo.qml
    RESOURCES bgstudio.png)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
set_target_properties(
    appDemo-BGControls
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDemo-BGControls
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_link_libraries(appDemo-BGControls PRIVATE Qt6::Quick BGControlsplugin)

target_compile_definitions(
    appDemo-BGControls
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
