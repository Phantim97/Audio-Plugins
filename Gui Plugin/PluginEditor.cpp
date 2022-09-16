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
    setSize(600, 400);
}

TimAudioProcessorEditor::~TimAudioProcessorEditor()
{
}

//==============================================================================
void TimAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::lightblue);
    g.setColour(juce::Colours::darkblue);

    //Setting a font
    const juce::Font main_comp_font("Times New Roman", 20.0f, juce::Font::bold | juce::Font::italic);
    g.setFont(main_comp_font);
    g.drawText("Gui Test", 20, 40, 200, 40, juce::Justification::centred, true);

    //Drawing lines
    g.setColour(juce::Colours::green);
    g.drawLine(10, 295, 590, 295, 9);

    //Drawing Rectangles
    g.setColour(juce::Colours::sandybrown);
    g.drawRect(300, 120, 200, 170);
    //g.fillRect(300, 120, 200, 170); //For filled rectangle

    const juce::Rectangle<float> house(300, 120, 200, 170);
    //g.setColour(juce::Colours::sandybrown); //takes juce::Rectangle<int>
    // g.fillRect(house);
    g.fillCheckerBoard(house, 30, 10, juce::Colours::sandybrown, juce::Colours::saddlebrown);

    //Roof
    g.setColour(juce::Colours::red);
    juce::Path roof;
    roof.addTriangle(300, 120, 500, 120, 400, 70);
    g.fillPath(roof);

    //Door
    g.fillRect(380, 240, 40, 50);



}

void TimAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}