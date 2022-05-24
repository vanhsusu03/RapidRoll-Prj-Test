
#include "CommonFunction.h"
#include "BaseObj.h"
#include "TextObj.h"

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

// Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {

            return true;

        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {

            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {

            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {

            return true;
        }
    }

// Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {

            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {

            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {

            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {

            return true;
        }
    }

// Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {

        return true;
    }

    return false;
}

int SDLCommonFunc::ShowMenu(SDL_Renderer* des, TTF_Font* font, Uint32 &time_menu)
{
    BaseObj Menu, Buttom;
    TTF_Font* title_font = TTF_OpenFont("font//karma suture.ttf", 80);

    bool ret_menu = Menu.LoadImg("img//menu.png", des);
    bool ret_buttom = Buttom.LoadImg("img//buttom.png", des);

    if (!ret_menu || !ret_buttom) return -1;

    const int kMenuItemNum = 2;

    TextObj text_menu[kMenuItemNum], title;

    title.SetText("RAPID ROLL");
    title.SetColor(TextObj::WHITE_TEXT);

    text_menu[0].SetText("Play Game");
    text_menu[0].SetColor(TextObj::WHITE_TEXT);
    text_menu[0].rect_.x = 235;
    text_menu[0].rect_.y = 360;
    text_menu[0].LoadFromRenderText(font, des);
    text_menu[0].rect_.h = text_menu[0].GetHeight();
    text_menu[0].rect_.w = text_menu[0].GetWidth();

    text_menu[1].SetText("Exit");
    text_menu[1].SetColor(TextObj::WHITE_TEXT);
    text_menu[1].rect_.x = 280;
    text_menu[1].rect_.y = 480;
    text_menu[1].LoadFromRenderText(font, des);
    text_menu[1].rect_.h = text_menu[0].GetHeight();
    text_menu[1].rect_.w = text_menu[0].GetWidth();

    bool selected[kMenuItemNum] = {0, 0};
    int xm = 0, ym = 0;
    SDL_Event m_event;

    while(1)
    {

        SDL_RenderClear(des);
        Menu.Render(des, NULL);
        for (int i=0; i<kMenuItemNum; i++)
        {
            Buttom.SetRect(190, 345 + 120*i);
            Buttom.Render(des, NULL);
        }
        title.LoadFromRenderText(title_font, des);
        title.RenderText(des, 120, 100);
        for (int i=0; i < kMenuItemNum; i++)
        {
            text_menu[i].LoadFromRenderText(font, des);
            text_menu[i].RenderText(des, text_menu[i].rect_.x, text_menu[i].rect_.y);
        }
        while (SDL_PollEvent(&m_event))
        {
            time_menu = SDL_GetTicks();
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i=0; i<kMenuItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
                        {
                            if (selected[i] == false)
                            {
                                selected[i] = 1;
                                text_menu[i].SetColor(TextObj::RED_TEXT);

                            }
                        }
                        else
                        {
                            if (selected[i] == true)
                            {
                                selected[i] = 0;
                                text_menu[i].SetColor(TextObj::WHITE_TEXT);

                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;

                    for (int i=0; i<kMenuItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
            default:
                break;


            }
        }
        SDL_RenderPresent(des);
    }

    return 0;
}

int SDLCommonFunc::ShowLevel(SDL_Renderer* des, TTF_Font* font, Uint32 &time_Level)
{
    BaseObj Level, Buttom;
    TTF_Font* title_font = TTF_OpenFont("font//karma suture.ttf", 60);

    bool ret_level = Level.LoadImg("img//menu.png", des);
    bool ret_buttom = Buttom.LoadImg("img//buttom.png", des);
    if (!ret_buttom || !ret_level) return -1;


    const int kLevelItemNum = 3;

    TextObj text_Level[kLevelItemNum], title;

    title.SetText("Choose Your Level");
    title.SetColor(TextObj::WHITE_TEXT);

    text_Level[0].SetText("Easy");
    text_Level[0].SetColor(TextObj::WHITE_TEXT);
    text_Level[0].rect_.x = 270;
    text_Level[0].rect_.y = 360;
    text_Level[0].LoadFromRenderText(font, des);
    text_Level[0].rect_.h = text_Level[0].GetHeight();
    text_Level[0].rect_.w = text_Level[0].GetWidth();

    text_Level[1].SetText("Medium");
    text_Level[1].SetColor(TextObj::WHITE_TEXT);
    text_Level[1].rect_.x = 250;
    text_Level[1].rect_.y = 480;
    text_Level[1].LoadFromRenderText(font, des);
    text_Level[1].rect_.h = text_Level[0].GetHeight();
    text_Level[1].rect_.w = text_Level[0].GetWidth();

    text_Level[2].SetText("Hard");
    text_Level[2].SetColor(TextObj::WHITE_TEXT);
    text_Level[2].rect_.x = 270;
    text_Level[2].rect_.y = 600;
    text_Level[2].LoadFromRenderText(font, des);
    text_Level[2].rect_.h = text_Level[0].GetHeight();
    text_Level[2].rect_.w = text_Level[0].GetWidth();

    bool selected[kLevelItemNum] = {0, 0, 0};
    int xm = 0, ym = 0;
    SDL_Event m_event;

    while(1)
    {

        SDL_RenderClear(des);
        Level.Render(des, NULL);
        for (int i=0; i<kLevelItemNum; i++)
        {
            Buttom.SetRect(190, 345 + 120*i);
            Buttom.Render(des, NULL);
        }
        title.LoadFromRenderText(title_font, des);
        title.RenderText(des, 80, 100);
        for (int i=0; i < kLevelItemNum; i++)
        {
            text_Level[i].LoadFromRenderText(font, des);
            text_Level[i].RenderText(des, text_Level[i].rect_.x, text_Level[i].rect_.y);
        }
        while (SDL_PollEvent(&m_event))
        {
            time_Level = SDL_GetTicks();
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i=0; i<kLevelItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_Level[i].GetRect()))
                        {
                            if (selected[i] == false)
                            {
                                selected[i] = 1;
                                text_Level[i].SetColor(TextObj::RED_TEXT);

                            }
                        }
                        else
                        {
                            if (selected[i] == true)
                            {
                                selected[i] = 0;
                                text_Level[i].SetColor(TextObj::WHITE_TEXT);

                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;

                    for (int i=0; i<kLevelItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_Level[i].GetRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
            default:
                break;


            }
        }
        SDL_RenderPresent(des);
    }

    return 0;
}

int SDLCommonFunc::ShowCharacter(SDL_Renderer* des, TTF_Font* font, Uint32 &time_Character)
{
    const int kCharacterItemNum = 3;
    BaseObj Character, Rim, Player[kCharacterItemNum], BigPlayer[kCharacterItemNum];
    TTF_Font* title_font = TTF_OpenFont("font//karma suture.ttf", 55);

    TextObj title;

    title.SetText("Choose Your Character");
    title.SetColor(TextObj::WHITE_TEXT);

    bool ret_Character = Character.LoadImg("img//menu.png", des);
    bool ret_rim = Rim.LoadImg("img//rim.png", des);
    Rim.SetRect(0,300);
    bool ret_p1 = Player[0].LoadImg("img//player.png", des);
    bool ret_p1_big = BigPlayer[0].LoadImg("img//player_big.png", des);
    BigPlayer[0].SetRect(45, 343);
    Player[0].SetRect(50,363);
    bool ret_p2 = Player[1].LoadImg("img//player1.png", des);
    bool ret_p2_big = BigPlayer[1].LoadImg("img//player1_big.png", des);
    BigPlayer[1].SetRect(265, 338);
    Player[1].SetRect(270, 358);
    bool ret_p3 = Player[2].LoadImg("img//player2.png", des);
    bool ret_p3_big = BigPlayer[2].LoadImg("img//player2_big.png", des);
    BigPlayer[2].SetRect(470, 350);
    Player[2].SetRect(475, 370);

    if (!ret_Character || !ret_rim || !ret_p1|| !ret_p2|| !ret_p3) return -1;



    bool selected[kCharacterItemNum] = {0, 0, 0};
    int xm = 0, ym = 0;
    SDL_Event m_event;

    while(1)
    {

        SDL_RenderClear(des);
        Character.Render(des, NULL);
        Rim.Render(des, NULL);
        title.LoadFromRenderText(title_font, des);
        title.RenderText(des, 25, 100);
        while (SDL_PollEvent(&m_event))
        {
            time_Character = SDL_GetTicks();
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i=0; i<kCharacterItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, Player[i].GetRect()))
                        {
                            if (selected[i] == false)
                            {
                                BigPlayer[i].Render(des, NULL);
                                for (int j=0; j <kCharacterItemNum; j++)
                                {
                                    if (j!=i) Player[j].Render(des, NULL);
                                }
                                SDL_RenderPresent(des);
                                selected[i] = 1;
                            }
                        }
                        else
                        {
                            if (selected[i] == true)
                            {
                                //Player[i].Render(des, NULL);
                                for (int j=0; j <kCharacterItemNum; j++)
                                {
                                    Player[j].Render(des, NULL);
                                }
                                SDL_RenderPresent(des);
                                selected[i] = 0;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;

                    for (int i=0; i<kCharacterItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, Player[i].GetRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
            default:
                break;


            }
        }
        //SDL_RenderPresent(des);
    }

    return 0;
}

int SDLCommonFunc::ShowExit(SDL_Renderer* des, TTF_Font* font, Uint32 scores_last, Uint32 high_scores, int ret_character)
{
    BaseObj Exit, Buttom, Character;
    TTF_Font* title_font = TTF_OpenFont("font//karma suture.ttf", 70);
    TTF_Font* title_font2 = TTF_OpenFont("font//karma suture.ttf", 50);

    bool ret_Exit = Exit.LoadImg("img//menu.png", des);
    bool ret_buttom = Buttom.LoadImg("img//buttom.png", des);
    bool ret_char = false;
    switch (ret_character)
    {
    case 0:
        ret_char = Character.LoadImg("img//player_big.png", des);
        break;
    case 1:
        ret_char = Character.LoadImg("img//player1_big.png", des);
        break;
    case 2:
        ret_char = Character.LoadImg("img//player2_big.png", des);
        break;
    }
    Character.SetRect(50, 580);

    if (!ret_Exit || !ret_buttom || !ret_char ) return -1;

    const int kExitItemNum = 2;

    TextObj text_Exit[kExitItemNum], title, scores, high_scr;

    std::string scores_txt = std::to_string(scores_last);
    scores.SetText(scores_txt);
    scores.SetColor(TextObj::WHITE_TEXT);

    title.SetText("Your scores: ");
    title.SetColor(TextObj::WHITE_TEXT);

    std::string high_scores_txt = "High scores: ";
    high_scores_txt += std::to_string(high_scores);
    high_scr.SetText(high_scores_txt);
    high_scr.SetColor(TextObj::WHITE_TEXT);


    text_Exit[0].SetText("Menu");
    text_Exit[0].SetColor(TextObj::WHITE_TEXT);
    text_Exit[0].rect_.x = 275;
    text_Exit[0].rect_.y = 400;
    text_Exit[0].LoadFromRenderText(font, des);
    text_Exit[0].rect_.h = text_Exit[0].GetHeight();
    text_Exit[0].rect_.w = text_Exit[0].GetWidth();

    text_Exit[1].SetText("Exit");
    text_Exit[1].SetColor(TextObj::WHITE_TEXT);
    text_Exit[1].rect_.x = 280;
    text_Exit[1].rect_.y = 520;
    text_Exit[1].LoadFromRenderText(font, des);
    text_Exit[1].rect_.h = text_Exit[0].GetHeight();
    text_Exit[1].rect_.w = text_Exit[0].GetWidth();

    bool selected[kExitItemNum] = {0, 0};
    int xm = 0, ym = 0;
    SDL_Event m_event;

    while(1)
    {

        SDL_RenderClear(des);
        Exit.Render(des, NULL);
        for (int i=0; i<kExitItemNum; i++)
        {
            Buttom.SetRect(190, 385 + 120*i);
            Buttom.Render(des, NULL);
        }
        Character.Render(des, NULL);
        title.LoadFromRenderText(title_font, des);
        title.RenderText(des, 110, 90);
        scores.LoadFromRenderText(title_font, des);
        scores.RenderText(des, 240, 180);
        high_scr.LoadFromRenderText(title_font2, des);
        high_scr.RenderText(des, 100, 280);

        for (int i=0; i < kExitItemNum; i++)
        {
            text_Exit[i].LoadFromRenderText(font, des);
            text_Exit[i].RenderText(des, text_Exit[i].rect_.x, text_Exit[i].rect_.y);
        }
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i=0; i<kExitItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_Exit[i].GetRect()))
                        {
                            if (selected[i] == false)
                            {
                                selected[i] = 1;
                                text_Exit[i].SetColor(TextObj::RED_TEXT);

                            }
                        }
                        else
                        {
                            if (selected[i] == true)
                            {
                                selected[i] = 0;
                                text_Exit[i].SetColor(TextObj::WHITE_TEXT);

                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;

                    for (int i=0; i<kExitItemNum; i++)
                    {
                        if (CheckFocusWithRect(xm, ym, text_Exit[i].GetRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
            default:
                break;


            }
        }
        SDL_RenderPresent(des);
    }

    return 0;
}


bool SDLCommonFunc::CheckFocusWithRect(const int &x, const int &y, const SDL_Rect& rect)
{
    if (x >= rect.x && x < rect.x + rect.w &&
        y >= rect.y && y < rect.y + rect.h)
    {
        return true;
    }
    return false;
}


