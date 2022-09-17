#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class RectComp : public juce::Component
{
private:
	juce::TextButton header_;
	juce::TextButton sidebar_;

	juce::TextButton lime_content_;
	juce::TextButton grapefruit_content_;
	juce::TextButton lemon_content_;
	juce::TextButton orange_content_;
	juce::TextButton footer_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RectComp);
public:
	RectComp()
	{
		header_.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
		header_.setButtonText("Header");
		addAndMakeVisible(header_);

		footer_.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
		footer_.setButtonText("Footer");
		addAndMakeVisible(footer_);

		sidebar_.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
		sidebar_.setButtonText("Sidebar");
		addAndMakeVisible(sidebar_);

		lime_content_.setColour(juce::TextButton::buttonColourId, juce::Colours::lime);
		addAndMakeVisible(lime_content_);

		grapefruit_content_.setColour(juce::TextButton::buttonColourId, juce::Colours::yellowgreen);
		addAndMakeVisible(grapefruit_content_);

		lemon_content_.setColour(juce::TextButton::buttonColourId, juce::Colours::yellow);
		addAndMakeVisible(lemon_content_);

		orange_content_.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
		addAndMakeVisible(orange_content_);

		setSize(400, 400);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::darkgrey);
	}

	void resized() override
	{
		juce::Rectangle<int> area = getLocalBounds();

		constexpr int header_footer_height = 36;
		header_.setBounds(area.removeFromTop(header_footer_height));
		footer_.setBounds(area.removeFromBottom(header_footer_height));

		constexpr int sidebar_width = 80;
		sidebar_.setBounds(area.removeFromLeft(sidebar_width));

		constexpr int content_item_height = 24;
		lime_content_.setBounds(area.removeFromTop(content_item_height));
		grapefruit_content_.setBounds(area.removeFromTop(content_item_height));
		lemon_content_.setBounds(area.removeFromTop(content_item_height));
		orange_content_.setBounds(area.removeFromTop(content_item_height));
	}
};