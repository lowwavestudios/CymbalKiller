/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath> 
#include <algorithm>




//==============================================================================
IceyGateAudioProcessor::IceyGateAudioProcessor()


#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()

        .withInput("Input", AudioChannelSet::stereo(), true)
        .withInput("SideChain In", AudioChannelSet::stereo(), true)
        .withOutput("Output", AudioChannelSet::stereo(), true)
        .withOutput("SideChain Out", AudioChannelSet::stereo(), true)

    ), parameters(*this, nullptr, "PARAMETER", createParameterLayout()
    ), 
    
    compressor(), curRatio(100)
    
         

#endif
{

}



IceyGateAudioProcessor::~IceyGateAudioProcessor()
{

}

AudioProcessorValueTreeState::ParameterLayout IceyGateAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr <RangedAudioParameter>> params;

    auto crossoverParam = std::make_unique <AudioParameterFloat>(CROSSOVER_ID, CROSSOVER_NAME, 20.0f, 20000.0f, 500.0f);
    auto scHPFParam = std::make_unique <AudioParameterFloat>(SCHIPASS_ID, SCHIPASS_NAME, 20.0f, 5000.0, 20.0f);
    auto scLPFParam = std::make_unique <AudioParameterFloat>(SCLOPASS_ID, SCLOPASS_NAME, 150.f, 20000.0f, 20000.0f);

    auto thresholdHighParam = std::make_unique <AudioParameterFloat>(THRESHOLD_ID, THRESHOLD_NAME, -90.0f, 0.0f, 0.0f);
    auto attackHighParam = std::make_unique <AudioParameterFloat>(ATTACK_ID, ATTACK_NAME, 0.01f, 100.0f, 0.01f);
    auto holdHighParam = std::make_unique <AudioParameterFloat>(HOLD_ID, HOLD_NAME, 1.f, 500.f, 35.f);
    auto releaseHighParam = std::make_unique <AudioParameterFloat>(RELEASE_ID, RELEASE_NAME, 1.f, 500.0f, 120.0f);

    auto thresholdLowParam = std::make_unique <AudioParameterFloat>(THRESHOLD2_ID, THRESHOLD2_NAME, -90.0f, 0.0f, 0.0f);
    auto attackLowParam = std::make_unique <AudioParameterFloat>(ATTACK2_ID, ATTACK2_NAME, 0.01f, 100.0f, 0.01f);
    auto holdLowParam = std::make_unique <AudioParameterFloat>(HOLD2_ID, HOLD2_NAME, 1.f, 500.f, 80.f);
    auto releaseLowParam = std::make_unique <AudioParameterFloat>(RELEASE2_ID, RELEASE2_NAME, 1.f, 1000.0f, 250.0f);

    auto inputGainParam = std::make_unique<AudioParameterFloat>(INPUT_ID, INPUT_NAME, -96.f, 12.f, 0.0f);
    auto outputGainParam = std::make_unique<AudioParameterFloat>(OUTPUT_ID, OUTPUT_NAME, -96.f, 12.f, 0.0f);
    auto drywetParam = std::make_unique<AudioParameterFloat>(WET_ID, WET_NAME, 0.f, 100.f, 100.0f);
    auto lookaheadParam = std::make_unique<AudioParameterBool>(LA_ID, LA_NAME, false);
    auto sidechainParam = std::make_unique<AudioParameterBool>(SC_ID, SC_NAME, false);

    auto soloHiParam = std::make_unique<AudioParameterBool>(SOLOHI_ID, SOLOHI_NAME, false);
    auto soloLowParam = std::make_unique<AudioParameterBool>(SOLOLOW_ID, SOLOLOW_NAME, false);

    params.push_back(std::move(crossoverParam));
    params.push_back(std::move(scHPFParam));
    params.push_back(std::move(scLPFParam));

    params.push_back(std::move(thresholdHighParam));
    params.push_back(std::move(attackHighParam));
    params.push_back(std::move(holdHighParam));
    params.push_back(std::move(releaseHighParam));

    params.push_back(std::move(thresholdLowParam));
    params.push_back(std::move(attackLowParam));
    params.push_back(std::move(holdLowParam));
    params.push_back(std::move(releaseLowParam));

    params.push_back(std::move(inputGainParam));
    params.push_back(std::move(outputGainParam));
    params.push_back(std::move(drywetParam));
    params.push_back(std::move(lookaheadParam));
    params.push_back(std::move(sidechainParam));

    params.push_back(std::move(soloLowParam));
    params.push_back(std::move(soloHiParam));


    return { params.begin(), params.end() };

}
//==============================================================================
const String IceyGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

