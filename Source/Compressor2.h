#pragma once

//#include "AudioDevEffect.h"
#include "EnvelopeShaper2.h"
#include "SidechainEQ.h"

#include <cmath>
#include <algorithm>

class Compressor2 //: public AudioDevEffect
{
public:
    // callback for initializing with samplerate
    void prepareForPlayback(float samplerate, int numSamp, int numChannels) //override
    {
        m_EnvelopeShaper.prepareForPlayback(samplerate);

        sidechainEq2.initFilters(samplerate, numSamp, 1);

        sideChain.setSize(1, numSamp, false, false, false);

        numCh = numChannels;
        numS = numSamp;
    }

    void processBuffer(AudioSampleBuffer& inputBuffer, AudioSampleBuffer& originalBuffer, AudioSampleBuffer& highpassedBuffer, AudioSampleBuffer& bufferForVisual, bool lookahead, bool sc)
    {
        auto w = sideChain.getWritePointer(0);

        auto numsamples = lookahead == true ? originalBuffer.getNumSamples() : inputBuffer.getNumSamples();
        //auto numsamples = inputBuffer.getNumSamples();

        if (numCh > 1)
        {
            for (int i = 0; i < numsamples; i++)
            {
                float L, R;
                if (lookahead == true)
                {
                    int ch;
                    if (sc == true)
                    {
                        ch = 2;
                    }
                    else { ch = 0; }

                    L = originalBuffer.getSample(ch, i);
                    R = originalBuffer.getSample(ch + 1, i);
                }
                else
                {
                    int ch;
                    if (sc == true)
                    {
                        ch = 2;
                    }
                    else { ch = 0; }

                    L = inputBuffer.getSample(ch, i);
                    R = inputBuffer.getSample(ch + 1, i);
                }


                float detectionSignal;
                if (fabs(R) == fmaxf(fabs(L), fabs(R)))
                {
                    detectionSignal = R;
                }
                else { detectionSignal = L; }

                w[i] = detectionSignal;


            }

            sidechainEq2.process(sideChain); //filter sidechain

            for (int i = 0; i < numsamples; i++)
            {
                // apply envelope shaping to detection signal
                float detectionSignal = sideChain.getSample(0, i);

                m_EnvelopeShaper.processAudioSample(detectionSignal);

                // check if the detection signal exceeds the threshold
                detectionSignal = amplitudeToDecibel(detectionSignal);

                w[i] = detectionSignal;
            }

        }
        else
        {
            for (int i = 0; i < numsamples; i++)
            {
                float detectionSignal;
                if (lookahead == true)
                {
                    int ch;
                    if (sc == true)
                    {
                        ch = 1;
                    }
                    else { ch = 0; }

                    detectionSignal = originalBuffer.getSample(ch, i);
                }
                else
                {
                    int ch;
                    if (sc == true)
                    {
                        ch = 1;
                    }
                    else { ch = 0; }

                    detectionSignal = inputBuffer.getSample(ch, i);
                }



                w[i] = detectionSignal;

            }

            sidechainEq2.process(sideChain); //filter sidechain

            for (int i = 0; i < numsamples; i++)
            {
                // apply envelope shaping to detection signal
                float detectionSignal = sideChain.getSample(0, i);

                m_EnvelopeShaper.processAudioSample(detectionSignal);

                // check if the detection signal exceeds the threshold
                detectionSignal = amplitudeToDecibel(detectionSignal);

                w[i] = detectionSignal;
            }
        }


        for (int ch = 0; ch < numCh && ch < 2; ch++)
        {
            float* input = highpassedBuffer.getWritePointer(ch);
            float* inputV = bufferForVisual.getWritePointer(ch);
            for (int i = 0; i < numsamples; i++)
            {
                auto sidechain = sideChain.getSample(0, i);

                //sc = amplitudeToDecibel(sc);

                if (sidechain > m_Threshold)
                {
                    // compute required gain reduction
                    float scale = 1.f - (1.f / m_Ratio);
                    float gain = scale * (m_Threshold - sidechain);


                    gain = decibelToAmplitude(gain);
                    tick = gain;

                    input[i] *= gain;
                    inputV[i] *= gain;

                }
            }
        }

        //sideChain.clear();



    }
    // callback for audio processing
    /*void processAudioSample(float & sample) //override
    {
        // copy signal in detection signal
        float detectionSignal = sample;

        //filter sidechain
        detectionSignal = HPF.processSample(detectionSignal);
        detectionSignal = LPF.processSample(detectionSignal);


        // rectify detection signal
        detectionSignal = fabs(detectionSignal);

        // apply envelope shaping to detection signal
        m_EnvelopeShaper.processAudioSample(detectionSignal);

        // check if the detection signal exceeds the threshold
        detectionSignal = amplitudeToDecibel(detectionSignal);
        if (detectionSignal > m_Threshold)
        {
            // compute required gain reduction
            float scale = 1.f - (1.f / m_Ratio);
            float gain = scale * (m_Threshold - detectionSignal);

            gain = decibelToAmplitude(gain);

            tick = gain; //get tick from Compressor

            // apply gain reduction
            sample *= gain;
        }
    }
    */
    float getTick()
    {

        return tick;
    }
    void setSC(float hpf, float lpf, float peqFreq, float peqGain, double samplerate)
    {

        sidechainEq2.updateCoeffs(hpf, lpf, peqFreq, peqGain);

    }

    // setters for compression parameters
    void setThreshold(float threshold)
    {
        m_Threshold = threshold;
    }

    void setRatio(float ratio)
    {
        m_Ratio = ratio;
    }

    void setAttack(float attack)
    {
        m_EnvelopeShaper.setAttack(attack);
    }

    void setRelease(float release)
    {
        m_EnvelopeShaper.setRelease(release);
    }
    void setHold(float hold)
    {
        m_EnvelopeShaper.setHold(hold);
    }

    

private:
    // compressor private variables
    float m_Threshold;
    float m_Ratio;
    float tick = 1.f;

    SidechainEQ sidechainEq2;

    EnvelopeShaper2 m_EnvelopeShaper;

    AudioSampleBuffer sideChain;
    int numS, numCh;

    const float BOUND_LOG = -96.f;
    const float BOUND_LIN = decibelToAmplitude(BOUND_LOG);

    // transform amplitude to decibel
    float amplitudeToDecibel(float amplitude)
    {
        amplitude = std::max(amplitude, BOUND_LIN);
        return 20.f * log10(amplitude);
    }

    // transform decibel to amplitude
    float decibelToAmplitude(float decibel)
    {
        return pow(10.f, decibel / 20.f);
    }
};