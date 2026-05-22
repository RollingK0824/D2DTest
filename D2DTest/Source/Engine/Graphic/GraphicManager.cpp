#include "Engine/Graphic/GraphicManager.h"
#include "Engine/Core/GameApp.h"

bool GraphicManager::Initialize()
{
	// 1. Factory 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"Direct2D Factory 생성 실패", L"Error", MB_ICONERROR);
		return false;
	}

	// 2. 엔진 커널에서 윈도우 핸들 가져오기
	HWND hWnd = GameApp::GetInstance()->GetWindowHandle();
	if(!hWnd)
	{
		MessageBoxW(nullptr, L"윈도우 핸들 가져오기 실패", L"Error", MB_ICONERROR);
		return false;
	}

	// 3. 현재 윈도우의 클라이언트 영역 크기 가져오기
	RECT rc;
	GetClientRect(hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// 4. Render Target 생성
	hr = m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size),
		&m_pRenderTarget
	);
	if(FAILED(hr))
	{
		MessageBoxW(nullptr, L"Render Target 생성 실패", L"Error", MB_ICONERROR);
		return false;
	}

	return true;
}

void GraphicManager::Update(float dt)
{
	// TODO : 렌더링 로직 추가
}

void GraphicManager::BeginDraw()
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->BeginDraw();
	}
}

void GraphicManager::EndDraw()
{
	if(m_pRenderTarget)
	{
		// 모든 그리기 명령을 GPU로 전송하고 화면 전환
		HRESULT hr = m_pRenderTarget->EndDraw();
		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"렌더링 실패", L"Error", MB_ICONERROR);
		}

		if(hr==D2DERR_RECREATE_TARGET)
		{
			// Render Target이 유효하지 않으므로 다시 생성
			m_pRenderTarget->Release();
			m_pRenderTarget = nullptr;
			Initialize(); // Render Target 재생성
		}
	}
}

void GraphicManager::Clear(const D2D1_COLOR_F& color)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Clear(color);
	}
}

void GraphicManager::Release()
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Release();
		m_pRenderTarget = nullptr;
	}
	if (m_pFactory)
	{
		m_pFactory->Release();
		m_pFactory = nullptr;
	}
}
