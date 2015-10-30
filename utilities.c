#include <stdlib.h>
#include <math.h>
#include <time.h>

float* getRandFloatArray(int size, float upperLimit, float lowerLimit) {
	float* randoms = malloc(size * sizeof(float));
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		randoms[i] = (float)rand() / (float)(RAND_MAX / (upperLimit - lowerLimit)) + lowerLimit;
	}
	return randoms;
}
