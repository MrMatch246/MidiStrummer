#pragma once

#include "PluginProcessor.h"

//==============================================================================
class MidiStrummerAudioProcessorEditor final : public juce::AudioProcessorEditor , public juce::Slider::Listener
{
public:
    explicit MidiStrummerAudioProcessorEditor (MidiStrummerAudioProcessor&);
    ~MidiStrummerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiStrummerAudioProcessor& processorRef;
    juce::Slider strumDelaySlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiStrummerAudioProcessorEditor)
};