/*
const String IceyGateAudioProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

const String IceyGateAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

bool IceyGateAudioProcessor::isInputChannelStereoPair(int index) const
{
    return true;
}

bool IceyGateAudioProcessor::isOutputChannelStereoPair(int index) const
{
    return true;
}
*/
bool IceyGateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IceyGateAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool IceyGateAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double IceyGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}


int IceyGateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IceyGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IceyGateAudioProcessor::setCurrentProgram (int index)
{

}

const String IceyGateAudioProcessor::getProgramName (int index)
{
    return String();
}

void IceyGateAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void IceyGateAudioProcessor::setParams()
{
    curThreshHigh = (*parameters.getRawParameterValue(THRESHOLD_ID));
    curAtkTimeHigh = (*parameters.getRawParameterValue(ATTACK_ID));
    curRelTimeHigh = (*parameters.getRawParameterValue(RELEASE_ID));
    curHoldTimeHigh = (*parameters.getRawParameterValue(HOLD_ID));

    curThreshLow = (*parameters.getRawParameterValue(THRESHOLD2_ID));
    curAtkTimeLow = (*parameters.getRawParameterValue(ATTACK2_ID));
    curRelTimeLow = (*parameters.getRawParameterValue(RELEASE2_ID));
    curHoldTimeLow = (*parameters.getRawParameterValue(HOLD2_ID));
    
    crossoverFreq = *parameters.getRawParameterValue(CROSSOVER_ID);
    scHPF = *parameters.getRawParameterValue(SCHIPASS_ID);
    scLPF = *parameters.getRawParameterValue(SCLOPASS_ID);

    inputGain = *parameters.getRawParameterValue(INPUT_ID);
    outputGain = *parameters.getRawParameterValue(OUTPUT_ID);
    drywet = *parameters.getRawParameterValue(WET_ID);
    lookahead = *parameters.getRawParameterValue(LA_ID);

    sidechain = *parameters.getRawParameterValue(SC_ID);

    soloHi = *parameters.getRawParameterValue(SOLOHI_ID);
    soloLow = *parameters.getRawParameterValue(SOLOLOW_ID);


    visualiserShaper.setAttack(curAtkTimeHigh);
    visualiserShaper.setRelease(curRelTimeHigh);
    visualiserShaper.setHold(curHoldTimeHigh);

    visualiserShaperLowFreq.setAttack(curAtkTimeLow);
    visualiserShaperLowFreq.setRelease(curRelTimeLow);
    visualiserShaperLowFreq.setHold(curHoldTimeLow);
}

//==============================================================================

void IceyGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    int numch = getTotalNumInputChannels(); //> 2 ? 2 : getTotalNumInputChannels();
    delayedBufferLength = samplesPerBlock + (0.01 * sampleRate);
    delayedBuffer.setSize(numch, delayedBufferLength);
    lateBuffer.setSize(numch, samplesPerBlock);

    lastSampleRate = sampleRate;
    
    compressor.prepareForPlayback(lastSampleRate, samplesPerBlock, numch);
    compressorLowFreq.prepareForPlayback(lastSampleRate, samplesPerBlock, numch);
    
    setParams();

    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numch;

    hiPassFilter.reset();
    hiPassFilter.prepare(spec);
    hiPassFilter2.reset();
    hiPassFilter2.prepare(spec);
    updateFilter();
    
    freqSmooth.reset(lastSampleRate, 0.0001);

    dryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);

    visualiser.clear();
    visualiserShaper.prepareForPlayback(lastSampleRate);

    visualiserLowFreq.clear();
    visualiserShaperLowFreq.prepareForPlayback(lastSampleRate);

}

void IceyGateAudioProcessor::updateFilter()
{
    crossoverFreq = *parameters.getRawParameterValue(CROSSOVER_ID);
    hiPassFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    hiPassFilter.state->setCutOffFrequency(lastSampleRate, crossoverFreq / 10, 0.1f );
    hiPassFilter2.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    hiPassFilter2.state->setCutOffFrequency(lastSampleRate, crossoverFreq / 10, 0.1f);
}

void IceyGateAudioProcessor::fillDelayedBuffer(int ch, int bufferNumSamples, const int delayedBufferLength, const float* bufferData, const float* delayedBufferData)
{
    if (delayedBufferLength >= bufferNumSamples + writePosition)
    {
        delayedBuffer.copyFrom(ch, writePosition, bufferData, bufferNumSamples);
    }
    else
    {
        const int bufferRemaining = delayedBufferLength - writePosition;

        delayedBuffer.copyFrom(ch, writePosition, bufferData, bufferRemaining);
        delayedBuffer.copyFrom(ch, 0, bufferData + bufferRemaining, bufferNumSamples - bufferRemaining);
    }
}

