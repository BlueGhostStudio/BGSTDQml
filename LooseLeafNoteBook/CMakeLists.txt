cmake_minimum_required(VERSION 3.16)

project(
    LooseLeafNoteBook
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appLooseLeafNoteBook main.cpp)

set_source_files_properties(ActivePaperList.qml PROPERTIES QT_QML_SINGLETON_TYPE
                                                           TRUE)

qt_add_qml_module(
    appLooseLeafNoteBook
    URI LooseLeafNoteBook
    VERSION 1.0
    QML_FILES Main.qml
              ActivePaperList.qml
              MainForm.ui.qml
              LooseLeafPaperList.qml
              LooseLeafViewport.qml
              LooseLeafPaper.qml
              ActivePaperTabBar.qml)

set_property(
    TARGET appLooseLeafNoteBook
    APPEND
    PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
set_target_properties(
    appLooseLeafNoteBook
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appLooseLeafNoteBook
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_link_libraries(appLooseLeafNoteBook PRIVATE Qt6::Quick Handwriterplugin
                                                   BGControlsplugin)

include(GNUInstallDirs)
install(
    TARGETS appLooseLeafNoteBook
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
