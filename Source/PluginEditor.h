#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class GabberKickGeneratorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    GabberKickGeneratorAudioProcessorEditor(GabberKickGeneratorAudioProcessor&);
    ~GabberKickGeneratorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    GabberKickGeneratorAudioProcessor& proc;

    juce::Slider pitchStart, pitchDecay, baseNote, ampDecay;
    juce::Slider clickDecay, clickMix;
    juce::ComboBox clickType;

    juce::Label lBody, lTrans;
    juce::Label lPitchStart, lPitchDecay, lBaseNote, lAmpDecay;
    juce::Label lClickType, lClickDecay, lClickMix;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        aPitchStart, aPitchDecay, aBaseNote, aAmpDecay, aClickDecay, aClickMix;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> aClickType;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        GabberKickGeneratorAudioProcessorEditor)
};
