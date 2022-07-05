#ifndef __HEADER_H__
#define __HEADER_H__

#include <GL/freeglut.h>
// #include <windows.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#include <string>
#include <cstring>
using namespace std;

#define PI 3.141592

#define FPS 60
#define GRAVITY 0.4f

#define CHARACTER_SPEED 2

#define DEFAULT_WINDOW_WIDTH 1600
#define DEFAULT_WINDOW_HEIGHT 900
#define ROTATION_SENSITIVITY 0.1

#define MENU_WIDTH 350
#define PREVIEW_WIDTH 300

#define BLOCK_111 0
#define BLOCK_121 1
#define BLOCK_141 2
#define BLOCK_441 3
#define BLOCK_222 4
#define BLOCK_242 5

#define BLOCK_111_IMG_PATH "./resources/111.bmp"
#define BLOCK_121_IMG_PATH "./resources/121.bmp"
#define BLOCK_141_IMG_PATH "./resources/141.bmp"
#define BLOCK_441_IMG_PATH "./resources/441.bmp"
#define BLOCK_222_IMG_PATH "./resources/222.bmp"
#define BLOCK_242_IMG_PATH "./resources/242.bmp"

#define COLOR_RED_IMG_PATH "./resources/red_color.bmp"
#define COLOR_ORANGE_IMG_PATH "./resources/orange_color.bmp"
#define COLOR_YELLOW_IMG_PATH "./resources/yellow_color.bmp"
#define COLOR_GREEN_IMG_PATH "./resources/green_color.bmp"
#define COLOR_SKYBLUE_IMG_PATH "./resources/skyblue_color.bmp"
#define COLOR_BLUE_IMG_PATH "./resources/blue_color.bmp"
#define COLOR_PINK_IMG_PATH "./resources/pink_color.bmp"
#define COLOR_PURPLE_IMG_PATH "./resources/purple_color.bmp"
#define COLOR_WHITE_IMG_PATH "./resources/white_color.bmp"
#define COLOR_BLACK_IMG_PATH "./resources/black_color.bmp"

struct Vec3f {
	float x, y, z;
};

struct Vec2f {
	float x, y;
};

#endif