void IceyGateAudioProcessor::getFromDelayBuffer(AudioBuffer<float> &lateBuffer, int ch, const int bufferNumSamples, const int delayBufferLength, const float* bufferData, const float* delayedBufferData)
{
    const int readPosition = static_cast<int> (delayBufferLength + writePosition - (lastSampleRate * 0.01)) % delayedBufferLength;

    if (delayBufferLength > bufferNumSamples + readPosition)
    {
        lateBuffer.copyFrom(ch, 0, delayedBufferData + readPosition, bufferNumSamples);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - readPosition;
        lateBuffer.copyFrom(ch, 0, delayedBufferData + readPosition, bufferRemaining);
        lateBuffer.copyFrom(ch, bufferRemaining, delayedBufferData, bufferNumSamples - bufferRemaining);
    }
}

void IceyGateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IceyGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    //// the sidechain can take any layout, the main bus needs to be the same on the input and output
    
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
       
        && !layouts.getMainInputChannelSet().isDisabled();
}
#endif



void IceyGateAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessage)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
   
    //totalNumOutputChannels > 2 ? totalNumOutputChannels = 2 : 1;
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).

    setParams();

    {//set High Freq Compressor params
        compressor.setThreshold(curThreshHigh);
        compressor.setRatio(curRatio);
        compressor.setAttack(curAtkTimeHigh);
        compressor.setRelease(curRelTimeHigh);
        compressor.setHold(curHoldTimeHigh);
        compressor.setSC(scHPF, scLPF, lastSampleRate);
    }
    
    {//set low Freq Compressor params
        compressorLowFreq.setThreshold(curThreshLow);
        compressorLowFreq.setRatio(curRatio);
        compressorLowFreq.setAttack(curAtkTimeLow);
        compressorLowFreq.setRelease(curRelTimeLow);
        compressorLowFreq.setHold(curHoldTimeLow);
        compressorLowFreq.setSC(scHPF, scLPF, lastSampleRate);
    }


    if (lookahead == true)
    { 
        lookahead = true;
        setLatencySamples(static_cast<int>(0.01 * lastSampleRate));
    }
    else 
    {
        lookahead = false;
        setLatencySamples(0);
    }
    
    //make sure solo hi and low deactivate one another
    if (soloHi == true)
    {
        soloLow = false;
    }
    else if (soloLow == true)
    {
        soloHi = false;
    }


    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    auto bufferNumSamples = buffer.getNumSamples();
    

    //input gain ramp
    auto currentInputGain = inputGain;

    if (currentInputGain == previousInputGain)
    {
        for (int ch = 0; ch < totalNumOutputChannels && ch < 2; ch++)
        {
            buffer.applyGain(ch, 0, bufferNumSamples, Decibels::decibelsToGain(currentInputGain));
        }
        
    }
    else
    {
        for (int ch = 0; ch < totalNumOutputChannels && ch < 2; ch++)
        {
            buffer.applyGainRamp(ch, 0, bufferNumSamples, Decibels::decibelsToGain(previousInputGain), Decibels::decibelsToGain(currentInputGain));
        }
        
        previousInputGain = currentInputGain;
    }


    //lookahead
    if (lookahead == true)
    {
        //copy original un-delayed buffer for compressor side-chain
        originalBuffer.makeCopyOf(buffer);

        //delay buffer
        for (int ch = 0; ch < totalNumOutputChannels; ch++)
        {
            const float* bufferData = buffer.getReadPointer(ch);
            const float* delayedBufferData = delayedBuffer.getReadPointer(ch);

            fillDelayedBuffer(ch, bufferNumSamples, delayedBufferLength, bufferData, delayedBufferData);
            getFromDelayBuffer(lateBuffer, ch, bufferNumSamples, delayedBufferLength, bufferData, delayedBufferData);
        }

        writePosition += bufferNumSamples;
        writePosition %= delayedBufferLength;

        buffer.makeCopyOf(lateBuffer);

        //mute first buffer to avoid overshoot
        if (muteLA == true)
        {
            for (int ch = 0; ch < totalNumOutputChannels; ch++)
            {
                buffer.applyGainRamp(ch, 0, bufferNumSamples, 0, 0);
            }
        }
        muteLA = false;

        //mute if number of ch changes
        numChanNew = getTotalNumOutputChannels();
        if (numChanOriginal != numChanNew)
        {
            for (int ch = 0; ch < totalNumOutputChannels; ch++)
            {
                buffer.applyGainRamp(ch, 0, bufferNumSamples, 0, 0);
            }
        }
        numChanOriginal = numChanNew;
    }


    //mute first buffer when activating sidechain
    if (sidechain == true)
    {
        if (getTotalNumInputChannels() < 3)
        {
            sidechain = false;
        }
        //mute first buffer to avoid overshoot
        if (muteSC == true)
        {
            for (int ch = 0; ch < totalNumOutputChannels; ch++)
            {
                buffer.applyGain(ch, 0, buffer.getNumSamples(), 0.f);
            }
        }
        muteSC = false;
    }
    

    //Copy Dry Signal
    dryBuffer.makeCopyOf(buffer, true);
    wetBufferLowFreq.makeCopyOf(buffer, true);
    wetBufferHighFreq.makeCopyOf(buffer, true);
    dsp::AudioBlock<float> dryBlock(dryBuffer);
    


    //LowFreq processing
    updateFilter();
    dsp::AudioBlock<float> lowblock(wetBufferLowFreq);
    lowBufferVisual.makeCopyOf(wetBufferLowFreq);
    hiPassFilter2.process(dsp::ProcessContextReplacing<float>(lowblock)); //hi pass buffer
    lowblock.multiplyBy(-1.f); //reverse polarity


    //add dry with hi pass reversed to get a low pass
    dsp::ProcessContextReplacing<float>lowpassedBlock(lowblock);
    lowpassedBlock.getOutputBlock().add(dryBlock); 

    //detects from buffer and applies to low pass (wetBufferLowFreq)
    if (soloHi == false)
    {
        compressorLowFreq.processBuffer(buffer, originalBuffer, wetBufferLowFreq, lowBufferVisual, lookahead, sidechain);
    }

    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        wetBufferLowFreq.applyGain(ch, 0, bufferNumSamples, -1.f);
    }

    //Visualiser Low freq
    dsp::AudioBlock<float> lowVisualBlock(lowBufferVisual);
    dsp::ProcessContextReplacing<float> lowVisualProcess(lowVisualBlock);

    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        lowBufferVisual.applyGain(ch, 0, bufferNumSamples, -1.f);
    }

    lowVisualProcess.getOutputBlock().add(dryBlock);

    //Process, push and clear visualiser buffer
    prepareSignalVisualiserLowFreq(lowBufferVisual);





    //HighFreq processing

    dsp::AudioBlock<float> hiblock(wetBufferHighFreq);
    hiBufferVisual.makeCopyOf(wetBufferHighFreq);
    hiPassFilter.process(dsp::ProcessContextReplacing<float>(hiblock));

    if (soloLow == false)
    {
        compressor.processBuffer(buffer, originalBuffer, wetBufferHighFreq, hiBufferVisual, lookahead, sidechain);
    }

    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        wetBufferHighFreq.applyGain(ch, 0, bufferNumSamples, -1.f);
    }

    //Hi-pass Filter / Sum dryWet
    dsp::AudioBlock<float> block(buffer);

    //Sum of dry with wet comp
    dsp::ProcessContextReplacing<float> finalBlock(block);
    finalBlock.getOutputBlock().add(lowblock).add(hiblock);


    //Visualiser High freq
    dsp::AudioBlock<float> hiVisualBlock(hiBufferVisual);
    dsp::ProcessContextReplacing<float> hiVisualProcess(hiVisualBlock);

    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        hiBufferVisual.applyGain(ch, 0, bufferNumSamples, -1.f);
    }

    hiVisualProcess.getOutputBlock().add(dryBlock);

    //Process, push and clear visualiser buffer
    prepareSignalVisualiser(hiBufferVisual);

    
    

    
    //OutputGain
    auto currentOutputGain = outputGain;

    if (currentOutputGain == previousOutputGain)
    {
        buffer.applyGain(Decibels::decibelsToGain(currentOutputGain));
    }
    else
    {
        buffer.applyGainRamp(0, bufferNumSamples, Decibels::decibelsToGain(previousOutputGain), Decibels::decibelsToGain(currentOutputGain));
        previousOutputGain = currentOutputGain;
    }
    

    //Mix Processed with unprocessed
    auto currDrywet = drywet;
    if (currDrywet == previousDrywet)
    {
        buffer.applyGain(currDrywet / 100);
        dryBuffer.applyGain(1.f - (currDrywet / 100));
    }
    else
    {
        buffer.applyGainRamp(0, bufferNumSamples, previousDrywet / 100, currDrywet / 100);
        dryBuffer.applyGainRamp(0, bufferNumSamples, 1.f - (previousDrywet / 100), 1.f - (currDrywet / 100));
        previousDrywet = currDrywet;
    }
    for (int ch = 0; ch < totalNumOutputChannels; ch++)
    {
        float* w = buffer.getWritePointer(ch);

        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            w[n] = buffer.getSample(ch, n) + dryBuffer.getSample(ch, n);
        }
    }

    //Mute first buffer on startup to avoid overshoot
    if (muteInit == true)
    {
        for (int ch = 0; ch < totalNumOutputChannels; ch++)
        {
            buffer.applyGainRamp(ch, 0, bufferNumSamples, 0, 0);
        }
    }
    muteInit = false;
    

}

