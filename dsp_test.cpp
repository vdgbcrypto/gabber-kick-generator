#include "Source/PluginProcessor.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>
#include <iostream>
#include <vector>

static double peakAmp(const juce::AudioBuffer<float>& b)
{
    float mx = 0.0f;
    for (int c = 0; c < b.getNumChannels(); ++c)
        for (int i = 0; i < b.getNumSamples(); ++i)
        { const float s = std::fabs(b.getSample(c, i)); if (s > mx) mx = s; }
    return (double)mx;
}

int main()
{
    const double sr = 44100.0;
    const int block = 512;
    bool ok = true;

    // ---- 1. No-trigger silence (synth must be silent without note-on) ----
    {
        auto* proc = dynamic_cast<GabberKickGeneratorAudioProcessor*>(createPluginFilter());
        proc->prepareToPlay(sr, block);
        juce::AudioBuffer<float> buf(2, block);
        juce::MidiBuffer midi;
        proc->processBlock(buf, midi);
        const double r = peakAmp(buf);
        std::cout << "[T1] no-trigger RMS = " << r << "\n";
        if (r > 1.0e-6) { std::cerr << "  FAIL: sound without MIDI\n"; ok = false; }
        delete proc;
    }

    // ---- 2. Trigger + decay tau-fit ----
    auto* proc = dynamic_cast<GabberKickGeneratorAudioProcessor*>(createPluginFilter());
    proc->prepareToPlay(sr, block);
    juce::AudioBuffer<float> buf(2, block);
    juce::MidiBuffer midi;

    midi.addEvent(juce::MidiMessage::noteOn(1, 36, 1.0f), 0);
    proc->processBlock(buf, midi);
    const double peak = peakAmp(buf);
    std::cout << "[T2] note-on peak RMS = " << peak << "\n";
    if (peak < 0.01) { std::cerr << "  FAIL: no output on note-on\n"; ok = false; }

    midi.clear();
    // isolate deterministic amp/pitch envelope from the stochastic CLICK layer:
    // set CLICK_MIX=0 so T3 measures the kick body only (click adds random RMS noise)
    {
        auto& apvts = proc->getApvts();
        auto* pMix = apvts.getParameter("CLICK_MIX");
        pMix->setValueNotifyingHost(pMix->getValueForText("0.0"));
    }
    std::vector<double> series;
    double prev = peak;
    bool monotonic = true;
    // skip the first ~3 blocks (pitch-glide region: energy shifts mid->low, RMS may tick up)
    const int glideSkp = 3;
    for (int b = 0; b < 40; ++b)
    {
        proc->processBlock(buf, midi);
        const double r = peakAmp(buf);
        // below the audible floor the exp tail is denormal/rounding noise (<1e-3);
        // only enforce monotonicity in the meaningful-signal region
        if (b >= glideSkp && r > 1.0e-3 && r > prev + 1.0e-4) monotonic = false;
        prev = r;
        series.push_back(r);
    }
    std::cout << "  [diag] RMS series:";
    for (size_t k = 0; k < series.size(); ++k) std::cout << " " << series[k];
    std::cout << "\n";
    std::cout << "[T3] monotonic decay = " << (monotonic ? "yes" : "no") << "\n";
    if (!monotonic) { std::cerr << "  FAIL: non-monotonic decay\n"; ok = false; }

    // fit: rms[k] = A * e^(-k * d)  ->  ln(rms) = lnA - d*k  (linear least squares)
    double Sx=0,Sy=0,Sxx=0,Sxy=0,N=0;
    for (size_t k=0;k<series.size();++k){ const double y=std::log(series[k]+1e-12);
        Sx+=k; Sy+=y; Sxx+=k*k; Sxy+=k*y; ++N; }
    const double d = (N*Sxy - Sx*Sy)/(N*Sxx - Sx*Sx);   // per-block decay
    const double perSample = d / block;
    const double tau = -1.0 / (perSample * sr);           // seconds
    std::cout << "[T4] fitted AMP_DECAY tau = " << (tau*1000.0) << " ms (target 200.0)\n";
    if (std::fabs(tau*1000.0 - 200.0) > 35.0) { std::cerr << "  FAIL: tau off\n"; ok = false; }

    // ---- 3. Param change: BASE_NOTE via APVTS ----
    {
        auto& apvts = proc->getApvts();
        auto* pBase = apvts.getParameter("BASE_NOTE");
        pBase->setValueNotifyingHost(pBase->getValueForText("55.0"));
        const float v = *apvts.getRawParameterValue("BASE_NOTE");
        std::cout << "[T5] BASE_NOTE set = " << v << " Hz\n";
        if (std::fabs(v - 55.0f) > 0.5f) { std::cerr << "  FAIL: param set\n"; ok = false; }
    }

    delete proc;
    std::cout << (ok ? "RESULT: PASS\n" : "RESULT: FAIL\n");
    return ok ? 0 : 1;
}
