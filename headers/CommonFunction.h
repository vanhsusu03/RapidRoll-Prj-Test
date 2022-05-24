#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H


#include <windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

//biến xây dựng nên form window cho game
static SDL_Window* g_windows = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Renderer* g_img_Menu = NULL;
static SDL_Event g_event;


// Screen
const int FRAME_PER_SECOND = 32;  // fps
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 832;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;


#define BLANK_TILE 0

#define TILE_SIZE 64

#define MAX_MAP_X 10
#define MAX_MAP_Y 10000

#define STATE_ITEM1 4

#define FIRE_ITEM 9

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

// xử lí va chạm bullet with threats
namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    int ShowMenu(SDL_Renderer* des, TTF_Font* font, Uint32 &time_Menu);
    bool CheckFocusWithRect(const int &x, const int &y, const SDL_Rect &rect);
    int ShowLevel(SDL_Renderer* des, TTF_Font* font, Uint32 &time_Menu);
    int ShowCharacter(SDL_Renderer* des, TTF_Font* font, Uint32 &time_Menu);
    int ShowExit(SDL_Renderer* des, TTF_Font* font, Uint32 scores_last, Uint32 high_scores, int ret_character );

}

#endif // COMMONFUNCTION_H
