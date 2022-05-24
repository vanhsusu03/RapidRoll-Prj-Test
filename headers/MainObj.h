
#ifndef MAINOBJ_H
#define MAINOBJ_H

#include <vector>                 // tạo 1 danh sách các viên đạn liên tục

#include "CommonFunction.h"
#include "BaseObj.h"
#include "BulletObj.h"
#include "TextObj.h"

#define GRAVITY_SPEED 15         // tốc độ rơi
#define MAX_FAIL_SPEED 25
#define PLAYER_SPEED 10            // tốc độ chạy
#define PLAYER_BULLET 0.4
#define DELAY_SHOT 15

class MainObj : public BaseObj
{
public:
    MainObj();
    ~MainObj();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des, int death_recent);
    void HandelInputAction(SDL_Event events, SDL_Renderer* screen, int death_recent);
    void set_clips();

    void DoPlayer(Map& map_data, int ret_level); // hàm xử lí tốc độ rơi
    void CheckToMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}  // cung cấp vị trí của map
    void MoveMap(Map& map_data, int ret_level);   // tính toán ra thông số của map khi nhân vật di chuyển
    void UpdateImagePlayer(SDL_Renderer* des, int death_recent);  // update lại png của nhân vật và các animations
    SDL_Rect GetRectFrame();
    bool IsDead(Map& map_data, bool bCol2, int death_recent);
    void AddLifes (Map& map_data, int& lifes);

    void set_bullet_list(std::vector<BulletObj*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }

    void SetCharacter(int ret_character);
    void SetFirstLoop (int first_loop, SDL_Renderer* des);

    std::vector<BulletObj*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des, int ret_level);
    void RemoveBullet(const int& idx);  // hàm xóa bullet khi va chạm

private:

    std::vector<BulletObj*> p_bullet_list_;
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    int come_back_time;

    int delay_shot;
    int _ret_character;
    int _first_loop;

    Mix_Chunk* chunkGun;
    Mix_Chunk* chunkGet;
};

#endif // MAINOBJ_H
