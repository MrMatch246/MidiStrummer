#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class MidiStrummerAudioProcessor final : public juce::AudioProcessor
{
public:
    juce::AudioParameterFloat* strumDelayMs;
    double bpm;
    juce::AudioParameterBool* isSynced;
    juce::AudioParameterChoice* timeSignatureChoice;
    juce::AudioPlayHead::TimeSignature timeSig { 4, 4 };

    enum timeSigEnum
    {
        x_1 = 1,
        x_2 = 2,
        x_3 = 3,
        x_4 = 4,
        x_8 = 8,
        x_16 = 16,
        x_32 = 32,
        x_64 = 64,
        x_128 = 128,
        x_256 = 256
    };

    //==============================================================================
    MidiStrummerAudioProcessor();
    ~MidiStrummerAudioProcessor() override;
    //==============================================================================

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    std::map<int, int> notes;
    juce::MidiBuffer preholdMidiBuffer;
    juce::AudioPlayHead *playHead;
    juce::AudioPlayHead::CurrentPositionInfo cpi;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiStrummerAudioProcessor)
};
