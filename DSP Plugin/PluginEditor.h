#pragma once

#include "PluginProcessor.h"

//==============================================================================
class TimAudioProcessorEditor : public juce::AudioProcessorEditor
{
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TimAudioProcessor& processor_ref_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimAudioProcessorEditor)

public:
    explicit TimAudioProcessorEditor(TimAudioProcessor&);
    ~TimAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
};