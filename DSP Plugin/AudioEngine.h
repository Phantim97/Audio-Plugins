#pragma once
#include "Voice.h"

//Controls the fx of our voice
class AudioEngine final : public juce::MPESynthesiser
{
private:
	enum chain_idx
	{
		reverb_idx
	};

	juce::dsp::ProcessorChain<juce::dsp::Reverb> fx_chain_;

	void renderNextSubBlock(juce::AudioBuffer<float>& output_audio, const int start_sample, const int num_samples) override
	{	
		MPESynthesiser::renderNextSubBlock(output_audio, start_sample, num_samples);

		const juce::dsp::AudioBlock<float> block = juce::dsp::AudioBlock<float>(output_audio);
		juce::dsp::AudioBlock<float> block_to_use = block.getSubBlock(start_sample, num_samples);
		const juce::dsp::ProcessContextReplacing<float> context_to_use(block_to_use);

		fx_chain_.process(context_to_use);
	}

public:
	static constexpr size_t max_num_voices = 4;

	AudioEngine()
	{
		for (size_t i = 0; i < max_num_voices; i++)
		{
			addVoice(new Voice); //populates array voices inherited from parent class
		}

		setVoiceStealingEnabled(true); //Steal voices if synth runs out
	}

	void prepare(const juce::dsp::ProcessSpec& spec) noexcept
	{
		setCurrentPlaybackSampleRate(spec.sampleRate);

		for (size_t i = 0; i < voices.size(); i++)
		{
			dynamic_cast<Voice*>(voices[i])->prepare(spec);
		}

		fx_chain_.prepare(spec);
	}
};