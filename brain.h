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

typedef struct Brain{
	int number_of_inputs;
	int number_of_layers;
	int number_of_outpus; 
	vector *layers[];
} Brain;

Brain *create_brain(int n_inputs, int n_layers, int n_outputs, int heights_of_layers[]);
double randn (double mu, double sigma);
vector *make_vector(size_t n, type type_of_array);
vector *copy_vector(vector *original_vector);
vector *forward(vector *input, vector *weights, vector *biases);
vector *activation_relu(vector *output);
vector *softmax_activation_function(vector *output);
int get_move(vector *output);
void mutate_brain(Brain *brain, float mutation_rate);
void free_brain(Brain *brain);
Brain *copy_brain(Brain *brain, int heights_of_layers[]);
vector *pass_through(vector *input, Brain *brain);
void print_vector(vector *vector);
void print_brain(Brain *brain);
double randn (double mu, double sigma);

#endif
