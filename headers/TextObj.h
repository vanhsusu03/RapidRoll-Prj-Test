#ifndef TEXTOBJ_H
#define TEXTOBJ_H

#include <CommonFunction.h>

// một hàm hiển thị bình thường không kế thừa từ hàm nào trước
class TextObj
{
public:
    TextObj();
    ~TextObj();

    enum
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

//    bool LoadFromFile(std::string path);    không dùng đến vì không load gì từ file
    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen); // kiểu chữ
    void Free(); // giải phóng

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* screen,
                    int xp, int yp,        // tọa độ hiển thị
                    SDL_Rect* clip = NULL, // phục vụ cho ảnh động
                    double angle = 0.0,    // góc nghiêng
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE); // gương chiếu

    int GetWidth() const {return width_;}
    int GetHeight() const {return height_;}
    SDL_Rect GetRect() const {return rect_;}

    void SetText(const std::string& text) {str_val_ = text; }
    std::string GetText() const {return str_val_;}
    SDL_Rect rect_;


private:
    std::string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    int width_;
    int height_;
};

#endif // TEXTOBJ_H
