class Color {
  private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

  public:
	Color();
	Color(unsigned char red, unsigned char green, unsigned char blue,
		  unsigned char alpha);

	~Color();

	void set(unsigned char red, unsigned char green, unsigned char blue,
			 unsigned char alpha);
};
