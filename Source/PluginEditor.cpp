/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleBinauralPannerAudioProcessorEditor::SimpleBinauralPannerAudioProcessorEditor (SimpleBinauralPannerAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p), bgColour(51, 51, 51),kColour(73, 166, 201)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    HeadImage = ImageCache::getFromMemory(HeadImage::head2_png, HeadImage::head2_pngSize);
    
    addAndMakeVisible(gainKnob);
    gainKnob.setSliderStyle(Slider::Rotary);
    gainKnob.setRange(0.0, 1.0, 0.05);
    gainKnob.setRotaryParameters(10/8.f*float_Pi, 22/8.f*float_Pi, true);
    gainKnob.setColour(Slider::rotarySliderFillColourId, kColour);
    gainKnob.setColour(Slider::textBoxBackgroundColourId, Colours::black);
    gainKnob.setColour(Slider::textBoxTextColourId,Colours::white);
    gainKnob.setColour(Slider::textBoxOutlineColourId,Colours::black);
    gainKnob.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    gainKnob.addListener(this);
    
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.addListener(this);
    
    addAndMakeVisible(wetKnob);
    wetKnob.setSliderStyle(Slider::Rotary);
    wetKnob.setRange(0.0, 1.0, 0.01);
    wetKnob.setRotaryParameters(10/8.f*float_Pi, 22/8.f*float_Pi, true);
    wetKnob.setColour(Slider::rotarySliderFillColourId, kColour);
    wetKnob.setColour(Slider::textBoxBackgroundColourId, Colours::black);
    wetKnob.setColour(Slider::textBoxTextColourId,Colours::white);
    wetKnob.setColour(Slider::textBoxOutlineColourId,Colours::black);
    wetKnob.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    wetKnob.addListener(this);
    
    addAndMakeVisible(wetLabel);
    wetLabel.setText("Wet Mix(%)", dontSendNotification);
    wetLabel.addListener(this);
    
    addAndMakeVisible(elevSlider);
    elevSlider.setSliderStyle(Slider::LinearVertical);
    elevSlider.setRange(-45, 90, 15); //10 levels
    elevSlider.setValue(0.);
    elevSlider.setColour(Slider::thumbColourId, kColour);
    elevSlider.setColour(Slider::textBoxBackgroundColourId, Colours::black);
    elevSlider.setColour(Slider::textBoxTextColourId, Colours::white);
    elevSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);
    elevSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    elevSlider.addListener(this);
    
    addAndMakeVisible(elevLabel);
    elevLabel.setText("Elevation", dontSendNotification);
    elevLabel.addListener(this);
    
    addAndMakeVisible(azimSlider);
    azimSlider.setSliderStyle(Slider::Rotary);
    //azimSlider.setRange(0.0, 127.0, 1.0);
    azimSlider.setRotaryParameters(0.0, (2.0*float_Pi), false); // make it 187 levels
    azimSlider.setRange(0.0, 360,15); //change it to 345 again if it does not work
    azimSlider.setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    azimSlider.setColour(Slider::textBoxBackgroundColourId, Colours::black);
    //azimSlider.setColour(Slider::textBoxTextColourId,Colours::white);
    //azimSlider.setColour(Slider::textBoxOutlineColourId,Colours::black);
    azimSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 30, 10);
    azimSlider.addListener(this);
    
    addAndMakeVisible(Bypass);
    Bypass.setName("Bypass");
    Bypass.setButtonText("BYPASS");
    Bypass.setColour(TextButton::buttonColourId, Colours::black);
    Bypass.setColour(TextButton::buttonOnColourId,Colours::blue);
    Bypass.setColour(TextButton::textColourOffId, Colours::blue);
    Bypass.setColour(TextButton::textColourOnId, Colours::black);
    Bypass.setToggleState(true, dontSendNotification);
    Bypass.setClickingTogglesState(true);
    Bypass.addListener(this);
    
}

SimpleBinauralPannerAudioProcessorEditor::~SimpleBinauralPannerAudioProcessorEditor()
{
}

//==============================================================================
void SimpleBinauralPannerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.drawImage(HeadImage, 105, 100, 155, 150, 5, 10, 140, 160);
    //g.setColour (Colours::white);
}

void SimpleBinauralPannerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainKnob.setBounds(getWidth()-180, getBottom()-80, 70, 70);
    gainLabel.setBounds(getWidth()-165, getBottom()-90, 70, 15);
    wetKnob.setBounds(40, getBottom()-80, 70, 70);
    wetLabel.setBounds(40, getBottom()-90, 70, 15);
    elevSlider.setBounds(getWidth()-75, 40, 50, 300);
    elevLabel.setBounds(getWidth()-80, 20, 70, 15);
    azimSlider.setBounds(getWidth()-390, 10, 310, 310);
    Bypass.setBounds(getWidth()-90, getBottom()-40, 80, 30);
}
void SimpleBinauralPannerAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if(slider == &elevSlider)
    {
        if(elevSlider.getValue() == -45)
        {
            elevation = 0;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == -30)
        {
            elevation = 24;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == -15)
        {
            elevation = 48;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 0)
        {
            elevation = 72;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 15)
        {
            elevation = 96;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 30)
        {
            elevation = 120;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 45)
        {
            elevation = 144;
            azimSlider.setRange(0.0, 360,15);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 60)
        {
            elevation = 168;
            azimSlider.setValue(0.0);
            azimSlider.setRange(0.0, 360,30);
            processor.theta = elevation;
        }
        else if(elevSlider.getValue() == 75)
        {
            elevation = 180;
            azimSlider.setValue(0.0);
            azimSlider.setRange(0.0, 360,60);
            processor.theta = elevation;
        }
        else
        {
            elevation = 186;
            azimSlider.setValue(0.0);
            processor.theta = elevation;
        }
    }
    
    if(slider == &azimSlider)
    {
        azimuth = azimSlider.getValue();
        if(0<=elevation && elevation<=144) //if -45<= elevation <= 45
            processor.theta = azimuth/15+elevation; // the increment of azimuth is 15 degrees
        else if(168==elevation) // if elevation = 60
        {
            processor.theta = azimuth/30+elevation;
        }
        else if(elevation == 180) // if elevation = 75
        {
            processor.theta = azimuth/60+elevation;
        }
        else //elevation == 186
            processor.theta = 186;

    }
    
    if(slider == &gainKnob)
    {
        processor.overallgain = gainKnob.getValue();
    }
    
    if(slider == &wetKnob)
    {
        processor.wetRatio = wetKnob.getValue();
    }
}
void SimpleBinauralPannerAudioProcessorEditor::labelTextChanged (Label* labelThatHasChanged)
{
    
}
void SimpleBinauralPannerAudioProcessorEditor::buttonClicked(Button* button)
{
    if(button == &Bypass)
    {
        if(Bypass.getToggleState() == true)
        {
            Bypass.setToggleState(true, dontSendNotification);
            processor.bypassed = true;
        }
        else
        {
            Bypass.setToggleState(false, dontSendNotification);
            processor.bypassed = false;
        }
        
    }
}

