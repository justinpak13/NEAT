#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "brain.h"
#include <time.h>

typedef struct Brain{
	int number_of_inputs;
	int number_of_layers;
	int number_of_outpus; 
	vector *layers[];
} Brain;


Brain *create_brain(int n_inputs, int n_layers, int n_outputs, int heights_of_layers[]){
	Brain *brain = malloc(sizeof(Brain) + sizeof(int) * 3 +  sizeof(vector) * 2 * n_layers);

	brain -> number_of_inputs = n_inputs;
	brain -> number_of_layers = n_layers;
	brain -> number_of_outpus = n_outputs;

	for (int i = 0; i < n_layers; i++){
		int inputs;
		if (i == 0){
			inputs = n_inputs;
		} else {
			inputs = heights_of_layers[i-1];
		}
		brain -> layers[i*2] = make_vector(inputs * heights_of_layers[i], weights);
		brain -> layers[i*2 + 1] = make_vector(heights_of_layers[i], biases);

	}
	return brain;
}

void mutate_brain(Brain *brain, int mutation_rate){
	for (int i = 0; i < brain -> number_of_layers * 2; i++){
		for (int j = 0; j < brain -> layers[i] -> length; j++){
			if (rand() % 100 <= mutation_rate ){
				brain -> layers[i] -> array[j] = randn(0.0, 1.0);
			}
		}
	}
}

void free_brain(Brain *brain){
	for (int i = 0; i < brain -> number_of_layers * 2; i++){
		free(brain -> layers[i]);
	}
	free(brain);
}

Brain *copy_brain(Brain *brain, int heights_of_layers[]){
	Brain *new_brain = create_brain(brain -> number_of_inputs, brain -> number_of_layers, brain -> number_of_outpus , heights_of_layers);

	for (int i = 0; i < brain -> number_of_layers * 2; i++){
		for (int j = 0; j < brain -> layers[i] -> length; j++){
			new_brain -> layers[i] -> array[j] = brain -> layers[i] -> array[j];
		}
	}

	return new_brain;
}

vector *pass_through(vector *input, Brain *brain){
	if (input -> length != brain -> number_of_inputs ){
		printf("Input vector and brain do not match!");
	}

	vector *current_input = copy_vector(input);
	int i = 0;
	for (; i < brain -> number_of_layers -1; i++){
		vector *forward_pass = forward(current_input, brain -> layers[i*2], brain -> layers[i*2 + 1]);
		free(current_input);
		current_input = activation_relu(forward_pass);
		free(forward_pass);
	}

	vector *forward_pass = forward(current_input, brain -> layers[i*2], brain -> layers[i*2 + 1]);
	vector *result = softmax_activation_function(forward_pass);

	free(current_input);
	free(forward_pass);
	return result;
}

void print_vector(vector *vector){
	for (int i = 0; i < vector -> length; i++){
		printf("%f ", vector -> array[i]);
	}
	printf("\n");
}

void print_brain(Brain *brain){
	printf("number of inputs: %d\n", brain -> number_of_inputs);
	printf("number of layers: %d\n", brain -> number_of_layers);
	printf("number of outputs: %d\n", brain -> number_of_outpus);

	for (int i = 0; i < brain -> number_of_layers * 2; i++){
		print_vector(brain -> layers[i]);
		printf("\n");
	}

}



double randn (double mu, double sigma){
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}

vector *make_vector(size_t n, type type_of_array){
	vector *v = malloc(sizeof(*v) + n * sizeof(v-> array[0]));
	v -> length = n;

	// if creating weights matrix 
	if (type_of_array == weights){
		for (size_t i = 0; i < n; i++){
			v -> array[i] = randn(0, 1);
		}
	
	// if creating biases array
	} else if (type_of_array == biases){
		for (size_t i = 0; i < n; i++){
			v -> array[i] = 0;
		}
	} 

	return v;
}


vector *copy_vector(vector *original_vector){
	vector *v = malloc(sizeof(*v) + original_vector -> length * sizeof(v -> array[0]));

	v -> length = original_vector -> length;

	for (int i = 0; i < original_vector->length; i++){
		v -> array[i] = original_vector -> array[i];
	}

	return v;
}

vector *forward(vector *input, vector *weights, vector *biases){
	vector *output = make_vector(biases -> length, empty);

	int matrix_width = input -> length;
	int matrix_height = weights -> length / input -> length;

	for (int i = 0; i < matrix_height; i++){
		double output_value = biases -> array[i];
		for (int j = 0; j < matrix_width; j++){
			output_value += input -> array[j] * (weights -> array[i * matrix_width + j]);  
		}
		output -> array[i] = output_value;
	}


	return output;

}

vector *activation_relu(vector *output){
	vector *result = make_vector(output -> length, empty);

	for (int i = 0; i < output -> length; i++){
		if (output -> array[i] < 0){
			result -> array[i] = 0;
		} else {
			result -> array[i] = output -> array[i];
		}
	}

	return result;
}

vector *softmax_activation_function(vector *output){
	vector *result = make_vector(output -> length, empty);
	double sum = 0;

	for (int i = 0; i < output -> length; i++){
		sum += exp(output -> array[i]);
	}


	for (int i = 0; i < output -> length; i++){
		result -> array[i] = exp(output -> array[i]) / sum;
	}

	return result;
}

int get_move(vector *output){
	double max_value = 0;
	int max_index = 0;;

	for (int i = 0; i < output -> length; i++){
		printf("%f ", output -> array[i]);
		if (output -> array[i] > max_value){
			max_value = output -> array[i];
			max_index = i;
		}
	}
	return max_index;
}

//int main(void){
//	srand(time(NULL));
//	int heights[4] = {5, 5, 3, 2};
//	int mutation_rate = 10; //represents 10%
//	int number_of_brains = 50;
//
//	vector *inputs0 = make_vector(10, weights);
//
//	Brain *brain0 = create_brain(10, 4, 4, heights);
//
//	Brain *brain_list[number_of_brains];
//
//	for (int i = 0; i < number_of_brains; i++){
//		brain_list[i] = copy_brain(brain0, heights);	
//		mutate_brain(brain_list[i], mutation_rate);
//	}
//
//	for (int i = 0; i < number_of_brains; i++){
//		vector *result = pass_through(inputs0, brain_list[i]);
//		print_vector(result);
//		free(result);
//	}
//
//	free_brain(brain0);
//	for (int i = 0; i < number_of_brains; i++){
//		free_brain(brain_list[i]);
//	}
//	free(inputs0);
//}
