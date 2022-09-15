#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TimAudioProcessor::TimAudioProcessor():
	AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

TimAudioProcessor::~TimAudioProcessor()
{
}

//==============================================================================
const juce::String TimAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TimAudioProcessor::acceptsMidi() const
{
    return true;
}

bool TimAudioProcessor::producesMidi() const
{
    return false;
}

bool TimAudioProcessor::isMidiEffect() const
{
    return false;
}

double TimAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TimAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TimAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TimAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String TimAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused (index);
    return {};
}

void TimAudioProcessor::changeProgramName(int index, const juce::String& new_name)
{
    juce::ignoreUnused(index, new_name);
}

//==============================================================================
void TimAudioProcessor::prepareToPlay(const double sample_rate, const int samples_per_block)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    audio_engine_.prepare({ sample_rate, static_cast<juce::uint32>(samples_per_block), 2 });
    midi_msg_collector_.reset(sample_rate);
}

void TimAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool TimAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    {
        return false;
    }

    return true;
}

void TimAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi_messages)
{
    juce::ScopedNoDenormals no_denormals;
    const int total_num_input_channels = getTotalNumInputChannels();
    const int total_num_output_channels = getTotalNumOutputChannels();

    midi_msg_collector_.removeNextBlockOfMessages(midi_messages, buffer.getNumSamples());

    for (int i = total_num_input_channels; i < total_num_output_channels; i++)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    //Render to Audio Engine
    audio_engine_.renderNextBlock(buffer, midi_messages, 0, buffer.getNumSamples());

    //Render to Oscilloscope
    scope_data_collector_.process(buffer.getReadPointer(0), buffer.getNumSamples());
}

//==============================================================================
bool TimAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TimAudioProcessor::createEditor()
{
    return new TimAudioProcessorEditor(*this);
}

//==============================================================================
void TimAudioProcessor::getStateInformation(juce::MemoryBlock& dest_data)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (dest_data);
}

void TimAudioProcessor::setStateInformation(const void* data, int size_in_bytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, size_in_bytes);
}

//==============================================================================
// This creates new instances of the plugin. Function name must stay the same
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TimAudioProcessor();
}