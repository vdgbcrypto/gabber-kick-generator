cmake_minimum_required(VERSION 3.22)
project(GabberKickGenerator VERSION 1.0.0)

set(JUCE_DIR "C:/JUCE" CACHE PATH "JUCE root")
add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/juce)

juce_add_plugin(GabberKickGenerator
    COMPANY_NAME             "vdgbcrypto"
    PLUGIN_MANUFACTURER_CODE Vdgb
    PLUGIN_CODE              GbKs
    FORMATS                  VST3
    IS_SYNTH                 TRUE
    NEEDS_MIDI_INPUT         TRUE
    NEEDS_MIDI_OUTPUT        FALSE
    IS_MIDI_EFFECT           FALSE
    VST3_CATEGORIES          "Synth|Generator"
    COPY_PLUGIN_AFTER_BUILD  FALSE
)

target_sources(GabberKickGenerator PRIVATE
    Source/PluginProcessor.cpp Source/PluginProcessor.h
    Source/PluginEditor.cpp    Source/PluginEditor.h)

target_compile_definitions(GabberKickGenerator PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(GabberKickGenerator PRIVATE
    juce::juce_dsp juce::juce_audio_utils)

target_compile_options(GabberKickGenerator PRIVATE
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2 /Oi /GL /fp:fast>
    $<$<AND:$<CONFIG:Release>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-O3 -ffast-math>)

# Offline DSP verification harness (no audio-thread, no GUI event loop)
add_executable(dsp_test
    dsp_test.cpp
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)
target_compile_definitions(dsp_test PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(dsp_test PRIVATE
    juce::juce_dsp juce::juce_audio_processors juce::juce_audio_basics juce::juce_gui_basics)
set_target_properties(dsp_test PROPERTIES WIN32_EXECUTABLE FALSE)

