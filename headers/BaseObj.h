

#ifndef BASEOBJ_H
#define BASEOBJ_H


#include "CommonFunction.h"

class BaseObj
{
public:
    BaseObj();
    ~BaseObj();
    void SetRect(const int &x, const int &y) {rect_.x = x, rect_.y = y;}
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture* GetObject() const {return p_object_;}

    virtual bool LoadImg(std::string path, SDL_Renderer* screen); // load ảnh lên
    void Render(SDL_Renderer* des, const SDL_Rect*clip = NULL); // in ra màn hình
    void Free();

protected:
    SDL_Texture* p_object_; // biến lưu trữ hình ảnh
    SDL_Rect  rect_;        // biến lưu kích thước
};
#endif // BASEOBJ_H
