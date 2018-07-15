#required on windows to link to qtmain.lib
cmake_minimum_required(VERSION 2.8.11)

set(PROJECT_NAME Log_Analyzer)

project(${PROJECT_NAME})

#setup c++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_OPTIONS -o inc/)

#Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5gui REQUIRED)

#dependencies
add_subdirectory(deps/QtOutputManager)

#configure output directories
if(WIN32)
  if(CMAKE_CL_64)
    set(ARCH "64")
  else(CMAKE_CL_64)
    set(ARCH "32")
  endif(CMAKE_CL_64)
else(WIN32)
  string(REGEX MATCH ".*[86|32]$" matches "${CMAKE_SYSTEM_PROCESSOR}")
  if(matches)
    set(ARCH "32")
  else(matches)
    set(ARCH "64")
  endif(matches)
endif(WIN32)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}${ARCH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}${ARCH}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}${ARCH}/lib)

set(src src/main.cpp
        src/DsLogReader.cpp
        src/BinaryReader.cpp
        )
        
set(inc inc/DsLogReader.h
        inc/BinaryReader.h
        )
        
set(forms 
          )

#setup inc as a valid include directory
include_directories(${CMAKE_SOURCE_DIR}/inc
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR}/deps/QtOutputManager/inc
                    )
                    
#create executable
add_executable(${PROJECT_NAME} ${src} ${inc} ${forms})

#use the widgets moduole from Qt 5
target_link_libraries(${PROJECT_NAME} OutputManager_Qt
                      Qt5::Widgets
                      Qt5::Gui)
