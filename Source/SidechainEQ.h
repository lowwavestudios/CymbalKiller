#pragma once

/*
HPF, LPF and Peak Filters for sidechain of processor

by Low Wave Studios

*/

#include <JuceHeader.h>

class SidechainEQ 
{
public:

	void initFilters(float samplerate, int numSamples, int numCh)
	{
		mSampleRate = samplerate;
		spec.sampleRate = mSampleRate;
		spec.maximumBlockSize = numSamples;
		spec.numChannels = numCh;

		hiPassFilter.reset();
		hiPassFilter.prepare(spec);

		lowPassFilter.reset();
		lowPassFilter.prepare(spec);

		peakFilter.reset();
		peakFilter.prepare(spec);

	}
	void updateCoeffs(float HPF, float LPF, float PeqFreq, float PeqGain)
	{
		mHPFFreq = HPF;
		hiPassFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		hiPassFilter.state->setCutOffFrequency(mSampleRate, mHPFFreq, 0.707f);

		mLPFFreq = LPF;
		lowPassFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		lowPassFilter.state->setCutOffFrequency(mSampleRate, mLPFFreq, 0.707f);

		mPeqGain = PeqGain;
		*peakFilter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, PeqFreq, 1, Decibels::decibelsToGain(mPeqGain));
	}

	void process(juce::AudioSampleBuffer& buffer)
	{
		dsp::AudioBlock<float> mBlock(buffer);

		if (mHPFFreq > 20.0f)
		{
			hiPassFilter.process(dsp::ProcessContextReplacing<float>(mBlock));
		}
		if (mLPFFreq < 20000.f)
		{
			lowPassFilter.process(dsp::ProcessContextReplacing<float>(mBlock));
		}
		if (mPeqGain != 0.f)
		{
			peakFilter.process(dsp::ProcessContextReplacing<float>(mBlock));
		}
	}

private:

	float mSampleRate{ 44100 };

	float mHPFFreq{ 20.0f };
	float mLPFFreq{ 20000.f };
	float mPeqGain{ 0.0f };

	dsp::ProcessSpec spec;

	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> hiPassFilter;
	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> lowPassFilter;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> peakFilter;

	//JUCE_LEAK_DETECTOR(SidechainEQ)
};
