#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiStrummerAudioProcessorEditor::MidiStrummerAudioProcessorEditor(MidiStrummerAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    strumDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    strumDelaySlider.setRange(0.0, 1000.0, 0.1);
    strumDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    strumDelaySlider.setPopupDisplayEnabled(true, false, this);
    strumDelaySlider.setValue(500.0f);
    strumDelaySlider.setTextValueSuffix("ms");
    strumDelaySlider.addListener(this);

    addAndMakeVisible(bpmLabel);
    bpmLabel.setText("BPM:ssa ", juce::dontSendNotification);


    // Time Signature Label
    addAndMakeVisible(timeSignatureLabel);
    timeSignatureLabel.setText("Time Signature:", juce::dontSendNotification);

    syncButton.setClickingTogglesState (true);
    syncButton.setButtonText("Sync");
    syncButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    syncButton.setToggleState(true, juce::NotificationType::dontSendNotification); // Initial state
    syncButton.addListener(this);
    addAndMakeVisible(syncButton);


    addAndMakeVisible(strumDelaySlider);
    setSize(400, 300);
}

MidiStrummerAudioProcessorEditor::~MidiStrummerAudioProcessorEditor()
{
}

//==============================================================================
void MidiStrummerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

}

void MidiStrummerAudioProcessorEditor::resized()
{
    strumDelaySlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Position and size the components
    auto area = getLocalBounds().reduced(10);
    auto row = area.removeFromTop(20);
    bpmLabel.setBounds(row.removeFromLeft(50));
    syncButton.setBounds(row.removeFromRight(50));

    row = area.removeFromTop(20);
    timeSignatureLabel.setBounds(row.removeFromLeft(100));
}

void MidiStrummerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &syncButton)
    {
        // Toggle button state has changed, handle the action here
        bool syncEnabled = syncButton.getToggleState();
        processorRef.isSynced->setValueNotifyingHost(syncEnabled);
        syncButton.setButtonText(syncEnabled ? "Sync" : "Free");
        this->updateLabels();
    }
}

void MidiStrummerAudioProcessorEditor::updateLabels()
{
    bpmLabel.setText("BPM:" + juce::String(processorRef.bpm), juce::dontSendNotification);
    timeSignatureLabel.setText("Time Signature: " + juce::String(processorRef.timeSig.numerator) + "/" + juce::String(processorRef.timeSig.denominator), juce::dontSendNotification);
}

void MidiStrummerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &strumDelaySlider)
    {
        processorRef.strumDelayMs->setValueNotifyingHost(strumDelaySlider.getValue());
        this->updateLabels();
        DBG("BPM: " + juce::String(processorRef.bpm));
        //DBG("Strum Speed: " + juce::String(processorRef.strumSpeed));
    }
}

