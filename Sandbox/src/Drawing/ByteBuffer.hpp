#pragma once

#include <cstdint>

class ByteBuffer {
  private:
	static const unsigned int DEFAULT_CAPACITY = 1024;

	uint8_t* data;	   // pointer to the actual data in the buffer
	uint32_t position; // position currently being read in the buffer
	uint32_t size;	   // amount of data in the buffer that is filled
	uint32_t capacity; // amout of data the buffer can hold

  public:
	ByteBuffer(uint32_t size = DEFAULT_CAPACITY);
	~ByteBuffer();

	void write(void* data, int size);
};
