#pragma once

#include "PluginProcessor.h"
#include <juce_audio_utils/juce_audio_utils.h>

//==============================================================================
class TimAudioProcessorEditor : public juce::AudioProcessorEditor
{
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TimAudioProcessor& processor_ref_;

    juce::MidiKeyboardState midi_keyboard_state_;
    juce::MidiKeyboardComponent midi_keyboard_component_ {midi_keyboard_state_, juce::MidiKeyboardComponent::horizontalKeyboard };
    ScopeComponent<float> scope_component_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimAudioProcessorEditor)

public:
    explicit TimAudioProcessorEditor(TimAudioProcessor&);
    ~TimAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
};