//==============================================================================
bool IceyGateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* IceyGateAudioProcessor::createEditor()
{
    //return new foleys::MagicPluginEditor(magicState, BinaryData::Magic_xml, BinaryData::Magic_xmlSize);
    return new IceyGateAudioProcessorEditor(*this);
}

//==============================================================================
void IceyGateAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    MemoryOutputStream stream(destData, false);
    parameters.state.writeToStream(stream);
}

void IceyGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    ValueTree tree = ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
        parameters.state = tree;
}


void IceyGateAudioProcessor::prepareSignalVisualiser(AudioBuffer<float> newbuffer)
{
    AudioSampleBuffer nBuffer;
    
    auto numS = newbuffer.getNumSamples();
    nBuffer.setSize(1, numS, false, true, false);
    

    if (newbuffer.getNumChannels() > 1)
    {
        newbuffer.addFrom(0, 0, newbuffer.getReadPointer(1), numS); //sum LR
    }

    //max sample in buffer

    auto mag = newbuffer.getMagnitude(0, numS); //get magnitude
    auto mult = 1 / jlimit<float>(0.00001f, 1.f, mag);  // multiple for gain

    float* w = newbuffer.getWritePointer(0);
    newbuffer.applyGain(0, 0, numS, 1);

    float maxS = mag * mult; // apply gain to max
    maxS > 0.00001 ? maxS = 1 / maxS : maxS = 1; // compare max less than 0

    //sum LR, LPF and rectify
    //float* w = newbuffer.getWritePointer(0);
    for (int i = 0; i < numS; i++)
    {
        w[i] = fabs(w[i]);
        w[i] *= maxS; //get auto gain from max in the buffer
        w[i] = (w[i] * 2) - 1.f; //offset to 0 to -1 and double gain
        visualiserShaper.processAudioSample(w[i]); // process Envelope
        nBuffer.setSample(0, i, w[i]);
            
    }


    newbuffer.clear();
    visualiser.pushBuffer(nBuffer);
    nBuffer.clear();

} 

