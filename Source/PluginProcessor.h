/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define LEN 256
//==============================================================================
/**
*/
class SimpleBinauralPannerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimpleBinauralPannerAudioProcessor();
    ~SimpleBinauralPannerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //=========================
    int theta = 72;
    bool bypassed = true;
    float overallgain = 0.0;
    float wetRatio = 0.0;
    float dryRatio = 0.0;
    //===========================
    void updateHRIRFilter();
private:
    dsp::ProcessSpec spec;
    AudioSampleBuffer monoBuffer;
    dsp::FIR::Filter<float> IR_L;
    dsp::FIR::Filter<float> IR_R;
    //dsp::ProcessorDuplicator<dsp::FIR::Filter<float>, dsp::FIR::Coefficients<float>> IR_L;
    //dsp::ProcessorDuplicator<dsp::FIR::Filter<float>, dsp::FIR::Coefficients<float>> IR_R;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleBinauralPannerAudioProcessor)
};
