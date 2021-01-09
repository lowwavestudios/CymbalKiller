/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#define CROSSOVER_ID "crossover"
#define CROSSOVER_NAME "Crossover"
#define SCHIPASS_ID "side-chain HPF"
#define SCHIPASS_NAME "Side-Chain HPF"
#define SCLOPASS_ID "side-chain LPF"
#define SCLOPASS_NAME "Side-Chain LPF"

#define SCPEAKFREQ_ID "side-chain peak freq."
#define SCPEAKFREQ_NAME "Side-Chain Peak Freq."
#define SCPEAKGAIN_ID "side-chain peak gain"
#define SCPEAKGAIN_NAME "Side-Chain Peak Gain"

#define THRESHOLD_ID "threshold high-freq"
#define THRESHOLD_NAME "Threshold High-freq"
#define THRESHOLD2_ID "threshold low-freq"
#define THRESHOLD2_NAME "Threshold Low-freq"

#define ATTACK_ID "attack high-freq"
#define ATTACK_NAME "Attack High-freq"
#define RELEASE_ID "release high-freq"
#define RELEASE_NAME "Release High-freq"
#define HOLD_ID "hold high-freq"
#define HOLD_NAME "Hold High-freq"

#define ATTACK2_ID "attack low-freq"
#define ATTACK2_NAME "Attack Low-freq"
#define RELEASE2_ID "release low-freq"
#define RELEASE2_NAME "Release Low-freq"
#define HOLD2_ID "hold low-freq"
#define HOLD2_NAME "Hold Low-freq"

#define MIX_ID "dry/wet"
#define MIX_NAME "Dry/Wet"

#define INPUT_ID "input"
#define INPUT_NAME "Input Gain"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output Gain"
#define OS_ID "oversample"
#define OS_NAME "Oversample"
#define WET_ID "dry/wet"
#define WET_NAME "Dry/Wet"
#define LA_ID "lookahead"
#define LA_NAME "Lookahead"

#define SC_ID "sidechain"
#define SC_NAME "Side Chain"

#define SOLOHI_ID "solo hifreq"
#define SOLOHI_NAME "Solo High-freq."
#define SOLOLOW_ID "solo lofreq"
#define SOLOLOW_NAME "Solo Low-freq."

#include <JuceHeader.h>
#include "AudioDevEffect.h"
#include "Compressor.h"
//#include "EnvelopeShaper.h"
#include "Compressor2.h"
//#include "EnvelopeShaper2.h"
#include "EnvelopeShaperVisual.h"
#include "EnvelopeShaperVisualLow.h"
#include "DSPFilters4Juce/DSPFilters.h"
//#include "SidechainEQ.h"


//==============================================================================
/**
*/
//create Visualiser class
class Visualiser : public AudioVisualiserComponent
{
public:
    Visualiser() : AudioVisualiserComponent(1)
    {
        setBufferSize(512);
        setSamplesPerBlock(128);
        setColours(Colours::transparentBlack, Colour::fromRGBA(7, 165, 18, 100));
        setOpaque(false);
        setRepaintRate(60);
        
    }

    //override paintChannel method in AudioVisualiserComponent
    void Visualiser::paintChannel(Graphics& g, Rectangle<float> area,
        const Range<float>* levels, int numLevels, int nextSample)
    {
        Path p;
        //create a stroke
        auto visualStrokePath = PathStrokeType(3.f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded);

        getChannelAsPath(p, levels, numLevels, nextSample);

        g.strokePath(p, visualStrokePath, AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
            0.0f, 1.0f, area.getX(), area.getBottom(),
            (float)numLevels, -1.0f, area.getRight(), area.getY()));
    }
};

class VisualiserLow : public AudioVisualiserComponent
{
public:
    VisualiserLow() : AudioVisualiserComponent(1)
    {
        setBufferSize(512);
        setSamplesPerBlock(128);
        setColours(Colours::transparentBlack, Colour::fromRGBA(7, 112, 165, 100));
        setOpaque(false);
        setRepaintRate(60);

    }

    //override paintChannel method in AudioVisualiserComponent
    void VisualiserLow::paintChannel(Graphics& g, Rectangle<float> area,
        const Range<float>* levels, int numLevels, int nextSample)
    {
        Path p;
        //create a stroke
        auto visualStrokePath = PathStrokeType(3.f, juce::PathStrokeType::JointStyle::beveled, juce::PathStrokeType::EndCapStyle::rounded);

        getChannelAsPath(p, levels, numLevels, nextSample);

        g.strokePath(p, visualStrokePath, AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
            0.0f, 1.0f, area.getX(), area.getBottom(),
            (float)numLevels, -1.0f, area.getRight(), area.getY()));
    }
};

class IceyGateAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    IceyGateAudioProcessor();
    ~IceyGateAudioProcessor();

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
    
    //const String getInputChannelName(int channelIndex) const override;
    //const String getOutputChannelName(int channelIndex) const override;
    //bool isInputChannelStereoPair(int index) const override;
    //bool isOutputChannelStereoPair(int index) const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;


    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void prepareSignalVisualiser(AudioBuffer<float> newbuffer);
    void prepareSignalVisualiserLowFreq(AudioBuffer<float> newbuffer);
    void updateFilter();

    
    
    AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    Compressor compressor;
    Compressor2 compressorLowFreq;

    float crossoverFreq, scHPF, scLPF, scPeakFreq, scPeakGain;
    int lastSampleRate {44100};

    int curRatio;
    float curThreshHigh, curAtkTimeHigh, curRelTimeHigh, curHoldTimeHigh;
    float curThreshLow, curAtkTimeLow, curRelTimeLow, curHoldTimeLow;

    float inputGain, outputGain;

    float previousInputGain;
    float previousOutputGain, previousDrywet;
    bool lookahead;
    int delayedBufferLength;
    int writePosition { 0 };
    float drywet;
    bool muteInit { true }; //bool for initial mute of output
    bool muteLA { true };
    bool sidechain;
    bool muteSC{ true };
    bool soloHi{ false }, soloLow{ false };
    int numChanOriginal{2}, numChanNew;


    
    LinearSmoothedValue<float> freqSmooth;
    
    void IceyGateAudioProcessor::fillDelayedBuffer(int ch, int bufferNumSamples, const int delayedBufferLength, const float* bufferData, const float* delayedBufferData);
    void IceyGateAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int ch, const int bufferNumSamples, const int delayBufferLength, const float* bufferData, const float* delayedBufferData);

    Visualiser visualiser;
    VisualiserLow visualiserLowFreq;

    EnvelopeShaperVisual visualiserShaper;
    EnvelopeShaperVisualLow visualiserShaperLowFreq;
    
private:

    void IceyGateAudioProcessor::setParams();

    AudioSampleBuffer delayedBuffer;
    AudioSampleBuffer lateBuffer;
    AudioSampleBuffer originalBuffer;
    AudioSampleBuffer hiBufferVisual;
    AudioSampleBuffer lowBufferVisual;

    //DSP::Butterworth<DSP::HIGHPASS, 1> hiPassFilter;
    dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> hiPassFilter;
    dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> hiPassFilter2;

    AudioSampleBuffer dryBuffer;
    AudioSampleBuffer wetBufferLowFreq, wetBufferHighFreq;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IceyGateAudioProcessor)

};

#endif  // PLUGINPROCESSOR_H_INCLUDED