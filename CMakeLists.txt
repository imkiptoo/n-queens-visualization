cmake_minimum_required(VERSION 3.16)  # Lower version for better compatibility
project(recursive_n_queens_mit_qt VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Allow setting Qt path from command line or environment
if(DEFINED ENV{Qt6_DIR} AND NOT DEFINED Qt6_DIR)
    set(Qt6_DIR $ENV{Qt6_DIR})
endif()

if(NOT DEFINED CMAKE_PREFIX_PATH AND EXISTS "D:/pgf/Qt/6.8.1/mingw_64")
    set(CMAKE_PREFIX_PATH "D:/pgf/Qt/6.8.1/mingw_64")
endif()

# Find Qt packages
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

# Define source files
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

# Create executable
add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Windows-specific settings
if(WIN32)
    # Enable deployment
    include(GNUInstallDirs)

    set(DEBUG_SUFFIX)
    if(MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()

    # Determine Qt installation path
    if(DEFINED CMAKE_PREFIX_PATH)
        set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    elseif(DEFINED Qt6_DIR)
        get_filename_component(QT_INSTALL_PATH "${Qt6_DIR}/../../../" ABSOLUTE)
    else()
        message(WARNING "Qt installation path not found. Deployment may fail.")
    endif()

    # Deploy Qt runtime files
    if(EXISTS "${QT_INSTALL_PATH}/bin")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
        )

        # Copy Qt runtime DLLs
        foreach(QT_LIB Core Gui Widgets)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
        endforeach()
    endif()
endif()

# Installation configuration
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
)

# Enable testing (if you add tests later)
#enable_testing()