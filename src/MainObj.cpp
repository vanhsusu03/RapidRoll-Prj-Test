
#include "MainObj.h"
#include "CommonFunction.h"


MainObj::MainObj()
{
    frame_ = 0;                // xử lí animations của player
    x_pos_ = 320;                // xử lí vị trí của player
    y_pos_ = 400;                //
    x_val_ = 0;                // xử lí lượng di chuyển của player
    y_val_ = 0;                //
    width_frame_ = 0;          // chiều rộng player
    height_frame_ = 0;         // chiều cao player
    status_ = WALK_NONE;    // trạng thái của player
    input_type_.left_ = 0;        //
    input_type_.right_ = 0;       //
    input_type_.jump_ = 0;        // xử lí chuyển động nhân vật
    input_type_.down_ = 0;        //
    input_type_.up_ = 0;          //
    on_ground_ = false;      // xử lí nhân vật đứng trên đất
    map_x_ = 0;              // xử lí map
    map_y_ = 0;
    come_back_time = 0;      // xử lí thời gian hồi sinh
    delay_shot = 0;
    _ret_character = 0;
    _first_loop = 300;
    chunkGun = Mix_LoadWAV("audio//gun.wav");
    chunkGet = Mix_LoadWAV("audio//get.wav");
}

MainObj::~MainObj()
{
    // Free
}

bool MainObj::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w/8;
        height_frame_ = rect_.h;
    }

    return ret;
}

SDL_Rect MainObj::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

// hàm xử lí animations của nhân vật
void MainObj::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i=0; i<8; i++)
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void MainObj::Show(SDL_Renderer* des, int death_recent)
{
    UpdateImagePlayer(des, death_recent);

    if (input_type_.left_ == 1 ||
        input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }

    if (frame_ >= 8)
    {
        frame_ = 0;
    }

    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

// hàm xử lí các thao tác từ bản phím
void MainObj::HandelInputAction(SDL_Event events, SDL_Renderer* screen, int death_recent)
{
    if (delay_shot > 0) delay_shot--;
    if (_first_loop == 0)
    {
        if (events.type == SDL_KEYDOWN) // KEYDOWN là ấn bất kì nút nào từ bàn phím
        {
            switch (events.key.keysym.sym)
            {
            case SDLK_d:        // nút mũi tên sang phải
                {
                    status_= WALK_RIGHT;
                    input_type_.right_ = 1;
                    input_type_.left_ = 0;
                    UpdateImagePlayer(screen, death_recent);
                }
                break;
            case SDLK_a:         // nút mũi tên sang trái
                {
                    status_ = WALK_LEFT;
                    input_type_.left_ = 1;
                    input_type_.right_ = 0;
                    UpdateImagePlayer(screen, death_recent);
                }
                break;
            }
        }
        else if (events.type == SDL_KEYUP)  // KEYUP là nhả phím
        {
            switch (events.key.keysym.sym)
            {
            case SDLK_d:                    // nhả d ra không chạy sang phải
                {
                    input_type_.right_ = 0;
                }
                break;
            case SDLK_a:                    // nhả a ra không chạy sang trái
                {
                    input_type_.left_ = 0;
                }
                break;
            }
        }

        if (events.type == SDL_KEYDOWN)
        {
            if (events.key.keysym.sym == SDLK_j && delay_shot == 0)  // ấn key j để bắn
            {
                Mix_PlayChannel(-1, chunkGun, 0);
                BulletObj* p_bullet = new BulletObj();
                switch (_ret_character)
                {
                case 0:
                    {
                        if (status_ == 1) p_bullet->LoadImg("img//player_bullet.png", screen);    // load vào hình ảnh đạn bắn
                        else p_bullet->LoadImg("img//player_bullet_left.png", screen);
                    }
                    break;
                case 1:
                    {
                        if (status_ == 1) p_bullet->LoadImg("img//player1_bullet.png", screen);    // load vào hình ảnh đạn bắn
                        else p_bullet->LoadImg("img//player1_bullet_left.png", screen);
                    }
                    break;
                case 2:
                    {
                        if (status_ == 1) p_bullet->LoadImg("img//player2_bullet.png", screen);    // load vào hình ảnh đạn bắn
                        else p_bullet->LoadImg("img//player2_bullet.png", screen);
                    }
                    break;
                default:
                    break;
                }


                if (status_ == WALK_LEFT)
                {
                    p_bullet->set_bullet_dir(BulletObj::DIR_LEFT);
                    p_bullet->SetRect(this->rect_.x, rect_.y + 10);   // set vị trí đạn bắn ra
                }
                else
                {
                    p_bullet->set_bullet_dir(BulletObj::DIR_RIGHT);
                    p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + 10 );   // set vị trí đạn bắn ra
                }

                p_bullet->set_x_val(20);       // tốc độ bullet
                p_bullet->set_is_move(true);   // bấm phím là bắn

                p_bullet_list_.push_back(p_bullet);  // thêm bullet vào danh sách
                delay_shot = DELAY_SHOT;
            }
        }
    }
}

