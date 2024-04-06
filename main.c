#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include "brain.h"


int main(void){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "test");

	SetTargetFPS(60);
	//HideCursor();
	
	//srand(time(NULL));

	//int player_x = GetScreenWidth() / 2, player_y = GetScreenHeight() / 2;


	while (!WindowShouldClose()){
		BeginDrawing();


		ClearBackground(RAYWHITE);
		DrawText("test of neat", GetScreenWidth() / 2,  GetScreenHeight()/ 2, 20, BLACK);


		//DrawCircle(player_x, player_y, 10, PURPLE);


		EndDrawing();
	}

}
