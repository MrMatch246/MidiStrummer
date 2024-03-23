#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class MidiStrummerAudioProcessor final : public juce::AudioProcessor
{
public:
    juce::AudioParameterFloat* strumDelayMs;
    double bpm{};
    juce::AudioParameterBool* isSynced;
    juce::AudioParameterBool* isTriplet;
    juce::AudioParameterBool* isStrummingUp;
    juce::AudioParameterBool* enforceOrder;
    juce::AudioParameterChoice* timeSignatureChoice;
    juce::AudioPlayHead::TimeSignature timeSig{4, 4};
    const juce::StringArray choices(bool triplet = false);


    //==============================================================================
    MidiStrummerAudioProcessor();
    ~MidiStrummerAudioProcessor() override;
    //==============================================================================

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    std::vector<int> timeSigList =
    {
        1,
        2,
        4,
        8,
        16,
        32,
        64,
        128,
        256
    };
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;


    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    double timePerBeat(int bpm, int x, int y, bool triplet);

    juce::ValueTree getParameters() { return parameters.copyState(); }


private:
    std::map<int, int> preholdNotes;
    juce::MidiBuffer preholdMidiBuffer;
    juce::AudioPlayHead* playHead{};
    juce::AudioPlayHead::CurrentPositionInfo cpi;
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* strumDelayParameter = nullptr;
    std::atomic<float>* isSyncedParameter = nullptr;
    std::atomic<float>* isTripletParameter = nullptr;
    std::atomic<int>* timeSignatureParameter = nullptr;
    std::atomic<float>* isStrummingUpParameter = nullptr;
    std::atomic<float>* enforceOrderParameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiStrummerAudioProcessor)
};