// hàm xử lí bắn đạn ra
void MainObj::HandleBullet(SDL_Renderer* des, int ret_level)
{
    for (int i = 0; i < p_bullet_list_.size(); i++)
    {
        BulletObj* p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)   // ktra trạng thái để được phép bắn
            {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT, ret_level);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);  // loại đạn khỏi danh sách
                if (p_bullet != NULL)   // ktra xem đã loại chưa rồi mới xóa
                {
                    delete p_bullet;    // xóa đạn tránh tốn bộ nhớ
                    p_bullet = NULL;
                }
            }
        }
    }
}

// hàm xóa viên đạn
void MainObj::RemoveBullet(const int& idx)
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObj* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

// xử lí toàn bộ các events player
void MainObj::DoPlayer(Map& map_data, int ret_level)
{
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= MAX_FAIL_SPEED)
        {
            y_val_ = MAX_FAIL_SPEED;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= PLAYER_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            x_val_ += PLAYER_SPEED;
        }
        CheckToMap(map_data);
        MoveMap(map_data, ret_level);

}

// hàm xử lí map di chuyển theo nhân vật
void MainObj::MoveMap(Map& map_data, int ret_level)
{
    int speed_move = 0;
    switch (ret_level)
    {
    case 0:
        speed_move = 7;
        break;
    case 1:
        speed_move = 10;
        break;
    case 2:
        speed_move = 13;
        break;
    }
    if (_first_loop == 0) map_data.start_y_ += speed_move;
}

void MainObj::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    // Kiểm tra theo chiều ngang
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // mainobj đang di chuyển sang phải
        {

            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )    // khi player chạm vào các ô vật phẩm
            {
                map_data.tile[y1][x2] = 0;         // xóa ô vật phẩm mà nhân vật chạm vào
                map_data.tile[y2][x2] = 0;

            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE) // player gặp phải những ô không phải ô trống
                {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;                     // không cho nhân vật đi thêm nữa
                }
            }
        }
        else if (x_val_ < 0)                     // tương tự với chiều âm
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;

            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    x_pos_ = (x1 + 1)*TILE_SIZE;
                    x_val_ = 0;
                }
            }
        }
    }


    // ktra theo chiều dọc

    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
    {
        if (y_val_ > 0)                   // tương tự với chiều dọc (rơi xuống chạm vào vật phẩm)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;

            }
            else
            {
                if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
                {
                    y_pos_ = y2*TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    y_val_ = 0;
                    if (status_ == WALK_NONE)
                    {
                        status_ = WALK_RIGHT;
                    }

                }
            }
        }
        else if (y_val_ < 0)            // tương tự với nhảy lên chạm vào vật phẩm
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;

            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    y_pos_ = (y1 + 1)*TILE_SIZE;
                    y_val_ = 0;
                }
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

}



void MainObj::UpdateImagePlayer(SDL_Renderer* des, int death_recent)
{
    if (death_recent != 0)
    {
        switch(_ret_character)
        {
        case 0:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player_protect_left.png", des);
                }
                else
                {
                    LoadImg("img//player_protect_right.png", des);
                }
            }
            break;
        case 1:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player1_protect_left.png", des);
                }
                else
                {
                    LoadImg("img//player1_protect_right.png", des);
                }
            }
            break;
        case 2:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player2_protect_left.png", des);
                }
                else
                {
                    LoadImg("img//player2_protect_right.png", des);
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch(_ret_character)
        {
        case 0:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player_left.png", des);
                }
                else
                {
                    LoadImg("img//player_right.png", des);
                }
            }
            break;
        case 1:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player1_left.png", des);
                }
                else
                {
                    LoadImg("img//player1_right.png", des);
                }
            }
            break;
        case 2:
            {
                if (status_ == WALK_LEFT)
                {
                    LoadImg("img//player2_left.png", des);
                }
                else
                {
                    LoadImg("img//player2_right.png", des);
                }
            }
            break;
        default:
            break;
        }
    }

}

void MainObj::SetCharacter(int ret_character)
{
    _ret_character = ret_character;
}

