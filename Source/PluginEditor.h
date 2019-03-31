/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "HeadImage.h"

//==============================================================================
/**
 */
class SimpleBinauralPannerAudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public Label::Listener,
public Button::Listener
{
public:
    SimpleBinauralPannerAudioProcessorEditor (SimpleBinauralPannerAudioProcessor&);
    ~SimpleBinauralPannerAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    // sliders
    void sliderValueChanged(Slider* slider) override;
    
    // button
    void buttonClicked(Button* button) override;
    
    // label
    void labelTextChanged (Label* labelThatHasChanged) override;
    //
    int elevation = 0;
    int azimuth = 0;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleBinauralPannerAudioProcessor& processor;
    // Below is a image object
    Image HeadImage;
    // Below are slider objects
    Slider gainKnob;
    Slider wetKnob;
    Slider elevSlider;
    Slider azimSlider;
    // Below are label objects
    Label gainLabel;
    Label elevLabel;
    Label wetLabel;
    // Below is a Button object
    TextButton Bypass;
    // Below are color objects
    Colour bgColour;
    Colour kColour;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleBinauralPannerAudioProcessorEditor)
};

