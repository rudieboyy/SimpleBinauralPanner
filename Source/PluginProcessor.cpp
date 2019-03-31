/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HRIR3D.h"
//==============================================================================
SimpleBinauralPannerAudioProcessor::SimpleBinauralPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SimpleBinauralPannerAudioProcessor::~SimpleBinauralPannerAudioProcessor()
{
}

//==============================================================================
const String SimpleBinauralPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleBinauralPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleBinauralPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleBinauralPannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleBinauralPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleBinauralPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleBinauralPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleBinauralPannerAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleBinauralPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleBinauralPannerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleBinauralPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    IR_L.prepare(spec);
    IR_R.prepare(spec);
    updateHRIRFilter();
    monoBuffer.setSize(1, samplesPerBlock);
    IR_L.reset(); // reset the filter's processing pipeline, ready to start a new stream of data
    IR_R.reset();
}

void SimpleBinauralPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleBinauralPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleBinauralPannerAudioProcessor::updateHRIRFilter()
{
    *(IR_L.coefficients) = dsp::FIR::Coefficients<float>(hrir_l[theta],LEN);
    *(IR_R.coefficients) = dsp::FIR::Coefficients<float>(hrir_r[theta],LEN);
}

void SimpleBinauralPannerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    if(bypassed == true)
    {
        buffer.applyGain(overallgain);
        return;
    }
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    buffer.applyGain(overallgain*3.0);
    auto bufferL = buffer.getWritePointer(0);
    auto bufferR = buffer.getWritePointer(1);
    const auto BLength = buffer.getNumSamples();
    if(totalNumInputChannels == 2)
    {
        buffer.addFrom(0, 0, buffer.getWritePointer(1), BLength);
        buffer.applyGain(0.5f);
    }
    monoBuffer.copyFrom(0, 0, buffer, 0, 0, BLength);
    
    updateHRIRFilter();
    dsp::AudioBlock<float> blockL = dsp::AudioBlock<float>(&bufferL,1,BLength);
    dsp::AudioBlock<float> blockR = dsp::AudioBlock<float>(&bufferR,1,BLength);
    IR_L.process(dsp::ProcessContextReplacing<float>(blockL));
    IR_R.process(dsp::ProcessContextReplacing<float>(blockR));
    
    dryRatio = 1.0 - wetRatio;
    for(auto i=0;i<BLength;i++)
    {
        const auto monoInput = *(monoBuffer.getReadPointer(0, i));
        bufferL[i] = wetRatio*bufferL[i]+dryRatio*monoInput;
        bufferR[i] = wetRatio*bufferR[i]+dryRatio*monoInput;
    }

}

//==============================================================================
bool SimpleBinauralPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleBinauralPannerAudioProcessor::createEditor()
{
    return new SimpleBinauralPannerAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleBinauralPannerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleBinauralPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleBinauralPannerAudioProcessor();
}
