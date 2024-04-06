#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include "brain.h"


int main(void){
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	
	int width = 1000;
	int height = 800;
	InitWindow(width, height, "NEAT");

	SetTargetFPS(60);
	//HideCursor();
	
	//srand(time(NULL));

	int player_x = width * 0.1, player_y = height * 0.9;
	int goal_x =  width * 0.9, goal_y = height * 0.1;


	while (!WindowShouldClose()){
		BeginDrawing();


		ClearBackground(RAYWHITE);
		DrawRectangle(width / 2, height / 2, 300, 300, BLACK);


		DrawCircle(player_x, player_y, 10, PURPLE);
		DrawCircle(goal_x, goal_y, 30, YELLOW);


		EndDrawing();
	}

}
