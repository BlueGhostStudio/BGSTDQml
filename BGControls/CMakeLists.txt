cmake_minimum_required(VERSION 3.16)

project(
    BGControls
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(
    Qt6 6.2
    COMPONENTS Quick
    REQUIRED)

qt_add_library(BGControls STATIC)

set_source_files_properties(BGText.qml PROPERTIES QT_QML_SOURCE_TYPENAME Text)
set_source_files_properties(BGButton.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    Button)
set_source_files_properties(BGApplicationWindow.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME ApplicationWindow)
set_source_files_properties(BGTextField.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                       TextField)
set_source_files_properties(BGTextArea.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                      TextArea)
set_source_files_properties(BGRadioButton.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                         RadioButton)
set_source_files_properties(BGCheckBox.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                      CheckBox)
set_source_files_properties(BGSwitch.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    Switch)
set_source_files_properties(BGRoundButton.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                         RoundButton)
set_source_files_properties(BGToolButton.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                        ToolButton)
set_source_files_properties(BGToolBar.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                     ToolBar)
set_source_files_properties(BGPane.qml PROPERTIES QT_QML_SOURCE_TYPENAME Pane)
set_source_files_properties(BGFrame.qml PROPERTIES QT_QML_SOURCE_TYPENAME Frame)
set_source_files_properties(BGPage.qml PROPERTIES QT_QML_SOURCE_TYPENAME Page)
set_source_files_properties(BGComboBox.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                      ComboBox)
set_source_files_properties(BGPopup.qml PROPERTIES QT_QML_SOURCE_TYPENAME Popup)
set_source_files_properties(BGItemDelegate.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                          ItemDelegate)
set_source_files_properties(BGScrollIndicator.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME ScrollIndicator)
set_source_files_properties(BGScrollBar.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                       ScrollBar)
set_source_files_properties(BGScrollView.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                        ScrollView)
set_source_files_properties(BGGroupBox.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                      GroupBox)
set_source_files_properties(BGDialog.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    Dialog)
set_source_files_properties(BGDialogButtonBox.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME DialogButtonBox)
set_source_files_properties(BGFieldsLayout.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                          FieldsLayout)
set_source_files_properties(BGFieldsGroupBox.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME FieldsGroupBox)
set_source_files_properties(BGFieldsPane.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                        FieldsPane)
set_source_files_properties(BGOptionField.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                         OptionField)
set_source_files_properties(BGTabButton.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                       TabButton)
set_source_files_properties(BGTabBar.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    TabBar)
set_source_files_properties(BGSlider.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    Slider)
set_source_files_properties(BGRangeSlider.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                         RangeSlider)
set_source_files_properties(Private/BGSliderHandle.qml
                            PROPERTIES QT_QML_SKIP_QMLDIR_ENTRY SliderHandle)
set_source_files_properties(Private/BGSliderTick.qml
                            PROPERTIES QT_QML_SKIP_QMLDIR_ENTRY SliderTick)
set_source_files_properties(BGMenu.qml PROPERTIES QT_QML_SOURCE_TYPENAME Menu)
set_source_files_properties(BGMenuItem.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                      MenuItem)
set_source_files_properties(BGMenuBar.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                     MenuBar)
set_source_files_properties(BGMenuBarItem.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                         MenuBarItem)
set_source_files_properties(BGSpinBox.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                     SpinBox)
set_source_files_properties(BGDrawer.qml PROPERTIES QT_QML_SOURCE_TYPENAME
                                                    Drawer)
set_source_files_properties(BGToolSeparator.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME ToolSeparator)
set_source_files_properties(BGMenuSeparator.qml
                            PROPERTIES QT_QML_SOURCE_TYPENAME MenuSeparator)

qt_add_qml_module(
    BGControls
    URI BGStudio.Controls
    VERSION 1.0
    QML_FILES BGApplicationWindow.qml
              BGText.qml
              BGButton.qml
              BGTextField.qml
              BGTextArea.qml
              BGRadioButton.qml
              BGCheckBox.qml
              BGSwitch.qml
              BGRoundButton.qml
              BGToolButton.qml
              BGToolBar.qml
              BGPane.qml
              BGFrame.qml
              BGPage.qml
              BGComboBox.qml
              BGPopup.qml
              BGItemDelegate.qml
              BGScrollIndicator.qml
              BGScrollBar.qml
              BGScrollView.qml
              BGGroupBox.qml
              BGDialog.qml
              BGDialogButtonBox.qml
              BGFieldsLayout.qml
              BGFieldsGroupBox.qml
              BGFieldsPane.qml
              BGOptionField.qml
              BGTabBar.qml
              BGTabButton.qml
              BGSlider.qml
              BGRangeSlider.qml
              BGMenu.qml
              BGMenuItem.qml
              BGMenuBar.qml
              BGMenuBarItem.qml
              BGSpinBox.qml
              BGDrawer.qml
              BGToolSeparator.qml
              BGMenuSeparator.qml
              Private/BGSliderHandle.qml
              Private/BGSliderTick.qml
    SOURCES bgcontrols.cpp
            bgcontrols.h
            bgrefreshconnection.h
            bgrefreshconnection.cpp
            bgcoloroperation.h
            bgcoloroperation.cpp
            bgborderbackground.h
            bgborderbackground.cpp
            bgtext.h
            bgtext.cpp
            # bgpalettecommon.h bgpalettecommon.cpp
            bgpalette.h
            bgpalette.cpp
            bgbuttoncommon.h
            bgbuttoncommon.cpp
            bgcommonbase.h
            bgcommonbase.cpp
            bgswitchcommon.h
            bgswitchcommon.cpp
            bgfielditem.h
            bgfielditem.cpp
            bgfielditemattachedtype.h
            bgfielditemattachedtype.cpp
            bgpalettevalue.h
            bgpalettevalue.cpp
            # bgcolorschemebase.h bgcolorschemebase.cpp bgcommoncolorscheme.h
            # bgcommoncolorscheme.cpp bgbuttoncolorscheme.h
            # bgbuttoncolorscheme.cpp
    RESOURCE_PREFIX /qt/qml
    IMPORTS QtQuick.Controls
    RESOURCES icons/expand_more.png
              icons/expand_less.png
              icons/check_box_unchecked.png
              icons/check_box.png
              icons/check.png
              icons/chevron_right.png
              icons/radio_button_partial.png
              icons/radio_button_unchecked.png
              icons/radio_button.png
              icons/add.png
              icons/minimize.png
              icons/remove.png
              icons/chevron_left.png
              icons/menu.png
              icons/cursor.png
              icons/pause.png
              icons/play.png
              icons/player_end.png
              icons/player_forward.png
              icons/player_repeat.png
              icons/player_rewind.png
              icons/player_shuffle.png
              icons/player_start.png
              icons/player_stop.png
              icons/record.png
              icons/refresh.png
              icons/reset.png
              icons/slider.png
              icons/close.png
              icons/target.png
              icons/view.png
              MaterialIcons-Regular.ttf
    # NO_GENERATE_PLUGIN_SOURCE
)

set_target_properties(
    BGControls
    PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_compile_definitions(
    BGControls
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(BGControls PRIVATE Qt6::Quick)

target_include_directories(BGControls PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
