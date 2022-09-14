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
    setSize(400, 300);
}

TimAudioProcessorEditor::~TimAudioProcessorEditor()
{
}

//==============================================================================
void TimAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    std::string txt = "Testing: ";
    std::stringstream ss;
    ss << "Eyes on the prize";
    txt.append(ss.str());
    g.drawFittedText(txt , getLocalBounds(), juce::Justification::centred, 1);
}

void TimAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}