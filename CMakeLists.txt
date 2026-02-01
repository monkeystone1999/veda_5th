cmake_minimum_required(VERSION 3.20)
project(v5 VERSION 5.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all source files
file(GLOB_RECURSE V5_SOURCES 
    "src/crypt/*.cpp"
    "src/db/*.cpp"
    "src/network/*.cpp"
    "src/packet/*.cpp"
    "src/thread_pool/*.cpp"
)

# Create shared library or interface library if no sources
if(V5_SOURCES)
    add_library(v5 SHARED ${V5_SOURCES})
    
    # Set library properties (only for non-INTERFACE)
    set_target_properties(v5 PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 5
        PUBLIC_HEADER "v5.hpp"
    )
    
    # Include directories
    target_include_directories(v5 
        PUBLIC 
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    
    # Installation rules
    install(TARGETS v5
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
    )
else()
    # If no sources, create interface library for header-only
    add_library(v5 INTERFACE)
    
    # Include directories for INTERFACE library
    target_include_directories(v5 INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )
    
    # Installation rules for INTERFACE library
    install(TARGETS v5
        INCLUDES DESTINATION include
    )
    install(FILES v5.hpp DESTINATION include)
endif()
