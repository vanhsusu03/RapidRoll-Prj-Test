
#include "CommonFunction.h"
#include "BaseObj.h"
#include "game_map.h"
#include "MainObj.h"
#include "ImpTimer.h"
#include "BulletObj.h"
#include "ThreatsObj.h"
#include "ExplosionObj.h"
#include "TextObj.h"
#include <iostream>

BaseObj g_background, num_lifes;
TTF_Font* scores_font = NULL;


bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_windows = SDL_CreateWindow("RAPID ROLL",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);

    if (g_windows == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_windows, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }


    scores_font = TTF_OpenFont("font//karma suture.ttf", 35);
    if (scores_font == NULL)
    {
        std::cout<<"Null font" << TTF_GetError();
        success = false;
    }

    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.jpg", g_screen);
    bool retLifes = num_lifes.LoadImg("map//4.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_windows);
    g_windows = NULL;

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObj*> MakeThreadList()    // hàm tạo ra các loại threats
{
    std::vector<ThreatsObj*> list_threats;

// sinh quái động
    ThreatsObj* dynamic_threats = new ThreatsObj[200];
    for (int i = 0; i < 200; i++)
    {
        ThreatsObj* p_threat = (dynamic_threats + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObj::MOVE_IN_SPACE_THREAT);
            int threat_x_pos[3] ={64, 320, 640-64*2};
            p_threat->set_x_pos(threat_x_pos[i%3]);
            if (i<20) p_threat->set_y_pos(400+i*3000);
            else if (i<50) p_threat->set_y_pos(60400+(i-20)*1500);
            else p_threat->set_y_pos(105400+(i-50)*500);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);

            list_threats.push_back(p_threat);            // p_threat cùng trỏ tới tĩnh và động

        }
    }

// sinh quái tĩnh
    ThreatsObj* threats_objs = new ThreatsObj[200];

    for (int i = 0; i < 200; i++)
    {
        ThreatsObj* p_threat = (threats_objs + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_level.png", g_screen);
            p_threat->set_clips();
            int threat_x_pos[3] ={64, 320, 640-64*2};
            p_threat->set_x_pos(threat_x_pos[i%3]);
            if (i<20) p_threat->set_y_pos(1700+i*3000);
            else if (i<50) p_threat->set_y_pos(61700+(i-20)*1500);
            else p_threat->set_y_pos(106700+(i-50)*500);
            p_threat->set_type_move(ThreatsObj::STATIC_THREAT);
            p_threat->set_input_left(0);


            list_threats.push_back(p_threat);
        }
    }

    return list_threats;
}

