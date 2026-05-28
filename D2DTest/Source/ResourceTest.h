#pragma once


#include "Engine/Core/Define.h"
#include <string>


#if ENABLE_RESOURCE_TEST
// 테스트용 오브젝트 데이터 구조 (RenderCommand와 완벽히 호환되도록 매칭)
struct TestPlayer
{
    std::wstring textureKey;
    Pos2D         position;  // 구조체에 정의된 Pos2D 타입 사용
    float         rotation;
    float         scaleX;
    float         scaleY;
    int           zOrder;
    float         opacity;
    bool          flipX;
    bool          flipY;
};

void Test_Initialize();
void Test_Update(float dt);
void Test_Render();

#endif  // ENABLE_RESOURCE_TEST