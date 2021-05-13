#pragma once

#include <GL/glew.h>
#include <iostream>

#include <signal.h>

// error handling macros
// -----------------------------------------------------------------------------
#define ASSERT(x)                                                                                  \
	if (!(x)) {                                                                                    \
		raise(SIGINT);                                                                             \
	}

#define GL_CALL(x)                                                                                 \
	glClearError();                                                                                \
	x;                                                                                             \
	ASSERT(glLogCall(#x, __FILE__, __LINE__))

static void glClearError() {
	while (glGetError() != GL_NO_ERROR)
		;
}

static bool glLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL ERROR]: " << error << " in " << function << " in " << file << ":"
				  << line << std::endl;
		return false;
	}

	return true;
}
