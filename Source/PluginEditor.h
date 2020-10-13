/*/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

//#ifndef __JUCE_HEADER_419FC5D7D3F5212A__
//#define __JUCE_HEADER_419FC5D7D3F5212A__

#define DETECTION_GROUP 1

#include <JuceHeader.h>


/*
#define dB(x) 20.0 * ((x) > 0.00001 ? log10(x) : -5.0)
#define dB2mag(x) pow(10.0, (x) / 20.0)
*/

//==============================================================================

/**
*/


class buttonLookAndFeel : public LookAndFeel_V4
{
public:
    buttonLookAndFeel() : LookAndFeel_V4()
    {

    }

    void buttonLookAndFeel::drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown) override
    {
        ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        Rectangle<float> tickBounds(x, y, w, h);

        g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
        g.drawRect(tickBounds, 1.0f);

        if (ticked)
        {
            g.setColour(component.findColour(ToggleButton::tickColourId));
            //auto tick = getCrossShape(1.f);
            g.fillRect(tickBounds.reduced(2.f, 2.f));
            //g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 4).toFloat(), false));
        }
    };
    
};

class soloButtonLookAndFeel : public LookAndFeel_V4
{
public:
    soloButtonLookAndFeel() : LookAndFeel_V4()
    {

    }

    void soloButtonLookAndFeel::drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown) override
    {
        ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        Rectangle<float> tickBounds(x, y, w, h);

        g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
        g.drawRect(tickBounds, 1.0f);

        if (ticked)
        {
            g.setColour(component.findColour(ToggleButton::tickColourId));
            //auto tick = getCrossShape(1.f);
            g.fillRect(tickBounds.reduced(2.f, 2.f));
            //g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 4).toFloat(), false));
        }
        {
            g.setColour(Colour::fromRGBA(255, 255, 255, 85));
            g.setFont(h * 0.9);
            g.drawText("S", tickBounds, Justification::centred);
        }
        
    };

};

class threshSliderLookAndFeel : public LookAndFeel_V4
{
public:
    threshSliderLookAndFeel() : LookAndFeel_V4()
    {

    }

    void threshSliderLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
    {
        
        
        g.setColour(slider.findColour(Slider::trackColourId));
        g.fillRect(Rectangle<float>(x + 0.5f, sliderPos, width - 1.0f, y + (height - sliderPos)));
        g.setOpacity(1.0);
        g.fillRect(Rectangle<float>(x + 0.5f, sliderPos, width - 1.0f, 2.5f));
       
        {
            g.fillRect(Rectangle<float>((x + 0.5) + (0.4 * (width - 1.0)), sliderPos - 2.5f, 0.2 * (width - 1.0), 7.5f));
            {
                g.setColour(Colours::black);
                g.fillRect(Rectangle<float>((x + 0.5) + (0.4 * (width - 1.0)) + 5, sliderPos + 0.5, (0.2 * (width - 1.0)) - 10, 1.5f));
            }

        }

    }

};


class IceyGateAudioProcessorEditor : public AudioProcessorEditor, public Timer, public Slider::Listener, public Button::Listener
{
public:
    IceyGateAudioProcessorEditor(IceyGateAudioProcessor&);
    ~IceyGateAudioProcessorEditor();

    //==============================================================================
    void timerCallback() override;

    void paint(Graphics& g) override;
    void paintOverChildren(Graphics& g) override;
    void resized() override;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    // Binary resources:
    static const char* cymbalKillerTitle_svg2;
    static const int cymbalKillerTitle_svg2Size;
    static const char* logoSmall_png;
    static const int logoSmall_pngSize;


private:

    void sliderValueChanged(Slider* sliderThatHasChanged) override;
    void buttonClicked(Button* buttonThatHasChanged) override;

    void IceyGateAudioProcessorEditor::keepAspectRatio();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    IceyGateAudioProcessor& processor;

    IceyGateAudioProcessor& getProcessor() const
    {
        return static_cast<IceyGateAudioProcessor&> (processor);
    }

    int sliderHeight;
    int sliderSpacing;
    int sideMargin;
    double square;
    juce::Rectangle<int> area;


    Slider thresholdSlider;
    Slider attackSlider;
    Slider releaseSlider, holdSlider;

    Slider thresholdLowSlider;
    Slider attackLowSlider;
    Slider releaseLowSlider, holdLowSlider;
    
    Slider crossoverSlider, scHPFSlider, scLPFSlider;

    Slider inputGainSlider, outputGainSlider, mixSlider;
    buttonLookAndFeel buttonLAF;
    soloButtonLookAndFeel soloLAF;
    threshSliderLookAndFeel threshLAF; 

    ToggleButton laButton;
    ToggleButton scButton;
    ToggleButton soloHiButton;
    ToggleButton soloLowButton;
    std::unique_ptr<HyperlinkButton> help;

    void IceyGateAudioProcessorEditor::newSliderStyle(Colour textColour, float trackTransp, Slider::TextEntryBoxPosition textBoxStyle, bool writable, int width, int height);

    void IceyGateAudioProcessorEditor::paramTextStyle(Graphics& g, String name, float x, float y, float width, float fontHeight, bool drawBox, Justification justification, Colour textColour = Colour(245, 245, 245));
    void IceyGateAudioProcessorEditor::threshTextStyle(Graphics& g, String name, float x, float y, float width, float fontHeight, bool drawBox);
    void IceyGateAudioProcessorEditor::newButtonStyle(Colour textColour, Colour tickColour, Colour disableColour);
    void IceyGateAudioProcessorEditor::newBoxStyle(Colour textColour, Colour tickColour, Colour disableColour);

    //Fundo svg
    //std::unique_ptr<Drawable> fundo;
    Image cachedImage_logo2020_png2_1;
    std::unique_ptr<Drawable> title;
    

public:

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        crossoverSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        scHPFSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        scLPFSliderValue;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        thresholdSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        attackSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        releaseSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        holdSliderValue;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        thresholdLowSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        attackLowSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        releaseLowSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        holdLowSliderValue;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        inputGainSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        outputGainSliderValue;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        mixSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        laButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        scButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        soloHiButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        soloLowButtonValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IceyGateAudioProcessorEditor)
};

//#endif   // __JUCE_HEADER_419FC5D7D3F5212A__
