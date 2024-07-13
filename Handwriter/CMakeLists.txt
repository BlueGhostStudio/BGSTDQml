cmake_minimum_required(VERSION 3.16)

project(
    Handwriter
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(
    Qt6 6.2
    COMPONENTS Quick Sql
    REQUIRED)

option(HWSOFTRENDER "Enable software rendering" OFF)
set(canvas_sources "")
if(HWSOFTRENDER)
    list(APPEND canvas_sources render/software/hwsftcanvas.h
         render/software/hwsftcanvas.cpp)
    include_directories(render/software)
    add_compile_definitions(HWSOFTRENDER)
else()
    list(
        APPEND
        canvas_sources
        render/QSG/hwsgcanvas.h
        render/QSG/hwsgcanvas.cpp
        render/QSG/contentnode.h
        render/QSG/contentnode.cpp
        render/QSG/strokegeometrynode.h
        render/QSG/strokegeometrynode.cpp
        render/QSG/hwsgguideline.h
        render/QSG/hwsgguideline.cpp)
    include_directories(render/QSG)
endif()

add_subdirectory(common)
include_directories(common)

qt_add_library(Handwriter STATIC)

# set_source_files_properties(Private/GestureArea.qml PROPERTIES
# QT_QML_SKIP_QMLDIR_ENTRY GestureArea)

set_source_files_properties(
    Private/StrokeCollection.qml PROPERTIES QT_QML_SKIP_QMLDIR_ENTRY
                                            StrokeCollection)
# set_source_files_properties(StrokeSettings.qml PROPERTIES
# QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
    Handwriter
    URI BGStudio.Handwriter
    VERSION 1.0
    QML_FILES # Private/GestureArea.qml
              Private/StrokeCollection.qml
    SOURCES hwcanvasbase.cpp
            hwcanvasbase.h
            hwcanvas.h
            hwwriter.h
            hwwriter.cpp
            hwsettings.h
            hwsettings.cpp
            global_variable.cpp
            hwstrokesample.h
            hwstrokesample.cpp
            hwguidelinebase.h
            hwguidelinebase.cpp
            hwguideline.h
            hwpaper.h
            hwpaper.cpp
            hwviewport.h
            hwviewport.cpp
            hwviewportattachedtype.h
            hwviewportattachedtype.cpp
            strokepoint.h
            strokepoint.cpp
            # hwgesturearea.h hwgesturearea.cpp
            hwgrabpreview.h
            hwgrabpreview.cpp
            hwpaperpreview.h
            hwpaperpreview.cpp
            hwoutput.h
            hwoutput.cpp
            hwdatastorage.h
            hwdatastorage.cpp
            hwlsdpapermodel.h
            hwlsdpapermodel.cpp
            androidinterface.h
            androidinterface.cpp
            ${canvas_sources}
    RESOURCE_PREFIX /qt/qml
    RESOURCES icons/quill_pen.png
              icons/NextEmptySpace.png
              icons/NextEmptyLine.png
              icons/autoScroll.png
              icons/indicator_magnifier.png
              icons/indicator_move.png
              icons/indicator_move1.png
              icons/indicator_quill_pen.png
              icons/indicator_menu.png
              imgs/page.png
              imgs/page_line.png
              imgs/page_gride.png
              imgs/fabric.png
              imgs/fabric_line.png
              imgs/fabric_grid.png
              strokeSample.json
              handwriter.db
    QML_FILES StrokeColorSelectorComboBox.qml
              StrokePresets.qml
              StrokeSampleButton.qml
              StrokeColorSelector.qml
              StrokeColorButton.qml
              UserSavedStrokes.qml
              StrokeAdjustment.qml
              # StrokeSettings.qml
              StrokeKit.qml
              StrokePreview.qml
              HWWriteNavigator.qml
              HWNavigatorPreview.qml
              QuickToolKit.qml
              HWViewportPreview.qml
              PaperPanBehavior.qml
              imgs/cover1.png
              imgs/cover2.png
              imgs/cover3.png)

set_target_properties(
    Handwriter
    PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_compile_definitions(
    Handwriter
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Handwriter PRIVATE Qt6::Quick Qt6::Sql HandwriterCommon)

target_include_directories(Handwriter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
