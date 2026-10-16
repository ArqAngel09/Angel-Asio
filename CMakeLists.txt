cmake_minimum_required(VERSION 3.24)
project(UltraASIOLab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.6
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(UltraASIOLab
  PRODUCT_NAME "UltraASIO Lab"
  COMPANY_NAME "Angel Ortiz Audio Lab"
  BUNDLE_ID "com.angelortiz.ultraasiolab"
)

target_sources(UltraASIOLab PRIVATE
  Source/Main.cpp
  Source/MainComponent.cpp
  Source/MainComponent.h
)

target_compile_definitions(UltraASIOLab PRIVATE
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:UltraASIOLab,JUCE_PRODUCT_NAME>"
  JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
)

target_link_libraries(UltraASIOLab PRIVATE
  juce::juce_gui_extra
  juce::juce_audio_utils
  juce::juce_recommended_config_flags
  juce::juce_recommended_warning_flags
)
