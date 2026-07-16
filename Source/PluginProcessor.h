#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <atomic>

class GabberKickGeneratorAudioProcessor : public juce::AudioProcessor
{
public:
    GabberKickGeneratorAudioProcessor();
    ~GabberKickGeneratorAudioProcessor() override = default;

    void prepareToPlay(double, int) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override;

    void getStateInformation(juce::MemoryBlock& dest) override
    {
        juce::MemoryOutputStream stream(dest, true);
        apvts.state.writeToStream(stream);
    }
    void setStateInformation(const void* data, int sizeInBytes) override
    {
        juce::ValueTree tree = juce::ValueTree::readFromData(data, (size_t)sizeInBytes);
        if (tree.isValid()) apvts.replaceState(tree);
    }
    const juce::String getName() const override { return "Gabber Kick Generator"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 5.0; } // covers max AMP_DECAY+click tail
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState& getApvts() { return apvts; }

private:
    juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

    // Envelope-state: audio-thread-only
    float currentPitch = 0.0f, ampEnv = 0.0f, velocity = 0.0f;
    std::atomic<double> sampleRate{44100.0}; // prepareToPlay(msg-thread) -> processBlock(audio)
    float pitchCoef = 0.999f, ampCoef = 0.999f;

    // Click/transient layer
    juce::uint32 clickLcg = 12345u;
    float clickEnv = 0.0f;
    float clickPhase = 0.0f;
    float clickCoef = 0.999f;
    static constexpr float clickImpFreq = 1500.0f;

    // Param-pointers (APVTS raw = std::atomic<float>, UI->audio lock-free)
    std::atomic<float>* pPitchStart = nullptr;
    std::atomic<float>* pPitchDecay = nullptr;
    std::atomic<float>* pBaseNote   = nullptr;
    std::atomic<float>* pAmpDecay   = nullptr;
    std::atomic<float>* pClickType  = nullptr;
    std::atomic<float>* pClickDecay = nullptr;
    std::atomic<float>* pClickMix   = nullptr;
};
