#pragma once

#include "PluginProcessor.h"

//==============================================================================
class MidiStrummerAudioProcessorEditor final : public juce::AudioProcessorEditor , public juce::Button::Listener , public juce::ComboBox::Listener
{
public:
    explicit MidiStrummerAudioProcessorEditor(MidiStrummerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~MidiStrummerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
private:


    // void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void updateLabels();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiStrummerAudioProcessor& processorRef;
    juce::Slider strumDelaySlider;
    bool isSynced{};
    juce::Label bpmLabel;
    juce::Label timeSignatureLabel;
    juce::Label versionLabel;
    juce::TextButton syncButton;
    juce::TextButton tripletButton;
    juce::TextButton strumDirectionButton;
    juce::ComboBox timeSignatureComboBox;
    juce::Label divLabel;

    juce::AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> strumDelayAttachment;
    std::unique_ptr<ButtonAttachment> syncAttachment;
    std::unique_ptr<ButtonAttachment> tripletAttachment;
    std::unique_ptr<ButtonAttachment> strumDirectionAttachment;
    std::unique_ptr<ComboBoxAttachment> timeSignatureAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiStrummerAudioProcessorEditor)
};