void IceyGateAudioProcessor::prepareSignalVisualiserLowFreq(AudioBuffer<float> newbuffer)
{
    AudioSampleBuffer nBuffer;

    auto numS = newbuffer.getNumSamples();
    nBuffer.setSize(1, numS, false, true, false);


    if (newbuffer.getNumChannels() > 1)
    {
        newbuffer.addFrom(0, 0, newbuffer.getReadPointer(1), numS); //sum LR
    }

    //max sample in buffer

    auto mag = newbuffer.getMagnitude(0, numS); //get magnitude
    auto mult = 1 / jlimit<float>(0.00001f, 1.f, mag);  // multiple for gain

    float* w = newbuffer.getWritePointer(0);
    newbuffer.applyGain(0, 0, numS, 1);

    float maxS = mag * mult; // apply gain to max
    maxS > 0.00001 ? maxS = 1 / maxS : maxS = 1; // compare max less than 0

    //sum LR, LPF and rectify
    //float* w = newbuffer.getWritePointer(0);
    for (int i = 0; i < numS; i++)
    {
        w[i] = fabs(w[i]);
        w[i] *= maxS; //get auto gain from max in the buffer
        w[i] = (w[i] * 2) - 1.f; //offset to 0 to -1 and double gain
        visualiserShaperLowFreq.processAudioSample(w[i]); // process Envelope
        nBuffer.setSample(0, i, w[i]);

    }


    newbuffer.clear();
    visualiserLowFreq.pushBuffer(nBuffer);
    nBuffer.clear();

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IceyGateAudioProcessor();
}