

#ifndef BULLETOBJ_H
#define BULLETOBJ_H

#include "BaseObj.h"
#include "CommonFunction.h"

class BulletObj : public BaseObj
{
public:
    BulletObj();
    ~BulletObj();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    void set_x_val(const int& xVal) {x_val_ = xVal;}
    void set_y_val(const int& yVal) {y_val_ = yVal;}
    int get_x_val() const {return x_val_;}
    int get_y_val() const {return y_val_;}

    void set_is_move(const bool& isMove) {is_move_ = isMove;}
    bool get_is_move() const {return is_move_;}

    void set_bullet_dir(const unsigned int& bulletDir) {bullet_dir_ = bulletDir;} // hàm xử lí hướng của đạn
    int get_bullet_dir() const {return bullet_dir_;}

    void HandleMove(const int& x_border, const int& y_border, int ret_level); // truyền vào hàm xử lí di chuyển bullet các giới hạn screenwidth
private:
    int x_val_;   // biến tọa độ của đạn
    int y_val_;   //
    bool is_move_;  // biến trạng thái trong hay ngoài màn hình
    unsigned int bullet_dir_;
};


#endif // BULLETOBJ_H
