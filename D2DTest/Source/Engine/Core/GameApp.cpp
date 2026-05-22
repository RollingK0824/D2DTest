#include "Engine/Core/GameApp.h"

#include "Engine/Graphic/GraphicManager.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Core/TimeManager.h"

bool GameApp::Initialize(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	m_hInstance = hInstance;

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

	// 4. 윈도우 창이 정상 개설된 직후 매니저 가동
	if (!GraphicManager::GetInstance()->Initialize())
	{
		MessageBoxW(nullptr, L"그래픽 매니저 초기화 실패", L"Error", MB_ICONERROR);
		return false;
	}

	if (!TimeManager::GetInstance()->Initialize())
	{
		MessageBoxW(nullptr, L"타임 매니저 초기화 실패", L"ERROR", MB_ICONERROR);
		return false;
	}
	

	if (!ResourceManager::GetInstance()->Initialize())
	{
		MessageBoxW(nullptr, L"리소스 매니저 초기화 실패", L"Error", MB_ICONERROR);
		return false;
	}

#if ENABLE_RESOURCE_TEST
	Test_Initialize();
#endif // ENABLE_RESOURCE_TEST

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
			
			// 매 루프의 시작점에 DeltaTime 계산
			TimeManager* pTime = TimeManager::GetInstance();
			pTime->Update(0.0f);

			float dt = pTime->GetDeltaTime();
			float fixedDt = pTime->GetFixedDeltaTime();

			while (pTime->AccumulateTime())
			{
				pTime->ConsumeFixedTick();
			}

			GraphicManager::GetInstance()->Update(dt);
			GraphicManager::GetInstance()->BeginDraw();

			GraphicManager::GetInstance()->Clear(D2D1::ColorF(D2D1::ColorF::White));

#if ENABLE_RESOURCE_TEST
			Test_Render();
#endif // ENABLE_RESOURCE_TEST

			GraphicManager::GetInstance()->EndDraw();
		}
	}

	return (int)msg.wParam;
}

void GameApp::Release()
{
	// TODO: 각종 해제 코드 추가
	GraphicManager::GetInstance()->Release();
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

#if ENABLE_RESOURCE_TEST
void GameApp::Test_Initialize()
{
	// ResourceManager를 통해 텍스처 로드
	m_pTestTexture = ResourceManager::GetInstance()->LoadTexture(L"TestTexture",
		L"Resources/character_antonio.png");

	if (!m_pTestTexture)
	{
		MessageBoxW(nullptr, L"테스트 텍스처 로드 실패", L"Error", MB_ICONWARNING);
	}
}

void GameApp::Test_Render()
{
	if (m_pTestTexture)
	{
		D2D1_SIZE_F imgSize = m_pTestTexture->GetSize();

		D2D1_RECT_F destRect = D2D1::RectF(GraphicManager::GetInstance()->GetRenderTarget()->GetSize().width / 2 - imgSize.width / 2,
			GraphicManager::GetInstance()->GetRenderTarget()->GetSize().height / 2 - imgSize.height / 2,
			imgSize.width + GraphicManager::GetInstance()->GetRenderTarget()->GetSize().width / 2,
			imgSize.height + GraphicManager::GetInstance()->GetRenderTarget()->GetSize().height / 2);
		GraphicManager::GetInstance()->GetRenderTarget()->DrawBitmap(
			m_pTestTexture,
			destRect
		);

		ID2D1SolidColorBrush* pDebugBrush = nullptr;
		HRESULT hr = GraphicManager::GetInstance()->GetRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red, 1.0f),
			&pDebugBrush
		);

		if (SUCCEEDED(hr) && pDebugBrush)
		{
			GraphicManager::GetInstance()->GetRenderTarget()->DrawRectangle(destRect, pDebugBrush,2.0f);
			GraphicManager::GetInstance()->GetRenderTarget()->DrawLine(
				D2D1::Point2F(destRect.left, destRect.top),
				D2D1::Point2F(destRect.right, destRect.bottom),
				pDebugBrush, 1.0f
			);

			GraphicManager::GetInstance()->GetRenderTarget()->DrawLine(
				D2D1::Point2F(destRect.right, destRect.top),
				D2D1::Point2F(destRect.left, destRect.bottom),
				pDebugBrush, 1.0f
			);

			pDebugBrush->Release();
		}
	}
}
#endif // ENABLE_RESOURCE_TEST


int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// 1. 엔진 커널 초기화 (800*600 해상도)
	if (!GameApp::GetInstance()->Initialize(hInstance, nCmdShow, 800, 600))
	{
		return 0;
	}

	// 2. 게임 메인 루프 실행
	int result = GameApp::GetInstance()->Run();

	// 3. 엔진 종료 및 메모리 해제
	GameApp::GetInstance()->Release();

	return result;
}
