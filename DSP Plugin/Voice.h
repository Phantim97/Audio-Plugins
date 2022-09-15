#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

template <typename T>
class CustomOscillator
{
private:
	//Access indicies for each chain index
	enum chain_idx
	{
		osc_idx,
		gain_idx
	};

	//Chains Oscillator and Gain processor together in an array
	juce::dsp::ProcessorChain<juce::dsp::Oscillator<T>, juce::dsp::Gain<T>> processor_chain_;

public:
	CustomOscillator()
	{
		juce::dsp::Oscillator<T>& osc = processor_chain_.template get<osc_idx>();

		//Custom Sine function
		//osc.initialise([] (Type x) { return std::sin (x); }, 128)

		//Custom Sawtooth function
		osc.initialise([](T x)
			{
				return juce::jmap(x,
					T(-juce::MathConstants<double>::pi),
					T(juce::MathConstants<double>::pi),
					T(-1),
					T(1));
			}, 2);
	}

	//Set Oscillator frequency
	void set_frequency(T new_val, bool force = false)
	{
		juce::dsp::Oscillator<T>& osc = processor_chain_.template get<osc_idx>();
		osc.setFrequency(new_val, force);
	}

	//Set gain level
	void set_level(T new_val)
	{
		juce::dsp::Gain<T>& gain = processor_chain_.template get<gain_idx>();
		gain.setGainLinear(new_val);
	}

	void reset() noexcept
	{
		processor_chain_.reset();
	}

	//Note: should probably SFINAE this to context types only
	template <typename ProcessContext>
	void process(const ProcessContext& context) noexcept
	{
		processor_chain_.process(context);
	}

	void prepare(const juce::dsp::ProcessSpec& spec)
	{
		processor_chain_.prepare(spec);
	}
};

class Voice final : public juce::MPESynthesiserVoice
{
private:
	//Mem alloc
	juce::HeapBlock<char> heap_block_;
	juce::dsp::AudioBlock<float> temp_block_;

	//Chain access
	enum chain_idx
	{
		osc1_idx,
		osc2_idx,
		filter_idx,
		gain_idx
	};

	juce::dsp::ProcessorChain<CustomOscillator<float>, CustomOscillator<float>,
		juce::dsp::LadderFilter<float>, juce::dsp::Gain<float>> processor_chain_;

	juce::dsp::Oscillator<float> lfo_;
	static constexpr size_t lfo_update_rate = 100;
	size_t lfo_update_counter_ = lfo_update_rate;

public:
	Voice()
	{
		//Gain Setup
		juce::dsp::Gain<float>& gain = processor_chain_.get<gain_idx>();
		gain.setGainLinear(0.7f);

		//Ladder Filter Setup
		juce::dsp::LadderFilter<float>& filter = processor_chain_.get<filter_idx>();
		filter.setCutoffFrequencyHz(1000.0f);
		filter.setResonance(0.7f); //Amplification of sound waves = 1 of natural freq of its vibration

		//Low Frequency Oscillation
		//(low frequency to create a rhythmic pulse or sweep (i.e tremolo, vibrato, phasing, vibrato)
		lfo_.initialise([](const float x) { return std::sin(x); }, 128);
		lfo_.setFrequency(3.0f);
	}

	//Prepare each processor in the chain sequentially
	void prepare(const juce::dsp::ProcessSpec& spec)
	{
		temp_block_ = juce::dsp::AudioBlock<float>(heap_block_, spec.numChannels, spec.maximumBlockSize);
		processor_chain_.prepare(spec);

		//LFO Prep
		lfo_.prepare({ spec.sampleRate / lfo_update_rate, spec.maximumBlockSize, spec.numChannels });
	}

	void noteStarted() override
	{
		const float velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
		const float freq_hz = getCurrentlyPlayingNote().getFrequencyInHertz();

		//Oscillator 1 setting
		processor_chain_.get<osc1_idx>().set_frequency(freq_hz, true);
		processor_chain_.get<osc1_idx>().set_level(velocity);

		//Oscillator 2 setting
		processor_chain_.get<osc2_idx>().set_frequency(freq_hz * 1.01f, true); //slight variation
		processor_chain_.get<osc2_idx>().set_level(velocity);
	}

	//If the Listener detects a change in the pitch (via MIDI protocol)
	void notePitchbendChanged() override
	{
		const float freq_hz = getCurrentlyPlayingNote().getFrequencyInHertz();
		processor_chain_.get<osc1_idx>().set_frequency(freq_hz);
		processor_chain_.get<osc2_idx>().set_frequency(freq_hz * 1.01f);
	}

	//Just use bool as param because we aren't using the bool
	void noteStopped(bool) override
	{
		clearCurrentNote(); //Clear note from playing
	}

	//Wipe these methods clean
	void notePressureChanged() override {}
	void noteTimbreChanged() override {}
	void noteKeyStateChanged() override {}

	//Render next block of audio data
	void renderNextBlock(juce::AudioBuffer<float>& output_buffer, int start_sample, int num_samples) override
	{
		juce::dsp::AudioBlock<float> output = 
			temp_block_.getSubBlock(0, static_cast<size_t>(num_samples));

		output.clear();

		for (size_t pos = 0; pos < num_samples;)
		{
			//Grab output sub-block
			const size_t max = juce::jmin(num_samples - pos, lfo_update_counter_);
			juce::dsp::AudioBlock<float> block = output.getSubBlock(pos, max);

			juce::dsp::ProcessContextReplacing<float> context(block);
			processor_chain_.process(context);

			pos += max;
			lfo_update_counter_ -= max;

			//update the lfo update counter
			if (lfo_update_counter_ == 0)
			{
				lfo_update_counter_ = lfo_update_rate;
				const float lfo_out = lfo_.processSample(0.0f);
				const float cutoff_freq_hz = juce::jmap(lfo_out, -1.0f, 1.0f, 100.0f, 2000.0f);
				processor_chain_.get<filter_idx>().setCutoffFrequencyHz(cutoff_freq_hz);
			}
		}

		//Add the temp block to the end of the audio buffer
		juce::dsp::AudioBlock<float>(output_buffer).getSubBlock(start_sample, num_samples).add(temp_block_);
	}
};