int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    if (InitData() == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    bool is_quit = false;
    Uint32 high_scores = 0;
    while (!is_quit)
    {
        is_quit = true;
        bool is_loss = false;

        GameMap game_map;
        game_map.LoadMap("map/map01.dat");
        game_map.LoadTiles(g_screen);

        std::vector<ThreatsObj*> threats_list = MakeThreadList();

        int death_recent = 30, lifes = 3, killed_threat = 0, killed_recent = 0, first_loop = 40;
        Uint32 time_Menu = 0, scores_last  = 0;

        //âm thanh
        Mix_Chunk* chunkDie = Mix_LoadWAV("audio//die.wav");
        Mix_Chunk* chunkExp = Mix_LoadWAV("audio//exp.wav");
        Mix_Chunk* chunkOver = Mix_LoadWAV("audio//over.wav");

        Mix_Music* bgm = Mix_LoadMUS("audio//bgm.mp3");
        Mix_VolumeMusic(50);
        Mix_PlayMusic(bgm, 10);

        TextObj Scores, GameOver, AddScores;

        Scores.SetColor(TextObj::WHITE_TEXT);
        GameOver.SetColor(TextObj::WHITE_TEXT);
        TTF_Font* font_over = TTF_OpenFont("font//karma suture.ttf", 80);
        GameOver.SetText("GAME OVER");
        AddScores.SetColor(TextObj::WHITE_TEXT);



        int ret_Menu = SDLCommonFunc::ShowMenu(g_screen, scores_font, time_Menu);
        int ret_level = 0, ret_character = 0;
        if (ret_Menu == 1) is_loss = true;
        if (ret_Menu == 0)
        {
            ret_level = SDLCommonFunc::ShowLevel(g_screen, scores_font, time_Menu);
            ret_character = SDLCommonFunc::ShowCharacter(g_screen, scores_font, time_Menu);
        }

        MainObj p_player;
        switch(ret_character)
        {
        case 0:
            p_player.LoadImg("img//player_right.png", g_screen);
            break;
        case 1:
            p_player.LoadImg("img//player1_right.png", g_screen);
            break;
        case 2:
            p_player.LoadImg("img//player2_right.png", g_screen);
            break;
        }
        p_player.SetCharacter(ret_character);

        p_player.set_clips();

        switch (ret_level)
        {
        case 0:
            AddScores.SetText("+200");
            break;
        case 1:
            AddScores.SetText("+400");
            break;
        case 2:
            AddScores.SetText("+600");
            break;
        default:
            break;
        }

        while (!is_loss)
        {
            fps_timer.start();
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {
                    is_loss = true;
                }

                p_player.HandelInputAction(g_event, g_screen, death_recent);
            }

            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);

            g_background.Render(g_screen, NULL);
            for (int i=0; i<lifes; i++)
            {
                num_lifes.SetRect(i*55, 0);
                num_lifes.Render(g_screen, NULL);
            }

            Map map_data = game_map.getMap();

            p_player.HandleBullet(g_screen, ret_level);
            p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
            p_player.DoPlayer(map_data, ret_level);
            p_player.Show(g_screen, death_recent);


            game_map.SetMap(map_data);
            game_map.DrawMap(g_screen);

            bool bCol2 = false;
            for (int i = 0; i < threats_list.size(); i++)
            {
                ThreatsObj* p_threat = threats_list.at(i);
                if (p_threat != NULL)
                {
                    p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                    p_threat->ImpMoveType(g_screen);
                    p_threat->DoPlayer(map_data);
                    p_threat->Show(g_screen);

                    // xử lí va chạm player với threats
                    SDL_Rect rect_player = p_player.GetRectFrame(); // gọi ra các thông số về tọa độ (x,y,w,h) của 2 đối tượng
                    SDL_Rect rect_threat = p_threat->GetRectFrame();
                    bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat); // check
                    if (bCol2)
                    {
                        break;
                    }


                }
            }

            if (p_player.IsDead(map_data, bCol2, death_recent))
            {
                lifes--;
                Mix_PlayChannel(-1, chunkDie, 0);
                SDL_Delay(500);
                death_recent = 50;
            }
            if (death_recent != 0) death_recent--;
            p_player.AddLifes(map_data, lifes);
            if (lifes >=5) lifes = 5;

            Uint32 level_factor = 1;
            switch (ret_level)
            {
            case 0:
                level_factor = 1;
                break;
            case 1:
                level_factor = 2;
                break;
            case 2:
                level_factor = 3;
                break;
            default:
                break;
            }

            std::string scores_txt = "Scores: ";
            Uint32 scores_val = 0;
            if (first_loop == 0) scores_val = ((SDL_GetTicks()-time_Menu)/10 + killed_threat*200)*level_factor;
            scores_last = scores_val;
            scores_txt += std::to_string(scores_val);

            Scores.SetText(scores_txt);
            Scores.LoadFromRenderText(scores_font, g_screen);
            Scores.RenderText(g_screen, SCREEN_WIDTH-300, 5);



        std::vector<BulletObj*> bullet_arr = p_player.get_bullet_list();
            for (int r = 0; r < bullet_arr.size(); r++)
            {
                BulletObj* p_bullet = bullet_arr.at(r);
                if (p_bullet != NULL)
                {
                    for (int t = 0; t < threats_list.size(); t++)
                    {
                        ThreatsObj* obj_threat = threats_list.at(t);
                        if (obj_threat != NULL)
                        {

                            SDL_Rect tRect;
                            tRect.x = obj_threat->GetRect().x;
                            tRect.y = obj_threat->GetRect().y;
                            tRect.w = obj_threat->get_width_frame(); // vì sử dụng khung hình để load hiệu ứng động nên chỉ lấy ra cái khung hình width chuẩn
                            tRect.h = obj_threat->get_height_frame();

                            SDL_Rect bRect = p_bullet->GetRect();

                            bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                            if (bCol)
                            {
                                p_player.RemoveBullet(r);   // trước tiên đạn trúng thì phải xóa đạn
                                Mix_PlayChannel(-1, chunkExp, 0);
                                obj_threat->Free();         // xóa threat bị trúng đạn
                                threats_list.erase(threats_list.begin() + t);
                                killed_threat++;
                                killed_recent = 30;
                            }
                        }
                    }
                }
            }
            if (first_loop > 0) first_loop--;
            p_player.SetFirstLoop(first_loop, g_screen);

            if (killed_recent > 0)
            {
                killed_recent--;
                AddScores.LoadFromRenderText(scores_font, g_screen);
                AddScores.RenderText(g_screen, SCREEN_WIDTH-100, 40 );
            }


            if (lifes == 0)
            {
                if (scores_last > high_scores) high_scores = scores_last;
                GameOver.LoadFromRenderText(font_over, g_screen);
                GameOver.RenderText(g_screen, 130, 300);

                Mix_PlayChannel(-1, chunkOver, 0);
                SDL_RenderPresent(g_screen);
                SDL_Delay(2000);
                is_loss = true;
            }

            SDL_RenderPresent(g_screen);


            int real_imp_time = fps_timer.get_ticks();
            int time_one_frame = 1000/FRAME_PER_SECOND; // ms

            if (real_imp_time < time_one_frame)
            {
                int delay_time = time_one_frame - real_imp_time;
                if (delay_time >= 0)
                    SDL_Delay(delay_time);
            }


        };
        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObj* p_threats = threats_list.at(i);
            if (p_threats)
            {
                p_threats->Free();
                p_threats = NULL;
            }
        }
        threats_list.clear();

        bool ret_exit = SDLCommonFunc::ShowExit(g_screen, scores_font, scores_last, high_scores, ret_character);
        if (ret_exit == 0) is_quit = false;
    }
    close();

// tránh thừa bộ nhớ (xóa lớp threat sau khi xong)
    return 0;
}



