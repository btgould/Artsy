#include "ByteBuffer.hpp"

#include <stdlib.h> // malloc
#include <cstring>	// memset

ByteBuffer::ByteBuffer(uint32_t cap) : capacity(cap), size(0), position(0) {
	data = (uint8_t*) malloc(cap);
	memset(data, 0, cap);
}

ByteBuffer::~ByteBuffer() {
	free(data);
}

void ByteBuffer::write(void* toWrite, int sz) {
	unsigned int neededSize = position + sz;
	bool reallocNeeded = false;

	// increase size to accomodate write if necessecary
	if (neededSize > capacity) {
		capacity = (capacity) ? capacity * 2 : DEFAULT_CAPACITY;
		reallocNeeded = true;
	}

	while (capacity < neededSize)
		capacity *= 2;

	// resize if needed
	if (reallocNeeded)
		data = (uint8_t*) realloc(data, capacity);

	// write data
	memcpy(data + position, toWrite, sz);
	position += sz;
	size += sz;
}
