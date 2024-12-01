cmake_minimum_required(VERSION 3.14)

option(CTX_PLUGIN_BUILD "Build for plugin target" OFF)

project(cortex VERSION 0.1.0 LANGUAGES CXX)

if(CTX_PLUGIN_BUILD)
    add_subdirectory(../../vendor/juce)
    set(CTX_JUCE_SOURCE_DIR "../../vendor/juce/modules")
else()
    # If building with a plugin, do NOT the JUCE submodule from this library.
    add_subdirectory(vendor/juce)
    set(CTX_JUCE_SOURCE_DIR "vendor/juce/modules")
endif()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(cortex STATIC ${SRC_FILES})

set_target_properties(cortex PROPERTIES PUBLIC
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED
)

target_link_libraries(cortex
    PRIVATE
        juce::juce_gui_basics
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags)

target_include_directories(cortex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src PRIVATE ${CTX_JUCE_SOURCE_DIR})
