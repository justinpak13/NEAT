#include "raylib.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "brain.h"
#include <stdbool.h>
#include <float.h>

typedef struct Player{
	int x;
	int y;
	double average_score;
	int number_of_touches;
	bool dead;
	Brain *brain;
	
} Player;

Player *create_player(int player_x, int player_y, Brain *brain){
	Player *player = malloc(sizeof(Player) + sizeof(int) * 2 + sizeof(*brain));

	player -> x = player_x;
	player -> y = player_y;
	player -> brain = brain;
	player -> average_score = 0;
	player -> number_of_touches = 0;

	return player;
}

void free_player(Player *player){
	free_brain(player -> brain);
}

vector *get_inputs(Player *player, int goal_x, int goal_y, Vector2 rectangle_vector, Vector2 rectangle_vector_size, double total_width, double total_height){
	vector *result = make_vector(16, empty);

	result -> array[0] = player -> x / total_width;
	result -> array[1] = player -> y / total_height;

	result -> array[2] = goal_x / total_width;
	result -> array[3] = goal_y / total_height;

	result -> array[4] = rectangle_vector.x / total_width;
	result -> array[5] = rectangle_vector.y / total_height;

	result -> array[6] = rectangle_vector_size.x / total_width;
	result -> array[7] = rectangle_vector_size.y / total_height;

	result -> array[8] = player -> x;
	result -> array[9] = player -> y;

	result -> array[10] = goal_x;
	result -> array[11] = goal_y;

	result -> array[12] = rectangle_vector.x;
	result -> array[13] = rectangle_vector.y;

	result -> array[14] = rectangle_vector_size.x;
	result -> array[15] = rectangle_vector_size.y;

	return result;

}

void move_player(int move, Player *player){
	if (move == 0){
		player -> x += 1;
	}

	if (move == 1){

		player -> x -= 1;
	}
	
	if (move == 2){
		player -> y += 1;
	}

	if (move == 3){
		player -> y -= 1;
	}
}

double get_distance(Player *player, int goal_x, int goal_y){
	return sqrt(pow(goal_x - player -> x, 2.0) + pow(goal_y - player -> y, 3.0));
}

int calculate_score(Player *player, int goal_x, int goal_y){
	int points = -pow(get_distance(player,goal_x, goal_y), 2);
	if (points == 0 ){
		if (player -> number_of_touches == 0){
			points = 500;
		}
		points += 10 * player -> number_of_touches;
	}

	return points;
}

double calculate_mutation_rate(double player_score){
	if (player_score > -100){
		if (player_score > -50){
			return -(1/player_score) / 500;
		}
		return -(1/player_score) / 100;
	}
	return log10(-player_score) / log10(15) / 100;
}