void MainObj::SetFirstLoop(int first_loop, SDL_Renderer* des)
{
    _first_loop = first_loop;
    TextObj FirstLoopTxt;
    FirstLoopTxt.SetColor(TextObj::WHITE_TEXT);
    TTF_Font* first_loop_font = TTF_OpenFont("font//karma suture.ttf", 100);
    if (first_loop > 30)
    {
        FirstLoopTxt.SetText("3");
    }
    else if (first_loop > 20)
    {
        FirstLoopTxt.SetText("2");
    }
    else if (first_loop > 10)
    {
        FirstLoopTxt.SetText("1");
    }
    FirstLoopTxt.LoadFromRenderText(first_loop_font, des);
    FirstLoopTxt.RenderText(des, 300, 380);
    if (first_loop > 0 && first_loop <= 10)
    {
        FirstLoopTxt.SetText("GO");
        FirstLoopTxt.LoadFromRenderText(first_loop_font, des);
        FirstLoopTxt.RenderText(des, 270, 380);
    }
}


bool MainObj::IsDead(Map& map_data, bool bCol2, int death_recent)
{
    if (death_recent == 0)
    {
        if (bCol2)
        {
            x_pos_ = map_data.start_x_ + 300;
            y_pos_ = map_data.start_y_ + 350;
            return true;
        }
        if (y_pos_ > map_data.start_y_+SCREEN_HEIGHT-80 || y_pos_ < map_data.start_y_+128)
        {
            x_pos_ = map_data.start_x_ + 300;
            y_pos_ = map_data.start_y_ + 350;
            return true;
        }
        int x1 = 0;
        int x2 = 0;

        int y1 = 0;
        int y2 = 0;

        // Kiểm tra theo chiều ngang
        int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

        x1 = (x_pos_ + x_val_)/TILE_SIZE;
        x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

        y1 = (y_pos_)/TILE_SIZE;
        y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
        {
            if (x_val_ > 0) // mainobj đang di chuyển sang phải
            {

                int val1 = map_data.tile[y1][x2];
                int val2 = map_data.tile[y2][x2];

                if (val1 == FIRE_ITEM || val2 == FIRE_ITEM  )    // khi player chạm vào các ô vật phẩm
                {
                    x_pos_ = map_data.start_x_ + 300;
                    y_pos_ = map_data.start_y_ + 350;
                    return true;
                }
            }
            else if (x_val_ < 0)                     // tương tự với chiều âm
            {
                int val1 = map_data.tile[y1][x1];
                int val2 = map_data.tile[y2][x1];

                if (val1 == FIRE_ITEM || val2 == FIRE_ITEM  )
                {
                    x_pos_ = map_data.start_x_ + 300;
                    y_pos_ = map_data.start_y_ + 350;
                    return true;
                }
            }
        }


        // ktra theo chiều dọc

        int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
        x1 = (x_pos_)/TILE_SIZE;
        x2 = (x_pos_ + width_min)/TILE_SIZE;

        y1 = (y_pos_ + y_val_)/TILE_SIZE;
        y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
        {
            if (y_val_ > 0)                   // tương tự với chiều dọc (rơi xuống chạm vào vật phẩm)
            {
                int val1 = map_data.tile[y2][x1];
                int val2 = map_data.tile[y2][x2];

                if (val1 == FIRE_ITEM || val2 == FIRE_ITEM  )
                {
                    x_pos_ = map_data.start_x_ + 300;
                    y_pos_ = map_data.start_y_ + 350;
                   return true;
                }
            }
            else if (y_val_ < 0)            // tương tự với nhảy lên chạm vào vật phẩm
            {
                int val1 = map_data.tile[y1][x1];
                int val2 = map_data.tile[y1][x2];
                if (val1 == FIRE_ITEM || val2 == FIRE_ITEM  )
                {
                    x_pos_ = map_data.start_x_ + 300;
                    y_pos_ = map_data.start_y_ + 350;
                    return true;
                }
            }
        }
    }

    return false;
}

void MainObj::AddLifes(Map& map_data, int& lifes)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    // Kiểm tra theo chiều ngang
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // mainobj đang di chuyển sang phải
        {

            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )    // khi player chạm vào các ô vật phẩm
            {
                map_data.tile[y1][x2] = 0;         // xóa ô vật phẩm mà nhân vật chạm vào
                map_data.tile[y2][x2] = 0;
                lifes++;
                Mix_PlayChannel(-1, chunkGet, 0);
            }
        }
        else if (x_val_ < 0)                     // tương tự với chiều âm
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                lifes++;
                Mix_PlayChannel(-1, chunkGet, 0);
            }
        }
    }


    // ktra theo chiều dọc

    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
    {
        if (y_val_ > 0)                   // tương tự với chiều dọc (rơi xuống chạm vào vật phẩm)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                lifes++;
                Mix_PlayChannel(-1, chunkGet, 0);
            }
        }
        else if (y_val_ < 0)            // tương tự với nhảy lên chạm vào vật phẩm
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if (val1 == STATE_ITEM1 || val2 == STATE_ITEM1  )
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                lifes++;
                Mix_PlayChannel(-1, chunkGet, 0);
            }
        }
    }
}

