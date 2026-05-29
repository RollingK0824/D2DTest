#pragma once
#include <d2d1.h>
#include <string>

struct Sprite
{
    std::wstring textureKey; // 리소스 매니저 쿼리용 이름
    D2D1_RECT_F  srcRect;    // 아틀라스 내부 사각형 영역
    float        width;      // 원본 가로 크기
    float        height;     // 원본 세로 크기
};