#pragma once
#include <juce_dsp/juce_dsp.h>

template <typename T>
class AudioBufferQueue
{
public:
	static constexpr size_t order = 9;
	static constexpr size_t buffer_size = 1U << order;
	static constexpr size_t num_buffers = 5;
private:
	juce::AbstractFifo abstract_fifo_{ num_buffers };
	std::array<std::array<T, buffer_size>, num_buffers> buffers_;
public:
	void push(const T* data_to_push, const size_t num_samples)
	{
		jassert(num_samples <= buffer_size);

		int start1;
		int size1;
		int start2;
		int size2;

		//Prep Memory Writing Zone
		abstract_fifo_.prepareToWrite(1, start1, size1, start2, size2);

		//Make sure we are only operating on one block
		jassert(size <= 1);
		jassert(size2 == 0);

		//Perform Write Operation
		if (size1 > 0)
		{
			juce::FloatVectorOperations::copy(buffers_[start1].data(), data_to_push, juce::jmin(buffer_size, num_samples));
		}

		abstract_fifo_.finishedWrite(size1); //Indicate how much has been written to the fifo
	}

	void pop(T* output_buffer)
	{
		//Must be declared as int
		int start1;
		int size1;
		int start2;
		int size2;

		//Prepare block in fifo to be consumed
		abstract_fifo_.prepareToRead(1, start1, size1, start2, size2);

		//Make sure we are only operating on one block
		jassert(size <= 1);
		jassert(size2 == 0);

		if (size1 > 0)
		{
			juce::FloatVectorOperations::copy(output_buffer, buffers_[start1].data(), buffer_size);
		}

		abstract_fifo_.finishedRead(size1); //Indicate that item has been read and consumed
	}
};