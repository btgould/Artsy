#pragma once

#include <stdlib.h>

class Texture {
  private:
	u_int32_t width;
	u_int32_t height;
	u_int32_t numComps; // components per pixel
	void* pixelData; // array of all pixel data
};
