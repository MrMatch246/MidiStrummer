#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiStrummerAudioProcessorEditor::MidiStrummerAudioProcessorEditor(MidiStrummerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processorRef(p), valueTreeState (vts)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(strumDelaySlider);
    strumDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    strumDelaySlider.setRange(0.0f, 1000.0f, 0.1f);
    strumDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    strumDelaySlider.setPopupDisplayEnabled(true, false, this);
    strumDelaySlider.setValue(500.0f);
    strumDelaySlider.setTextValueSuffix("ms");
    // strumDelaySlider.addListener(this);
    strumDelayAttachment.reset(new SliderAttachment(valueTreeState, "strumDelayMs", strumDelaySlider));
    strumDelaySlider.setEnabled(false);



    addAndMakeVisible(bpmLabel);
    bpmLabel.setText("BPM: ", juce::dontSendNotification);

    addAndMakeVisible(timeSignatureLabel);
    timeSignatureLabel.setText("Time Signature:", juce::dontSendNotification);

    addAndMakeVisible(syncButton);
    syncButton.setClickingTogglesState(true);
    syncButton.setButtonText("Sync");
    syncButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    syncButton.setToggleState(true, juce::NotificationType::dontSendNotification); // Initial state
    syncButton.addListener(this);
    syncAttachment.reset(new ButtonAttachment(valueTreeState, "isSynced", syncButton));

    addAndMakeVisible(tripletButton);
    tripletButton.setClickingTogglesState(true);
    tripletButton.setButtonText("Normal");
    tripletButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    tripletButton.setToggleState(true, juce::NotificationType::dontSendNotification); // Initial state
    tripletButton.addListener(this);
    tripletAttachment.reset(new ButtonAttachment(valueTreeState, "isTriplet", tripletButton));


    addAndMakeVisible(timeSignatureComboBox);
    juce::StringArray choices = processorRef.choices(false);
    timeSignatureComboBox.clear();
    for (int i = 0; i < choices.size(); i++)
    {
        timeSignatureComboBox.addItem(choices[i], i + 1);
    }

    timeSignatureComboBox.setSelectedId(2);
    // timeSignatureComboBox.addListener(this);
    timeSignatureAttachment.reset(new ComboBoxAttachment(valueTreeState, "timeSignatureChoice", timeSignatureComboBox));



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
    syncButton.setBounds(row.removeFromRight(100));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    timeSignatureLabel.setBounds(row.removeFromLeft(100));
    tripletButton.setBounds(row.removeFromRight(100));
    row = area.removeFromTop(20);
    timeSignatureComboBox.setBounds(row.removeFromRight(100));
}

void MidiStrummerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &syncButton)
    {
        // Toggle button state has changed, handle the action here
        /bool syncEnabled = syncButton.getToggleState();
        strumDelaySlider.setEnabled(!syncEnabled);
        syncButton.setButtonText(syncEnabled ? "Sync" : "Free");
        this->updateLabels();
    }
    else if (button == &tripletButton)
    {
        bool tripletEnabled = tripletButton.getToggleState();
        tripletButton.setButtonText(tripletEnabled ? "Triplet" : "Normal");
        this->updateLabels();
    }
}

void MidiStrummerAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &timeSignatureComboBox)
    {
        processorRef.timeSignatureChoice->setValueNotifyingHost(timeSignatureComboBox.getSelectedId() - 1);
        this->updateLabels();
    }
}


void MidiStrummerAudioProcessorEditor::updateLabels()
{
    bpmLabel.setText("BPM:" + juce::String(processorRef.bpm), juce::dontSendNotification);

    timeSignatureLabel.setText("Time Signature: " + juce::String(processorRef.timeSig.numerator) + "/" + juce::String(processorRef.timeSig.denominator), juce::dontSendNotification);
    auto choice = processorRef.timeSignatureChoice->getIndex();
    timeSignatureComboBox.clear();
    juce::StringArray choices = processorRef.choices(tripletButton.getToggleState());
    for (int i = 0; i < choices.size(); i++)
    {
        timeSignatureComboBox.addItem(choices[i], i + 1);
    }
    timeSignatureComboBox.setSelectedId(choice + 1);

}