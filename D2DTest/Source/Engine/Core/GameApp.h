#pragma once

#include "Engine/Core/Define.h"
#include "Engine/Core/Singleton.h"
#include <windows.h>

struct ID2D1Bitmap;

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
	HINSTANCE GetInstanceHandle() const { return m_hInstance; }

private:
	GameApp() = default;
	virtual ~GameApp() = default;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND m_hWnd = nullptr;
	HINSTANCE m_hInstance = nullptr;
	bool m_bIsRunning = true;
};