#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <sstream>
#include <string>

//==============================================================================
TimAudioProcessorEditor::TimAudioProcessorEditor(TimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor_ref_ (p)
{
    juce::ignoreUnused (processor_ref_);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(rc_.getLocalBounds().getWidth(), rc_.getLocalBounds().getHeight());
    addAndMakeVisible(rc_);
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}