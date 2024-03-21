#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiStrummerAudioProcessorEditor::MidiStrummerAudioProcessorEditor (MidiStrummerAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    strumDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    strumDelaySlider.setRange(0.0,1000.0,0.1);
    strumDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    strumDelaySlider.setPopupDisplayEnabled(true, false, this);
    strumDelaySlider.setValue(500.0f);
    strumDelaySlider.setTextValueSuffix("ms");
    strumDelaySlider.addListener(this);
    addAndMakeVisible(strumDelaySlider);
    setSize (400, 300);
}

MidiStrummerAudioProcessorEditor::~MidiStrummerAudioProcessorEditor()
{
}

//==============================================================================
void MidiStrummerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::red);
    g.setFont (15.0f);
    g.drawFittedText ("Hello Test!", getLocalBounds(), juce::Justification::topRight, 1);
}

void MidiStrummerAudioProcessorEditor::resized()
{
    strumDelaySlider.setBounds(getWidth()/2-50,getHeight()/2-50,100,100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MidiStrummerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

    if(slider == &strumDelaySlider)
    {
        processorRef.strumDelayMs = strumDelaySlider.getValue();
        //DBG("Strum Speed: " + juce::String(processorRef.strumSpeed));
    }

}
