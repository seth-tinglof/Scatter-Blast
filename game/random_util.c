#include "pcg_basic.h"

pcg32_random_t rng;
int initialized = 0;

void initRandom() {
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

double randDouble() {
	if (!initialized) {
		initRandom();
		initialized = 1;
	}
	return pcg32_random_r(&rng) / 4294967296.0;
}

int randRange(int i, int f) {
	return (int)(randDouble() * (f - i) + i);
}