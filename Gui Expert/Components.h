#pragma once
#include <juce_audio_devices/juce_audio_devices.h>

class MainPanel : public juce::Component
{
private:
    juce::OwnedArray<juce::Slider> sliders_;
    juce::Colour bg_;
public:
    explicit MainPanel(const juce::Colour c)
    {
        bg_ = c;

        for (int i = 0; i < 5; ++i)
        {
            addAndMakeVisible(sliders_.add(new juce::Slider()));
            sliders_.getLast()->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        }
    }

	void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::hotpink);
    }

    void resized() override
    {
	    const bool is_portrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();

        juce::FlexBox fb;
        if (is_portrait)
        {
            fb.flexDirection = juce::FlexBox::Direction::column;
        }
        else
        {
            fb.flexDirection = juce::FlexBox::Direction::row;
        }

        for (juce::Slider* s : sliders_)
        {
            if (is_portrait)
            {
                s->setSliderStyle(juce::Slider::LinearHorizontal);
                fb.items.add(juce::FlexItem(*s).withFlex(static_cast<float>(getHeight()) / 5.0f));
            }
            else
            {
                s->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
                fb.items.add(juce::FlexItem(*s).withFlex(static_cast<float>(getWidth()) / 5.0f));
            }
        }

        fb.performLayout(getLocalBounds().toFloat());
    }
};

class RightPanel : public juce::Component
{
private:
    juce::OwnedArray<juce::Button> buttons_;
    juce::Colour bg_;
public:
    explicit RightPanel(const juce::Colour c) : bg_(c)
    {
        for (int i = 0; i < 10; ++i)
        {
            addAndMakeVisible(buttons_.add(new juce::TextButton(juce::String(i))));
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::hotpink);
    }

    void resized() override
    {
        juce::FlexBox fb;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        for (juce::Button* b : buttons_)
        {
            fb.items.add(juce::FlexItem(*b).withMinWidth(50.0f).withMinHeight(50.0f));
        }

        fb.performLayout(getLocalBounds().toFloat());
    }
};

class LeftPanel : public juce::Component
{
private:
    juce::OwnedArray<juce::Slider> knobs_;
    juce::Colour bg_;
public:
    explicit LeftPanel(const juce::Colour c)
    {
        bg_ = c;
        for (int i = 0; i < 6; i++)
        {
            juce::Slider* slider = new juce::Slider(); //The owned array will delete the allocated slider on cleanup
            slider->setSliderStyle(juce::Slider::SliderStyle::Rotary); //Changes your normal slider into a knob
            slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

            addAndMakeVisible(knobs_.add(slider));
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::hotpink);
    }

    void resized() override
    {
        //==============================================================================
        juce::FlexBox knob_box;
        knob_box.flexWrap = juce::FlexBox::Wrap::wrap;
        knob_box.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

        for (juce::Slider* k : knobs_)
        {
            knob_box.items.add(juce::FlexItem(*k).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
        }

        //==============================================================================
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;

        fb.items.add(juce::FlexItem(knob_box).withFlex(2.5));

        fb.performLayout(getLocalBounds().toFloat());
    }

};
