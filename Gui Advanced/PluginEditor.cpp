#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <sstream>
#include <string>

//==============================================================================
TimAudioProcessorEditor::TimAudioProcessorEditor(TimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor_ref_ (p)
{
    juce::ignoreUnused (processor_ref_);
    setSize(rc_.getLocalBounds().getWidth(), rc_.getLocalBounds().getHeight());
    addAndMakeVisible(rc_);
    setResizable(true, true);
}

TimAudioProcessorEditor::~TimAudioProcessorEditor()
{
}

//==============================================================================
void TimAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void TimAudioProcessorEditor::resized()
{
    rc_.setSize(getLocalBounds().getWidth(), getLocalBounds().getHeight());
}