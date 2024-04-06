#ifndef BRAIN_H
#define BRAIN_H
#include <stdlib.h>

typedef struct vector {
	size_t length;
	double array[];
} vector;

typedef	enum type {
	weights,
	biases,
	empty
} type;

double randn (double mu, double sigma);
vector *make_vector(size_t n, type type_of_array);
vector *copy_vector(vector *original_vector);
vector *forward(vector *input, vector *weights, vector *biases);
vector *activation_relu(vector *output);
vector *softmax_activation_function(vector *output);
int get_move(vector *output);

#endif
