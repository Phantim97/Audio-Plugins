#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include "AudioEngine.h"
#include "Oscilloscope.h"

//==============================================================================
class TimAudioProcessor  : public juce::AudioProcessor
{
private:
    //==============================================================================
    AudioEngine audio_engine_;
    juce::MidiMessageCollector midi_msg_collector_;
    AudioBufferQueue<float> audio_buffer_queue_;
    ScopeDataCollector<float> scope_data_collector_{audio_buffer_queue_};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimAudioProcessor)

public:
    //==============================================================================
    TimAudioProcessor();
    ~TimAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sample_rate, int samples_per_block) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

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
    void changeProgramName(int index, const juce::String& new_name) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& dest_data) override;
    void setStateInformation(const void* data, int size_in_bytes) override;

    //==============================================================================
    juce::MidiMessageCollector& get_midi_message_collector() noexcept { return midi_msg_collector_; }
    AudioBufferQueue<float>& get_audio_buffer_queue() noexcept { return audio_buffer_queue_; }
};