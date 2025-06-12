#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# This file is a part of: LinaVG
# https://github.com/inanevin/LinaVG
# 
# Author: Inan Evin
# http://www.inanevin.com
# 
# The 2-Clause BSD License
# 
# Copyright (c) [2022-] Inan Evin
# 
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice, this
#       list of conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright notice,
#       this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.10...3.31)
project(vekt_example)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#--------------------------------------------------------------------
# Set sources
#--------------------------------------------------------------------

set(sources

main.cpp
glfw_window.cpp
gl_backend.cpp
app.cpp
)

set(headers
../src/vekt.hpp
../src/stb_truetype.h
glfw_window.hpp
gl_backend.hpp
app.cpp
)

#--------------------------------------------------------------------
# Create executable project
#--------------------------------------------------------------------

if(WIN32)
	add_executable(${PROJECT_NAME} ${sources} ${headers})
else()
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${sources} ${headers})
endif()

#--------------------------------------------------------------------
# Options & Definitions
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/../src)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

 if(MSVC)
   set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/)
 endif()
 
 if(APPLE)
 set_target_properties(
     ${PROJECT_NAME}
     PROPERTIES
     XCODE_GENERATE_SCHEME TRUE
     XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
     XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
 )
 
 endif()


add_subdirectory(glad)
add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)
set_property(TARGET glad PROPERTY FOLDER Dependencies)
set_property(TARGET glfw PROPERTY FOLDER Dependencies)

add_custom_command(
TARGET ${PROJECT_NAME}
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources/" "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/")

#--------------------------------------------------------------------
# Folder structuring in visual studio
#--------------------------------------------------------------------
if(MSVC_IDE)
	foreach(source IN LISTS headers sources)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${EXAMPLE_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
				source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()