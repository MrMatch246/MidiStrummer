#include "PluginProcessor.h"

//#include <D2DBaseTypes.h>

#include "PluginEditor.h"

//==============================================================================
MidiStrummerAudioProcessor::MidiStrummerAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) , parameters(*this, nullptr, juce::Identifier("ValueTreeState"),
                    {
                        std::make_unique<juce::AudioParameterFloat>("strumDelayMs", "Strum Delay Time", 0.0f, 1000.0f, 250.0f),
                        std::make_unique<juce::AudioParameterBool>("isSynced", "Sync Mode", true),
                        std::make_unique<juce::AudioParameterChoice>("timeSignatureChoice", "Time Signature", choices(), 2),
                        std::make_unique<juce::AudioParameterBool>("isTriplet", "Triplet", false),
                        std::make_unique<juce::AudioParameterBool>("isStrummingUp", "Strumming Direction", true),
                        std::make_unique<juce::AudioParameterBool>("enforceOrder", "Order Mode", false)
                    }
          )
{
    strumDelayParameter = parameters.getRawParameterValue("strumDelayMs");
    isSyncedParameter = parameters.getRawParameterValue("isSynced");
    isTripletParameter = parameters.getRawParameterValue("isTriplet");
    timeSignatureChoice = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("timeSignatureChoice"));
    isStrummingUpParameter = parameters.getRawParameterValue("isStrummingUp");
    enforceOrderParameter = parameters.getRawParameterValue("enforceOrder");
}

MidiStrummerAudioProcessor::~MidiStrummerAudioProcessor()
{
}

//==============================================================================
const juce::String MidiStrummerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiStrummerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MidiStrummerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MidiStrummerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MidiStrummerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiStrummerAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MidiStrummerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiStrummerAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MidiStrummerAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MidiStrummerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void MidiStrummerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiStrummerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MidiStrummerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void MidiStrummerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{



    const int numSamples = buffer.getNumSamples();
    const auto sampleRate = getSampleRate();

    bpm = getPlayHead()->getPosition()->getBpm().orFallback(120);
    timeSig = *getPlayHead()->getPosition()->getTimeSignature();


    int strumSampleDelay = 0;
    if (isSyncedParameter->load())
    {
        int numerator =  timeSig.numerator == 0 ? 4 : timeSig.numerator;
        int denominator = timeSigList[timeSignatureChoice->getIndex()];
        strumSampleDelay = static_cast<int>(timePerBeat(bpm, numerator, denominator, isTripletParameter->load()) * sampleRate / 1000.0f);
    }
    else
        strumSampleDelay = static_cast<int>(*strumDelayParameter * sampleRate / 1000.0f);

    int strumSampleOffset = 0;
    int eventsAhead = midiMessages.getNumEvents();


    // if (eventsAhead > 0)
    // {
    //     DBG("Midi Events: " + juce::String(eventsAhead));
    //     DBG("BPM: " + juce::String(bpm) + " TimeSig: " + juce::String(timeSig.numerator) + "/" +
    //         juce::String(timeSig.denominator));
    // }

    juce::MidiBuffer newPreholdMidiBuffer;
    juce::MidiBuffer processedMidi;
    juce::SortedSet<int> notes;
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            notes.add(msg.getNoteNumber());
        }
    }


    bool onNoteSeen = false;


    for (const auto metadata : midiMessages)
    {
        eventsAhead--;
        const auto msg = metadata.getMessage();
        const auto NoteNumber = msg.getNoteNumber();
        const auto originalSamplePosition = metadata.samplePosition;
        if (msg.isNoteOff())
        {
            if (preholdNotes.count(NoteNumber) == 1 && preholdNotes[NoteNumber] >= originalSamplePosition)
            {
                preholdNotes.erase(NoteNumber);
            }
            else
            {
                processedMidi.addEvent(msg, originalSamplePosition);
            }
        }
        else
        {

            auto targetSamplePosition = -1;
            if (enforceOrderParameter->load())
            {
                int strumOffset = 0;
                if (!isStrummingUpParameter->load())
                    strumOffset = (notes.size() - notes.indexOf(NoteNumber) - 1) * strumSampleDelay;
                else
                    strumOffset = notes.indexOf(NoteNumber) * strumSampleDelay;
                targetSamplePosition = originalSamplePosition + strumOffset;
            }
            else
            {
                if (onNoteSeen == false)
                {
                    onNoteSeen = true;
                    if (isStrummingUpParameter->load())
                        strumSampleOffset = 0;
                    else
                        strumSampleOffset = strumSampleDelay * (eventsAhead);
                }
                targetSamplePosition = originalSamplePosition + strumSampleOffset;
            }



            const auto newTargetPosition = targetSamplePosition - numSamples;
            if (targetSamplePosition <= numSamples)
            {
                processedMidi.addEvent(msg, targetSamplePosition);
                preholdNotes.erase(NoteNumber);
            }
            else
            {
                preholdNotes[NoteNumber] = newTargetPosition;
                newPreholdMidiBuffer.addEvent(msg, newTargetPosition);
            }


            if(!enforceOrderParameter->load())
            {
                if (isStrummingUpParameter->load())
                    strumSampleOffset += strumSampleDelay;
                else
                    strumSampleOffset -= strumSampleDelay;
            }

        }
    }


    for (const auto metadata : preholdMidiBuffer)
    {
        const auto msg = metadata.getMessage();
        const int NoteNumber = msg.getNoteNumber();
        const int currentTargetSPos = metadata.samplePosition;
        const int newTargetSPos = currentTargetSPos - numSamples;

        if (currentTargetSPos <= numSamples)
        {
            if (preholdNotes.erase(NoteNumber) == 1)
                processedMidi.addEvent(msg, currentTargetSPos);
        }
        else if (preholdNotes.count(NoteNumber) == 1 && currentTargetSPos > numSamples)
        {
            if (preholdNotes[NoteNumber] == currentTargetSPos)
            {
                preholdNotes[NoteNumber] = newTargetSPos;
                newPreholdMidiBuffer.addEvent(msg, newTargetSPos);
            }
        }
    }

    preholdMidiBuffer.swapWith(newPreholdMidiBuffer);
    midiMessages.swapWith(processedMidi);
}

//==============================================================================
bool MidiStrummerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiStrummerAudioProcessor::createEditor()
{
    return new MidiStrummerAudioProcessorEditor(*this , parameters);
}



//==============================================================================
void MidiStrummerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MidiStrummerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

double MidiStrummerAudioProcessor::timePerBeat(int bpm, int x, int y, bool triplet = false) {
    double factor = triplet ? 2.0 / 3.0 : 1.0; // For triplet rhythm, adjust factor
    double divisor = triplet ? 2.0 : 1.0; // For triplet rhythm, use 2 BPM

    return 60000.0 / (bpm * divisor * y) * x * factor;
}

const juce::StringArray MidiStrummerAudioProcessor::choices(bool triplet)
{
    juce::StringArray choices = {};
    for (auto denumerator : timeSigList)
    {
        choices.add("1/"+juce::String(denumerator) + (triplet ? "T" : ""));
    }
    return choices;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiStrummerAudioProcessor();
}
