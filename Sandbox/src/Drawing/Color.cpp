#include "Drawing/Color.hpp"

Color::Color() : r(255), g(255), b(255), a(255) {}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	: r(red), g(green), b(blue), a(alpha) {}

Color::~Color() {}

void Color::set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

void Color::set(Color other) {
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
}