int main(void){
	int number_of_players = 200;
	float mutation_rate = 0.50; // represent 50%

	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	
	int width = 1000;
	int height = 800;
	InitWindow(width, height, "NEAT");

	SetTargetFPS(700);
	//HideCursor();
	
	srand(time(NULL));

	// need to generate the player somewhere in the lower left quarter of the screen 
	int player_x = width * (rand() % 25 / 100.0), player_y = height * (100 - (rand() % 25)) / 100;

	// need to generate the goal somewhere in the upper right quarter of the screen 
	int goal_x =  width * (100 - (rand() % 25)) / 100, goal_y = height * (rand() % 25 / 100.0);


	// width and height between 0 and 60%
	
	
	int rectangle_width = (rand() % 60) / 100.0 * width + 1;
	int rectangle_height = (rand() % 60) / 100.0 * height + 1;
	
	
	Vector2 rectangle_vector = (Vector2) {width / 2.0 - rectangle_width / 2.0, height / 2.0 - rectangle_height / 2.0};
	Vector2 rectangle_vector_size = (Vector2) {rectangle_width, rectangle_height};


	int heights[4] = {16, 10, 4};

	Brain *best_brain = create_brain(16, 3, 4, heights);

	// create brain list
	Brain *brain_list[number_of_players];
	for (int i = 0; i < number_of_players; i++){
		brain_list[i] = copy_brain(best_brain, heights);
		mutate_brain(brain_list[i], mutation_rate);
	}

	// create player list
	Player *player_list[number_of_players];
	
	for (int i = 0; i < number_of_players; i++){
		player_list[i] = create_player(player_x, player_y, brain_list[i]);
		player_list[i] -> average_score =-INT_MAX;
	}


	int best_player = 0;
	int best_score = -INT_MAX;


	int generation = 0;
	clock_t begin = clock();
	while (!WindowShouldClose()){
		BeginDrawing();



		ClearBackground(RAYWHITE);
		DrawRectangleV(rectangle_vector, rectangle_vector_size, BLACK);
		DrawCircle(goal_x, goal_y, 30, YELLOW);

		DrawText(TextFormat("Generation: %d", generation), 100, 100, 40, BLACK);


		bool empty_check = true;

		for (int i = 0; i < number_of_players; i++){
			if (!player_list[i] -> dead){
				empty_check = false;
				DrawCircle(player_list[i] -> x, player_list[i] -> y, 10, PURPLE);

				vector* inputs = get_inputs(player_list[i], goal_x, goal_y, rectangle_vector, rectangle_vector_size, width, height);
				vector *results = pass_through(inputs, player_list[i] -> brain);
				int move = get_move(results);
				move_player(move, player_list[i]);
				free(inputs);
				free(results);

				player_list[i] -> average_score = (player_list[i] -> average_score +  calculate_score(player_list[i], goal_x,  goal_y)) / 2; 

				if (player_list[i] -> x < 0 || player_list[i] -> y < 0 || player_list[i] -> x > width || player_list[i] -> y > height){
					player_list[i] -> average_score -= 15000;
					player_list[i] -> dead = true;
				}

				if (player_list[i] -> x  > rectangle_vector.x 
					&& player_list[i] -> x <  rectangle_vector.x + rectangle_vector_size.x 
					&&player_list[i] -> y  > rectangle_vector.y 
					&& player_list[i] -> y <  rectangle_vector.y + rectangle_vector_size.y){
					player_list[i] -> average_score -= 15000;
					player_list[i] -> dead = true;
				}
			}
		}
		clock_t end = clock();
		if (((double) end - (double)begin) / CLOCKS_PER_SEC >=0.8){
			empty_check = true;
		} 

		if (IsKeyPressed(KEY_K)) {
			empty_check = true;
		}

		if (empty_check){
			for (int i = 0; i < number_of_players; i++){
				if (player_list[i] -> average_score > best_score){
					printf("Player # %d Score: %f\n", i, player_list[i] -> average_score);
					best_score = player_list[i] -> average_score;
					best_brain = player_list[i] -> brain;
					best_player = i;
				}
			}

			brain_list[0] = copy_brain(best_brain, heights);
			mutation_rate = calculate_mutation_rate(best_score);
			printf("Best Score: %d\n",  best_score);
			printf("Mutation Rate: %f\n",  mutation_rate);
			for (int i = 1; i < number_of_players; i++){
				brain_list[i] = copy_brain(best_brain, heights);
				mutate_brain(brain_list[i], mutation_rate);
			}

			for (int i = 0; i < number_of_players; i++){
				player_list[i] = create_player(player_x, player_y, brain_list[i]);
			}
			generation++;
			begin = clock();

			// reseting the game objects
			//player_x = width * (rand() % 25 / 100.0), player_y = height * (100 - (rand() % 25)) / 100;
			//goal_x =  width * (100 - (rand() % 25)) / 100, goal_y = height * (rand() % 25 / 100.0);
			//rectangle_width = (rand() % 60) / 100.0 * width + 1;
			//rectangle_height = (rand() % 60) / 100.0 * height + 1;
			//rectangle_vector = (Vector2) {width / 2.0 - rectangle_width / 2.0, height / 2.0 - rectangle_height / 2.0};
			//rectangle_vector_size = (Vector2) {rectangle_width, rectangle_height};


		}

		EndDrawing();
	}
	free_brain(best_brain);
	for (int i = 0; i < number_of_players; i++){
		free_brain(brain_list[i]);
	}

}
