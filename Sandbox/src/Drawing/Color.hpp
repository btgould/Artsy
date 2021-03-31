#pragma once

class Color {
  private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

  public:
	Color();
	Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

	~Color();

	void set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	void set(Color other);

	Color operator-(const Color& other) {
		Color color =
			Color(this->r - other.r, this->g - other.g, this->b - other.b, this->a - other.a);
		return color;
	}

	Color operator+(const Color& other) {
		Color color =
			Color(this->r + other.r, this->g + other.g, this->b + other.b, this->a + other.a);
		return color;
	}

	bool operator==(const Color& other) {
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}

	bool operator!=(const Color& other) {
		return !(*this == other);
	}
};
