#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <sstream>
#include <string>

//==============================================================================
TimAudioProcessorEditor::TimAudioProcessorEditor(TimAudioProcessor& p)
    : AudioProcessorEditor(&p), processor_ref_(p), main_panel_(juce::Colours::lightcoral),
	left_panel_(juce::Colours::lightgreen),right_panel_(juce::Colours::lightcyan)
{
    juce::ignoreUnused (processor_ref_);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(right_panel_);
    addAndMakeVisible(left_panel_);
    addAndMakeVisible(main_panel_);

    setSize(600, 400);
}

TimAudioProcessorEditor::~TimAudioProcessorEditor()
{
}

//==============================================================================
void TimAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void TimAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)), Track(Fr(2)), Track(Fr(3)) };

    grid.items = { juce::GridItem(left_panel_), juce::GridItem(main_panel_), juce::GridItem(right_panel_) };

    grid.performLayout(getLocalBounds());
}