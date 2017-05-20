#ifndef IDS_GENERATOR
#define IDS_GENERATOR 

struct ids_generator {
public:
	ids_generator() : counter(0) {}

	unsigned long long next() {
		return counter++;
	}

private:
	unsigned long long counter;
};

#endif