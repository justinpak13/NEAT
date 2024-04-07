#include "raylib.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "brain.h"
#include <stdbool.h>

typedef struct Player{
	int x;
	int y;
	int average_score;
	bool dead;
	Brain *brain;
	
} Player;

Player *create_player(int player_x, int player_y, Brain *brain){
	Player *player = malloc(sizeof(Player) + sizeof(int) * 2 + sizeof(*brain));

	player -> x = player_x;
	player -> y = player_y;
	player -> brain = brain;

	return player;
}

void free_player(Player *player){
	free_brain(player -> brain);
}

vector *get_inputs(Player *player, int goal_x, int goal_y, Vector2 rectangle_vector, Vector2 rectangle_vector_size, double total_width, double total_height){
	vector *result = make_vector(8, empty);

	result -> array[0] = player -> x / total_width;
	result -> array[1] = player -> y / total_height;

	result -> array[2] = goal_x / total_width;
	result -> array[3] = goal_y / total_height;

	result -> array[4] = rectangle_vector.x / total_width;
	result -> array[5] = rectangle_vector.y / total_height;

	result -> array[6] = rectangle_vector_size.x / total_width;
	result -> array[7] = rectangle_vector_size.y / total_height;

	return result;

}

void move_player(int move, Player *player){
	if (move == 0){
		player -> x += 3;
	}

	if (move == 1){

		player -> x -= 3;
	}
	
	if (move == 2){
		player -> y += 3;
	}

	if (move == 3){
		player -> y -= 3;
	}
}

double get_distance(Player *player, int goal_x, int goal_y){
	return sqrt(pow(goal_x - player -> x, 2.0) + pow(goal_y - player -> y, 2.0));
}

int calculate_score(Player *player, int goal_x, int goal_y){
	int points = -pow(get_distance(player,goal_x, goal_y), 1.4);
	if (points == 0){
		points = 100;
	}

	return points;


}

int main(void){
	int number_of_players = 100;
	float mutation_rate = 0.10; // represent 10%

	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	
	int width = 1000;
	int height = 800;
	InitWindow(width, height, "NEAT");

	SetTargetFPS(60);
	//HideCursor();
	
	srand(time(NULL));

	// need to generate the player somewhere in the lower left quarter of the screen 
	int player_x = width * (rand() % 25 / 100.0), player_y = height * (100 - (rand() % 25)) / 100;
	printf("Width: %d\n", player_x);
	printf("Height: %d\n", player_y);

	// need to generate the goal somewhere in the upper right quarter of the screen 
	int goal_x =  width * (100 - (rand() % 25)) / 100, goal_y = height * (rand() % 25 / 100.0);

	printf("Width: %d\n", goal_x);
	printf("Height: %d\n", goal_y);

	// width and height between 0 and 60%
	
	
	int rectangle_width = (rand() % 60) / 100.0 * width;
	int rectangle_height = (rand() % 60) / 100.0 * height;
	
	
	Vector2 rectangle_vector = (Vector2) {width / 2.0 - rectangle_width / 2.0, height / 2.0 - rectangle_height / 2.0};
	Vector2 rectangle_vector_size = (Vector2) {rectangle_width, rectangle_height};


	int heights[4] = {8, 4};

	Brain *best_brain = create_brain(8, 2, 4, heights);

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

				player_list[i] -> average_score = (player_list[i] -> average_score +calculate_score(player_list[i], goal_x,  goal_y)) / 2; 

				if (player_list[i] -> x < 0 || player_list[i] -> y < 0 || player_list[i] -> x > width || player_list[i] -> y > height){
					player_list[i] -> average_score -= 1000;
					player_list[i] -> dead = true;
				}

				if (player_list[i] -> x  > rectangle_vector.x 
					&& player_list[i] -> x <  rectangle_vector.x + rectangle_vector_size.x 
					&&player_list[i] -> y  > rectangle_vector.y 
					&& player_list[i] -> y <  rectangle_vector.y + rectangle_vector_size.y){
					player_list[i] -> average_score -= 1000;
					player_list[i] -> dead = true;
				}
			}
		}

		if (IsKeyPressed(KEY_K)) {
			empty_check = true;
		}

		if (empty_check){
			for (int i = 0; i < number_of_players; i++){
				if (player_list[i] -> average_score > best_score){
					printf("Player # %d Score: %d\n", i, player_list[i] -> average_score);
					best_score = player_list[i] -> average_score;
					best_brain = player_list[i] -> brain;
					best_player = i;
				}
			}
			//printf("Best Player: %d\n", best_player);
			//printf("Best score: %d\n", best_score);
			//printf("Mutation Rate: %f\n", log(-best_score));

			//printf("Winning Brain: \n");
			//print_brain(best_brain);

			brain_list[0] = copy_brain(best_brain, heights);
			for (int i = 1; i < number_of_players; i++){
				brain_list[i] = copy_brain(best_brain, heights);
				mutate_brain(brain_list[i], log10(-best_score) / (100));
				printf("Mutation Score: %f\n",  log10(-best_score) / (100));
			}

			for (int i = 0; i < number_of_players; i++){
				player_list[i] = create_player(player_x, player_y, brain_list[i]);
				player_list[i] -> average_score = -INT_MAX;
			}
			generation++;

		}

		EndDrawing();
	}
	free_brain(best_brain);
	for (int i = 0; i < number_of_players; i++){
		free_brain(brain_list[i]);
	}

}
