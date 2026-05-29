#include "Engine/Core/GameApp.h"
#include "Engine/Core/EngineKernel.h"
#include "Engine/Graphic/GraphicManager.h"
#include "Engine/Render/RenderSystem.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Core/TimeManager.h"
#include "Engine/Core/InputManager.h"
#include "Engine/Core/RandomManager.h"
#include "Manager/GameManager.h"

#if ENABLE_RESOURCE_TEST
#include "ResourceTest.h"
#endif // ENABLE_RESOURCE_TEST

bool GameApp::Initialize(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	m_hInstance = hInstance;

	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"OS COM 라이브러리 초기화 실패", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	// 1. 윈도우 클래스 등록
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"D2D_GameEngine";

	if (!RegisterClassExW(&wc))
	{
		MessageBoxW(nullptr, L"윈도우 클래스 등록 실패", L"Error", MB_ICONERROR);
		return false;
	}

	// 2. 화면 중앙 윈도우 크기 계산
	RECT rc = { 0,0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 3. 윈도우 생성
	m_hWnd = CreateWindowW(L"D2D_GameEngine", L"D2D Game Engine",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd) return false;

	EngineKernel* kernel = EngineKernel::GetInstance();

	kernel->RegisterManager(TimeManager::GetInstance());
	kernel->RegisterManager(GraphicManager::GetInstance());
	kernel->RegisterManager(RenderSystem::GetInstance());
	kernel->RegisterManager(ResourceManager::GetInstance());
	kernel->RegisterManager(InputManager::GetInstance());
	kernel->RegisterManager(RandomManager::GetInstance());
	kernel->RegisterManager(GameManager::GetInstance());

	if (!EngineKernel::GetInstance()->Initialize())
	{
		MessageBoxW(nullptr, L"EngineKernel 초기화 실패", L"Error", MB_ICONERROR);
		return false;
	}
#if ENABLE_RESOURCE_TEST
	Test_Initialize();
#endif // ENABLE_RESOURE_TEST

	m_bIsRunning = true;

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

int GameApp::Run()
{
	MSG msg = {};

	while (m_bIsRunning)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_bIsRunning = false;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			// TODO: 게임 업데이트 및 렌더링 로직 추가
			EngineKernel::GetInstance()->ProcessFrame();
		}
	}

	return (int)msg.wParam;
}

void GameApp::Release()
{
	// TODO: 각종 해제 코드 추가
	EngineKernel::GetInstance()->Release();

	CoUninitialize();
}

LRESULT CALLBACK GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// 1. 엔진 커널 초기화
	if (!GameApp::GetInstance()->Initialize(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return 0;
	}

	// 2. 게임 메인 루프 실행
	int result = GameApp::GetInstance()->Run();

	// 3. 엔진 종료 및 메모리 해제
	GameApp::GetInstance()->Release();

	return result;
}
