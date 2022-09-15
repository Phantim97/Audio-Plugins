cmake_minimum_required(VERSION 3.15)

project("Audio Plugins" LANGUAGES CXX)

# Here I will add Juce for all subprojects
include_directories($ENV{LIBS_PATH}/JUCE/build/install/include/JUCE-6.1.5/modules)
set(JUCE_DIR $ENV{LIBS_PATH}/JUCE/build/install/lib/cmake/JUCE-6.1.5)
find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system

# add_subdirectory("Basic Plugin")
add_subdirectory("DSP Plugin")