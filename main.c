#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "brain.h"


int main(void){
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


	while (!WindowShouldClose()){
		BeginDrawing();


		ClearBackground(RAYWHITE);
		DrawRectangleV(rectangle_vector, rectangle_vector_size, BLACK);


		DrawCircle(player_x, player_y, 10, PURPLE);
		DrawCircle(goal_x, goal_y, 30, YELLOW);


		EndDrawing();
	}

}
