#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GabberKickGeneratorAudioProcessor();
}

GabberKickGeneratorAudioProcessor::GabberKickGeneratorAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Out", juce::AudioChannelSet::stereo())),
      apvts(*this, nullptr, "PARAMS", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout
GabberKickGeneratorAudioProcessor::createParameterLayout()
{
    return {
        std::make_unique<juce::AudioParameterFloat>("PITCH_START", "Pitch Start",
            juce::NormalisableRange<float>(120.0f, 400.0f, 1.0f), 180.0f),
        std::make_unique<juce::AudioParameterFloat>("PITCH_DECAY", "Pitch Decay",
            juce::NormalisableRange<float>(1.0f, 8.0f, 0.1f), 2.0f),
        std::make_unique<juce::AudioParameterFloat>("BASE_NOTE", "Base Note",
            juce::NormalisableRange<float>(40.0f, 60.0f, 0.1f), 50.0f),
        std::make_unique<juce::AudioParameterFloat>("AMP_DECAY", "Amp Decay",
            juce::NormalisableRange<float>(120.0f, 400.0f, 1.0f), 200.0f),
        std::make_unique<juce::AudioParameterChoice>("CLICK_TYPE", "Click Type",
            juce::StringArray("White", "Impuls", "Curated"), 0),
        std::make_unique<juce::AudioParameterFloat>("CLICK_DECAY", "Click Decay",
            juce::NormalisableRange<float>(1.0f, 6.0f, 0.1f), 2.0f),
        std::make_unique<juce::AudioParameterFloat>("CLICK_MIX", "Click Mix",
            juce::NormalisableRange<float>(0.0f, 0.5f, 0.01f), 0.20f),
    };
}

juce::AudioProcessorEditor* GabberKickGeneratorAudioProcessor::createEditor()
{
    return new GabberKickGeneratorAudioProcessorEditor(*this);
}

void GabberKickGeneratorAudioProcessor::prepareToPlay(double sr, int)
{
    sampleRate = sr;
    osc.prepare({ sr, 1, 1 });
    osc.setFrequency(45.0f);
    pPitchStart = apvts.getRawParameterValue("PITCH_START");
    pPitchDecay = apvts.getRawParameterValue("PITCH_DECAY");
    pBaseNote   = apvts.getRawParameterValue("BASE_NOTE");
    pAmpDecay   = apvts.getRawParameterValue("AMP_DECAY");
    pClickType  = apvts.getRawParameterValue("CLICK_TYPE");
    pClickDecay = apvts.getRawParameterValue("CLICK_DECAY");
    pClickMix   = apvts.getRawParameterValue("CLICK_MIX");
}

void GabberKickGeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                     juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    const int ch = getTotalNumOutputChannels();
    buffer.clear();
    const float base = pBaseNote->load();

    // MIDI trigger (audio-thread): note-on herstart envelopen + phase
    for (const auto& m : midi)
    {
        const auto msg = m.getMessage();
        if (msg.isNoteOn())
        {
            currentPitch = pPitchStart->load();
            ampEnv = 1.0f;
            velocity = msg.getFloatVelocity();
            const float pDec = pPitchDecay->load() / 1000.0f;
            const float aDec = pAmpDecay->load()   / 1000.0f;
            const float cDec = pClickDecay->load() / 1000.0f;
            pitchCoef = (float)std::exp(-1.0 / (pDec * sampleRate));
            ampCoef   = (float)std::exp(-1.0 / (aDec * sampleRate));
            clickCoef = (float)std::exp(-1.0 / (cDec * sampleRate));
            clickEnv  = 1.0f;
            clickPhase = 0.0f;
            osc.reset();
        }
    }

    const int   cType    = (int)(pClickType->load() + 0.5f);
    const float clickMix = pClickMix->load();

    float one = 0.0f;
    for (int i = 0; i < n; ++i)
    {
        // Pitch-envelop: exp decay van start -> base
        currentPitch += (base - currentPitch) * pitchCoef;
        osc.setFrequency(currentPitch, true);
        one = 0.0f;
        float* chPtr = &one;
        juce::dsp::AudioBlock<float> blk(&chPtr, 1, 1);
        osc.process(juce::dsp::ProcessContextReplacing<float>(blk));

        ampEnv *= ampCoef;
        if (ampEnv < 1.0e-5f) ampEnv = 0.0f;

        // Click laag
        float clickSrc = 0.0f;
        if (cType == 1) {
            clickPhase += clickImpFreq / (float)sampleRate;
            if (clickPhase >= 1.0f) clickPhase -= 1.0f;
            clickSrc = clickPhase * 2.0f - 1.0f;
        } else {
            clickLcg = clickLcg * 1664525u + 1013904223u;
            clickSrc = (float)clickLcg / (float)0xFFFFFFFFu * 2.0f - 1.0f;
        }
        const float cEnv = (cType == 2) ? clickEnv * clickEnv : clickEnv;
        const float clickSig = clickSrc * cEnv * clickMix * 0.7f;
        clickEnv *= clickCoef;
        if (clickEnv < 1.0e-5f) clickEnv = 0.0f;

        const float s = one * ampEnv * velocity * 0.8f + clickSig;
        for (int c = 0; c < ch; ++c)
            buffer.setSample(c, i, s);
    }
}
