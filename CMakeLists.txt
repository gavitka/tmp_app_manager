cmake_minimum_required(VERSION 3.18)

project("app_manger")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DMESSAGE_HANDLER)

find_package(Qt5 COMPONENTS
    Widgets
    Quick
    Core
    QuickControls2
    Multimedia
    winextras
    REQUIRED)
    
qt5_add_resources(QTRESOURCES src/res.qrc)

file(GLOB_RECURSE SRC_UI
    src/main.qml
    src/qtquickcontrols2.conf
    src/imports/Theme/Theme.qml
    src/Components/WindowList.qml
)

add_executable(${PROJECT_NAME} WIN32 
    src/main.cpp
    src/backend.cpp
    src/backend.h
    src/appmanager.cpp
    src/appmanager.h
    src/pch.h
    src/pch.cpp
    src/lib.h
    src/lib.cpp
    src/kheventfilter.h
    src/applistmodel.h
    src/applistmodel.cpp

    ${SRC_UI}
    ${RESOURCES}
    ${QTRESOURCES}
    )

    
source_group("ui" FILES ${SRC_UI})

set(PROJECT_DLL "hooks_dll")

set(QML_IMPORT_PATH 
    "${CMAKE_SOURCE_DIR}/src/imports" 
    CACHE STRING "" FORCE)

add_library(${PROJECT_DLL} SHARED
    src/hooks_dll/hook.cpp
    src/hooks_dll/hook.h
    )

target_include_directories(${PROJECT_DLL} PUBLIC
    "${CMAKE_SOURCE_DIR}/src/"
    )

target_precompile_headers(${PROJECT_NAME}
    PUBLIC src/pch.h
    )

target_precompile_headers(${PROJECT_DLL}
    PUBLIC src/pch.h
    )

target_compile_definitions(${PROJECT_DLL} PUBLIC DLLEXPORT)

set(LIBS
    Qt5::Widgets
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    Qt5::Multimedia
    Qt5::WinExtras
    )

target_link_libraries(${PROJECT_NAME} ${LIBS})

target_link_libraries(${PROJECT_NAME} ${PROJECT_DLL})

target_link_libraries(${PROJECT_DLL} Qt5::Core)

# set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE STREQUAL "Release")

# RUNNUNG WINDEPLOYQT

# Retrieve the absolute path to qmake and then use that path to find
# the binaries

     get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
     get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
     find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND "${CMAKE_COMMAND}" -E
             env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                 "$<TARGET_FILE:${PROJECT_NAME}>" ARGS --qmldir ${CMAKE_SOURCE_DIR}/src/
         COMMENT "Running windeployqt..."
     )

endif()
