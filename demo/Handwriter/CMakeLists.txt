cmake_minimum_required(VERSION 3.16)

project(
    demo-Handwriter
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appDemo-Handwriter main.cpp)

set_source_files_properties(NavigatorToolbars.qml
                            PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
    appDemo-Handwriter
    URI DemoHandwriter
    VERSION 1.0
    QML_FILES Main.qml StrokeSelectorComboBox.qml StrokeDrawer.qml
              NavigatorToolbars.qml)

set_property(
    TARGET appDemo-Handwriter
    APPEND
    PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
set_target_properties(
    appDemo-Handwriter
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDemo-Handwriter
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_link_libraries(appDemo-Handwriter PRIVATE Qt6::Quick Handwriterplugin
                                                 BGControlsplugin)

include(GNUInstallDirs)
install(
    TARGETS appDemo-Handwriter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
