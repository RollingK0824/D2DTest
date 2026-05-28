#pragma once
#include <d2d1.h>
#include "Engine/Core/Define.h"

struct RenderCommand
{
	ID2D1Bitmap* pTexture = nullptr;	// 그릴 이미지 포인터
	Pos2D position;					// 월드 좌표계상의 위치
	float rotation = 0.0f;				// 회전 값
	float scaleX = 1.0f;				// X축 크기 배율
	float scaleY = 1.0f;				// Y축 크기 배율

	int zOrder = 0;						// Sorting Order (낮을수록 먼저 높을수록 나중
	float opacity = 1.0f;				// 투명도 등 추가 렌더링 옵션

	// 상하 좌우 반전 플래그
	bool flipX = false;					 
	bool flipY = false;
};