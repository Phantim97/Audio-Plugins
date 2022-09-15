#pragma once
#include "AudioBufferQueue.h"


template <typename T>
class ScopeDataCollector
{
private:
	AudioBufferQueue<T>& audio_buffer_queue_;
	std::array<T, AudioBufferQueue<T>::buffer_size> buffer_;
	size_t num_collected_;
	T prev_sample_ = static_cast<T>(100);
	static constexpr float trigger_level = static_cast<T>(0.05);

	enum class State
	{
		waiting_for_trigger,
		collecting
	};

	State state_ = State::waiting_for_trigger;

public:
	ScopeDataCollector(AudioBufferQueue<T>& queue_to_use) : audio_buffer_queue_(queue_to_use)
	{
	}

	void process(const T* data, const size_t num_samples)
	{
		size_t idx = 0;

		//Waiting for trigger to collect samples
		if (state_ == State::waiting_for_trigger)
		{
			while (idx++ < num_samples)
			{
				T current_sample = *data++;

				if (current_sample >= trigger_level && prev_sample_ < trigger_level)
				{
					num_collected_ = 0;
					state_ = State::collecting; //start collecting phase
					break;
				}

				//Iterate
				prev_sample_ = current_sample;
			}
		}

		if (state_ == State::collecting)
		{
			while (idx++ < num_samples)
			{
				buffer_[num_collected_++] = *data++;

				if (num_collected_ == buffer_.size())
				{
					audio_buffer_queue_.push(buffer_.data(), buffer_.size());
					state_ = State::waiting_for_trigger;
					prev_sample_ = T(100);
					break;
				}
			}
		}
	}
};

template <typename T>
class ScopeComponent : public juce::Component, juce::Timer
{
private:
	AudioBufferQueue<T>& audio_buffer_queue_;
	std::array<T, AudioBufferQueue<T>::buffer_size> sample_data_;

	juce::dsp::FFT fft_{ AudioBufferQueue<T>::order };
	using WindowFunc = juce::dsp::WindowingFunction<T>;
	WindowFunc win_func_ { static_cast<size_t>(fft_.getSize()), WindowFunc::hann};
	std::array<T, 2 * AudioBufferQueue<T>::buffer_size> spectrum_data_;

	void timerCallback() override
	{
		audio_buffer_queue_.pop(sample_data_.data());
		juce::FloatVectorOperations::copy(spectrum_data_.data(), sample_data_.data(), sample_data_.size());

		size_t fft_size = fft_.getSize();

		//Apply the FFT on our spectrum data
		jassert(spectrum_data_.size() == 2 * fft_size);
		win_func_.multiplyWithWindowingTable(spectrum_data_.data(), fft_size);
		fft_.performFrequencyOnlyForwardTransform(spectrum_data_.data());

		static constexpr T min_db = T(-160);
		static constexpr T max_db = T(0);

		for (size_t i = 0; i < spectrum_data_.size(); i++)
		{
			spectrum_data_[i] = juce::jmap(juce::jlimit(min_db, max_db, juce::Decibels::gainToDecibels(spectrum_data_[i]) - juce::Decibels::gainToDecibels(T(fft_size))), min_db, max_db, T(0), T(1));
		}

		//Repaint the window
		repaint();
	}

	static void plot(const T* data, size_t num_samples, juce::Graphics& g, juce::Rectangle<T> rect,
		T scalar = T(1), T offset = T(0))
	{
		T width = rect.getWidth();
		T height = rect.getHeight();
		T right = rect.getRight();

		T center = rect.getBottom() - offset;
		T gain = height * scalar;

		for (size_t i = 1; i < num_samples; i++)
		{
			g.drawLine({ juce::jmap(T(i - 1), T(0), T(num_samples - 1), T(right - width), T(right)),
						  center - gain * data[i - 1],
						  juce::jmap(T(i), T(0), T(num_samples - 1), T(right - width), T(right)),
						  center - gain * data[i] });
		}
	}
public:

	ScopeComponent(AudioBufferQueue<T>& queue_to_use) : audio_buffer_queue_(queue_to_use)
	{
		sample_data_.fill(T(0));
		set_fps(30);
	}

	void set_fps(const int fps)
	{
		jassert(fps > 0 && fps < 1000);
		startTimerHz(fps);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::black);
		g.setColour(juce::Colours::white);

		const juce::Rectangle<int> area = getLocalBounds();
		const int h = area.getHeight();
		int w = area.getWidth();

		//Oscilloscope
		juce::Rectangle<T> scoped_rect = juce::Rectangle<T>(0, 0, w, h / 2);
		plot(sample_data_.data(), sample_data_.size(), g, scoped_rect, 1, h / 4);

		//Spectrum
		juce::Rectangle<T> spectrum_rect = juce::Rectangle<T>(0, h / 2, w, h / 2);
		plot(spectrum_data_.data(), spectrum_data_.size() / 4, g, spectrum_rect);
	}

	void resized() override {}
};