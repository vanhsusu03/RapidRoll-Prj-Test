#include "BulletObj.h"

BulletObj::BulletObj() // hàm khởi tạo các thông số ban đầu cho bullet
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
}

BulletObj::~BulletObj()
{
    //dtor
}

void BulletObj::HandleMove(const int& x_border, const int& y_border, int ret_level)
{
    int speed_move_map = 0;
    switch(ret_level)
    {
    case 0:
        speed_move_map = 7;
        break;
    case 1:
        speed_move_map = 10;
        break;
    case 2:
        speed_move_map = 13;
        break;
    }
    if (bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += x_val_;
        rect_.y -= speed_move_map;
        if (rect_.x > x_border)    // xử lí đạn bắn về phía phải
        {
            is_move_ = false;
        }

    }
    else if (bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= x_val_;
        rect_.y -= speed_move_map;
        if (rect_.x < 0)           // xử lí đạn bắn về phía trái
        {
            is_move_ = false;
        }

    }
}
