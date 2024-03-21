#pragma once

#include "PluginProcessor.h"

//==============================================================================
class MidiStrummerAudioProcessorEditor final : public juce::AudioProcessorEditor , public juce::Slider::Listener , public juce::Button::Listener
{
public:
    explicit MidiStrummerAudioProcessorEditor (MidiStrummerAudioProcessor&);
    ~MidiStrummerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:


    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void updateLabels();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiStrummerAudioProcessor& processorRef;
    juce::Slider strumDelaySlider;
    juce::ToggleButton syncToggle;
    juce::ComboBox div;
    bool isSynced;
    juce::Label bpmLabel;
    juce::Label timeSignatureLabel;
    juce::TextButton syncButton;
    juce::Label divLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiStrummerAudioProcessorEditor)
};
