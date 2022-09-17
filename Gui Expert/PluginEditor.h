#pragma once

#include "PluginProcessor.h"
#include "Components.h"
//==============================================================================
class TimAudioProcessorEditor : public juce::AudioProcessorEditor
{
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TimAudioProcessor& processor_ref_;
    MainPanel main_panel_;
    LeftPanel left_panel_;
    RightPanel right_panel_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimAudioProcessorEditor)

public:
    explicit TimAudioProcessorEditor(TimAudioProcessor&);
    ~TimAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
};