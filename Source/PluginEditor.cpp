#include "PluginEditor.h"
#include "PluginProcessor.h"

GabberKickGeneratorAudioProcessorEditor::GabberKickGeneratorAudioProcessorEditor(
    GabberKickGeneratorAudioProcessor& p)
    : AudioProcessorEditor(&p), proc(p)
{
    setSize(550, 320);

    auto cfg = [&](juce::Slider& s, juce::Label& l, const juce::String& txt)
    {
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 18);
        s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffcc5500));
        s.setColour(juce::Slider::thumbColourId,            juce::Colour(0xff33aa33));
        addAndMakeVisible(s);
        l.setText(txt, juce::dontSendNotification);
        l.setJustificationType(juce::Justification::centred);
        l.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        addAndMakeVisible(l);
    };
    cfg(pitchStart, lPitchStart, "PITCH START");
    cfg(pitchDecay, lPitchDecay, "PITCH DECAY");
    cfg(baseNote,   lBaseNote,   "BASE NOTE");
    cfg(ampDecay,   lAmpDecay,   "AMP DECAY");
    cfg(clickDecay, lClickDecay, "CLICK DECAY");
    cfg(clickMix,   lClickMix,   "CLICK MIX");

    lBody .setText("BODY & PITCH",     juce::dontSendNotification);
    lTrans.setText("TRANSIENT / CLICK", juce::dontSendNotification);
    for (auto* l : { &lBody, &lTrans })
    {
        l->setJustificationType(juce::Justification::centred);
        l->setColour(juce::Label::textColourId, juce::Colour(0xffcc5500));
        l->setFont(juce::Font(14.0f, juce::Font::bold));
        addAndMakeVisible(l);
    }

    // CLICK_TYPE: volgorde MOET gelijk zijn aan processor StringArray index
    clickType.addItem("White Noise Burst",     1);
    clickType.addItem("Pitch Saw Impuls",      2);
    clickType.addItem("Exponential Transient", 3);
    addAndMakeVisible(clickType);
    lClickType.setText("CLICK TYPE", juce::dontSendNotification);
    lClickType.setJustificationType(juce::Justification::centred);
    lClickType.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(lClickType);

    auto& apvts = proc.getApvts();
    aPitchStart = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "PITCH_START", pitchStart);
    aPitchDecay = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "PITCH_DECAY", pitchDecay);
    aBaseNote = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "BASE_NOTE", baseNote);
    aAmpDecay = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "AMP_DECAY", ampDecay);
    aClickDecay = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "CLICK_DECAY", clickDecay);
    aClickMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "CLICK_MIX", clickMix);
    aClickType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "CLICK_TYPE", clickType);
}

GabberKickGeneratorAudioProcessorEditor::~GabberKickGeneratorAudioProcessorEditor() = default;

void GabberKickGeneratorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff161616));
    g.setColour(juce::Colour(0xffcc5500));
    g.drawRect(getLocalBounds().reduced(2), 1);
    g.setColour(juce::Colour(0xff333333));
    g.drawVerticalLine(275, 34.0f, (float)getHeight() - 10);
}

void GabberKickGeneratorAudioProcessorEditor::resized()
{
    const int x0 = 10, top = 34;
    lBody .setBounds(x0,       8, 250, 20);
    lTrans.setBounds(x0 + 270, 8, 260, 20);

    // LINKS: BODY & PITCH (2x2) — label bovenaan elke cel
    lPitchStart.setBounds(x0,        top,      130, 14);
    pitchStart .setBounds(x0,        top + 14, 130, 96);
    lPitchDecay.setBounds(x0 + 130,  top,      130, 14);
    pitchDecay .setBounds(x0 + 130,  top + 14, 130, 96);
    lBaseNote  .setBounds(x0,        top + 116, 130, 14);
    baseNote   .setBounds(x0,        top + 130, 130, 96);
    lAmpDecay  .setBounds(x0 + 130,  top + 116, 130, 14);
    ampDecay   .setBounds(x0 + 130,  top + 130, 130, 96);

    // RECHTS: TRANSIENT / CLICK
    const int rx = x0 + 270; // 280
    lClickType.setBounds(rx, top,       260, 14);
    clickType  .setBounds(rx, top + 14, 260, 22);
    lClickDecay.setBounds(rx,        top + 52, 125, 14);
    clickDecay .setBounds(rx,        top + 66, 125, 96);
    lClickMix  .setBounds(rx + 135,  top + 52, 125, 14);
    clickMix   .setBounds(rx + 135,  top + 66, 125, 96);
}
