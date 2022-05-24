

#ifndef EXPLOSIONOBJ_H
#define EXPLOSIONOBJ_H

#include <BaseObj.h>
#include <CommonFunction.h>

#define NUM_FRAME_EXP 8

const int EXP_WIDTH = 165;
const int EXP_HEIGHT = 165;

class ExplosionObj : public BaseObj
{
public:
    ExplosionObj();
    ~ExplosionObj();

public:
    void set_clip();
    void set_frame(const int& fr) {frame_ = fr;}
    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}

private:
    int width_frame_;
    int height_frame_;

    unsigned int frame_;
    SDL_Rect frame_clip_[8];
};

#endif // EXPLOSIONOBJ_H
