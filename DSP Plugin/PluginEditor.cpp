#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <sstream>
#include <string>

//==============================================================================
TimAudioProcessorEditor::TimAudioProcessorEditor(TimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor_ref_ (p),
	scope_component_(processor_ref_.get_audio_buffer_queue())
{
    addAndMakeVisible(midi_keyboard_component_);
    addAndMakeVisible(scope_component_);

	setSize(400, 300);

    const juce::Rectangle<int> area =  getLocalBounds();

    scope_component_.setTopLeftPosition(0, 80);
    scope_component_.setSize(area.getWidth(), area.getHeight() - 100);

    midi_keyboard_component_.setMidiChannel(2);
    midi_keyboard_state_.addListener(&processor_ref_.get_midi_message_collector());
}

TimAudioProcessorEditor::~TimAudioProcessorEditor()
{
    midi_keyboard_state_.removeListener(&processor_ref_.get_midi_message_collector());
}

//==============================================================================
void TimAudioProcessorEditor::paint(juce::Graphics& g)
{
    //Scan the context background to fill the window color
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void TimAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();
    midi_keyboard_component_.setBounds(area.removeFromTop(80).reduced(8));
}