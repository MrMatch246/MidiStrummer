#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiStrummerAudioProcessorEditor::MidiStrummerAudioProcessorEditor(MidiStrummerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processorRef(p), valueTreeState (vts)
{

    addAndMakeVisible(bpmLabel);
    bpmLabel.setText("BPM: ", juce::dontSendNotification);

    addAndMakeVisible(timeSignatureLabel);
    timeSignatureLabel.setText("DAW Signature:", juce::dontSendNotification);

    addAndMakeVisible(versionLabel);
    versionLabel.setText("Version: 1.2.0", juce::dontSendNotification);

    addAndMakeVisible(syncLabel);
    syncLabel.setText("Sync Mode", juce::dontSendNotification);

    addAndMakeVisible(tripletLabel);
    tripletLabel.setText("Triplet Mode", juce::dontSendNotification);

    addAndMakeVisible(strumDirectionLabel);
    strumDirectionLabel.setText("Strum Direction", juce::dontSendNotification);

    addAndMakeVisible(enforceOrderLabel);
    enforceOrderLabel.setText("Order Mode", juce::dontSendNotification);

    addAndMakeVisible(timeSignatureSelectionLabel);
    timeSignatureSelectionLabel.setText("Time Signature", juce::dontSendNotification);


    addAndMakeVisible(strumDelaySlider);
    strumDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    strumDelaySlider.setRange(0.0f, 1000.0f, 0.1f);
    strumDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    strumDelaySlider.setPopupDisplayEnabled(true, false, this);
    strumDelaySlider.setValue(250.0f);
    strumDelaySlider.setTextValueSuffix("ms");
    strumDelayAttachment.reset(new SliderAttachment(valueTreeState, "strumDelayMs", strumDelaySlider));
    strumDelaySlider.setEnabled(false);


    addAndMakeVisible(syncButton);
    syncButton.setClickingTogglesState(true);
    syncButton.setButtonText("Sync");
    setButtonColors(syncButton);
    syncButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    syncButton.addListener(this);
    syncAttachment.reset(new ButtonAttachment(valueTreeState, "isSynced", syncButton));


    addAndMakeVisible(tripletButton);
    tripletButton.setClickingTogglesState(true);
    tripletButton.setButtonText("Normal");
    setButtonColors(tripletButton);
    tripletButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    tripletButton.addListener(this);
    tripletAttachment.reset(new ButtonAttachment(valueTreeState, "isTriplet", tripletButton));


    addAndMakeVisible(enforceOrderButton);
    enforceOrderButton.setClickingTogglesState(true);
    enforceOrderButton.setButtonText("Unchanged");
    setButtonColors(enforceOrderButton);
    enforceOrderButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    enforceOrderButton.addListener(this);
    enforceOrderAttachment.reset(new ButtonAttachment(valueTreeState, "enforceOrder", enforceOrderButton));


    addAndMakeVisible(strumDirectionButton);
    strumDirectionButton.setClickingTogglesState(true);
    strumDirectionButton.setButtonText("Up");
    setButtonColors(strumDirectionButton);
    strumDirectionButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    strumDirectionButton.addListener(this);
    strumDirectionAttachment.reset(new ButtonAttachment(valueTreeState, "isStrummingUp", strumDirectionButton));


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
    strumDelaySlider.setBounds(getWidth() / 2 - 45, 20, 110, 110);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Position and size the components
    auto area = getLocalBounds().reduced(10);
    auto row = area.removeFromTop(20);

    syncLabel.setBounds(row.removeFromRight(90));
    bpmLabel.setBounds(row.removeFromLeft(90));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    syncButton.setBounds(row.removeFromRight(100));
    timeSignatureLabel.setBounds(row.removeFromLeft(120));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    tripletLabel.setBounds(row.removeFromRight(90));
    strumDirectionLabel.setBounds(row.removeFromLeft(100));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    tripletButton.setBounds(row.removeFromRight(100));
    strumDirectionButton.setBounds(row.removeFromLeft(100));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    timeSignatureSelectionLabel.setBounds(row.removeFromRight(100));
    enforceOrderLabel.setBounds(row.removeFromLeft(120));

    row = area.removeFromTop(3);
    row = area.removeFromTop(20);

    timeSignatureComboBox.setBounds(row.removeFromRight(100));
    enforceOrderButton.setBounds(row.removeFromLeft(100));

    versionLabel.setBounds(area.removeFromBottom(20));
}

void MidiStrummerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &syncButton)
    {
        // Toggle button state has changed, handle the action here
        bool syncEnabled = syncButton.getToggleState();
        strumDelaySlider.setEnabled(!syncEnabled);
        tripletButton.setEnabled(syncEnabled);
        timeSignatureComboBox.setEnabled(syncEnabled);
        syncButton.setButtonText(syncEnabled ? "Sync" : "Free");
    }
    else if (button == &tripletButton)
    {
        bool tripletEnabled = tripletButton.getToggleState();
        tripletButton.setButtonText(tripletEnabled ? "Triplet" : "Normal");
    }
    else if (button == &strumDirectionButton)
    {
        bool strumDirection = strumDirectionButton.getToggleState();
        strumDirectionButton.setButtonText(strumDirection ? "Up" : "Down");
    }
    else if (button == &enforceOrderButton)
    {
        bool enforceOrder = enforceOrderButton.getToggleState();
        enforceOrderButton.setButtonText(enforceOrder ? "Enforce Order" : "Unchanged");
    }
    this->updateLabels();
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

    timeSignatureLabel.setText("DAW Signature: " + juce::String(processorRef.timeSig.numerator) + "/" + juce::String(processorRef.timeSig.denominator), juce::dontSendNotification);
    auto choice = processorRef.timeSignatureChoice->getIndex();
    timeSignatureComboBox.clear();
    juce::StringArray choices = processorRef.choices(tripletButton.getToggleState());
    for (int i = 0; i < choices.size(); i++)
    {
        timeSignatureComboBox.addItem(choices[i], i + 1);
    }
    timeSignatureComboBox.setSelectedId(choice + 1);

}

void MidiStrummerAudioProcessorEditor::setButtonColors(juce::TextButton& button)
{
    button.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colours::orange);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
}
