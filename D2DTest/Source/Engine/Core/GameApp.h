#pragma once

#include "Engine/Core/Singleton.h"
#include <windows.h>

#define ENABLE_RESOURCE_TEST 1

struct ID2D1Bitmap;

class GraphicManager;

class GameApp : public Singleton<GameApp>
{
	friend class Singleton<GameApp>;

public:
	// 윈도우 생성 및 초기화
	bool Initialize(HINSTANCE hInstance, int nCmdShow, int width, int height);

	// 게임 메인 루프
	int Run();

	// 엔진 종료 및 메모리 해제
	void Release();

	HWND GetWindowHandle() const { return m_hWnd; }

private:
	GameApp() = default;
	virtual ~GameApp() = default;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND m_hWnd = nullptr;
	HINSTANCE m_hInstance = nullptr;
	bool m_bIsRunning = true;

private:
	// ResourceManagerTEST용 함수 및 변수들
#if ENABLE_RESOURCE_TEST
	void Test_Initialize();
	void Test_Render();

	ID2D1Bitmap* m_pTestTexture = nullptr;
#endif	// ENABLE_RESOURCE_TEST
};