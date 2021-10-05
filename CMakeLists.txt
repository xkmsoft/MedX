cmake_minimum_required(VERSION 3.20)
project(MedX)

set(CMAKE_CXX_STANDARD 14)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
    cmake_policy(SET CMP0071 NEW)
endif()

find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
    message(FATAL_ERROR "${PROJECT_NAME}: ${VTK_NOT_FOUND_MESSAGE}")
endif()

find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
    message(FATAL ERROR "${PROJECT_NAME}: ${ITK_NOT_FOUND_MESSAGE}")
endif ()
include(${ITK_USE_FILE})

if(NOT(vtkGUISupportQt_LOADED OR TARGET VTK::GUISupportQt))
    message(FATAL_ERROR "${PROJECT_NAME}: VTK not built with Qt support.")
endif()

if(NOT DEFINED VTK_QT_VERSION)
    set(VTK_QT_VERSION 5)
endif()

set(qt_components Core Gui Widgets)
if(${VTK_QT_VERSION} VERSION_GREATER_EQUAL 6)
    list(APPEND qt_components OpenGLWidgets)
endif()
list(SORT qt_components)

# We have ui files, so this will also bring in the macro: qt5_wrap_ui or qt_wrap_ui from Widgets.
find_package(Qt${VTK_QT_VERSION} QUIET REQUIRED COMPONENTS ${qt_components})

foreach(_qt_comp IN LISTS qt_components)
    list(APPEND qt_modules "Qt${VTK_QT_VERSION}::${_qt_comp}")
endforeach()

message (STATUS "ITK Version: ${ITK_VERSION} VTK Version: ${VTK_VERSION}, QT Version: ${Qt${VTK_QT_VERSION}Widgets_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)
else()
    # Instruct CMake to run moc and uic automatically when needed.
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)

# For VTK versions greater than or equal to 8.90.0:
#  CMAKE_AUTOUIC is ON so we handle uic automatically for Qt targets.
#  CMAKE_AUTOMOC is ON so we handle moc automatically for Qt targets.
# However we have to do the following for VTK versions less than 8.90.0,
#  and we also assume, in this case, that Qt5 is being used.
if (VTK_VERSION VERSION_LESS "8.90.0")
    qt5_wrap_ui(UISrcs ${EXAMPLE}.ui)
endif()

# CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${QT_WRAP} MedXMainWindow.cpp MedXMainWindow.h QWidgets/CentralWidget.cpp QWidgets/CentralWidget.h QWidgets/CrossSectionalWidget.cpp QWidgets/CrossSectionalWidget.h QWidgets/VolumeRenderingWidget.cpp QWidgets/VolumeRenderingWidget.h Utils/LookupTableReader.cpp Utils/LookupTableReader.h Application.cpp MedXApplication.cpp MedXApplication.h Threads/MedicalImageReaderThread.cpp Threads/MedicalImageReaderThread.h Callbacks/ImageInteractionCallback.cpp Callbacks/ImageInteractionCallback.h Readers/ImageReader.h Readers/DICOMReader.cpp Readers/DICOMReader.h)
if (Qt${VTK_QT_VERSION}Widgets_VERSION VERSION_LESS "5.11.0")
    qt5_use_modules(${PROJECT_NAME} ${qt_components})
else()
    target_link_libraries(${PROJECT_NAME} ${qt_modules})
endif()
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES})