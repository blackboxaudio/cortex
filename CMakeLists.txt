cmake_minimum_required(VERSION 3.22)

option(CTX_PLUGIN_BUILD "Build for plugin target" OFF)

project(cortex VERSION 0.1.0 LANGUAGES C CXX)

if(CTX_PLUGIN_BUILD)
    # CAUTION: This assumes CMake is being run from plugin directory
    # with JUCE modules in the vendor directory.
#    add_subdirectory("../../vendor/juce" "../../bin/lib")
    set(CTX_JUCE_SOURCE_DIR "../../vendor/juce/modules")
else()
    # CAUTION: Only use this project's JUCE submodule if NOT working on a
    # plugin project.
    add_subdirectory("vendor/juce")
    set(CTX_JUCE_SOURCE_DIR "vendor/juce/modules")
endif()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(cortex STATIC ${SRC_FILES})

set_target_properties(cortex PROPERTIES
    PUBLIC
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED
        CXX_VISIBILITY_PRESET hidden
        OSX_ARCHITECTURES "x86_64;arm64")

target_link_libraries(cortex
    PRIVATE
        juce::juce_gui_basics
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags)

target_include_directories(cortex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src PRIVATE ${CTX_JUCE_SOURCE_DIR})
