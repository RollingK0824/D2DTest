#include "Engine/Resource/ResourceManager.h"
#include "Engine/Graphic/GraphicManager.h"

#include <wincodec.h>
#include <d2d1.h>


#pragma comment(lib, "windowscodecs.lib")

bool ResourceManager::Initialize()
{
	// WIC Imaging Factory 생성
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"WIC Factory 초기화 실패", L"Error", MB_ICONERROR);
		return false;
	}
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pWICFactory)
	);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"WIC Factory 생성 실패", L"Error", MB_ICONERROR);
		return false;
	}
	return true;
}

void ResourceManager::Update()
{
	// TODO : 리소스 관리 로직 추가 (예: 메모리 최적화, 리소스 갱신 등)
}

ID2D1Bitmap* ResourceManager::LoadTexture(const std::wstring& key, const std::wstring& filePath)
{
	// 중복 로드 방지 : 이미 Pool에 존재하는 Key인지 확인
	auto iter = m_texturePool.find(key);
	if (iter != m_texturePool.end())	// 이미 존재하는 Key라면
	{
		return iter->second; // 이미 로드된 텍스처 반환
	}

	if (!m_pWICFactory) return nullptr;

	// RenderTarget GraphicManager에서 가져오기
	ID2D1HwndRenderTarget* pRenderTarget = GraphicManager::GetInstance()->GetRenderTarget();
	if (!pRenderTarget) return nullptr;

	// WIC 디코딩 파이프라인 구성
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	ID2D1Bitmap* pBitmap = nullptr;

	// 하드디스크에서 파일 열람
	HRESULT hr = m_pWICFactory->CreateDecoderFromFilename(
		filePath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (FAILED(hr))return nullptr;

	// 이미지의 첫 번째 프레임 데이터 추출
	hr = pDecoder->GetFrame(0, &pSource);
	if (FAILED(hr)) { pDecoder->Release(); return nullptr; }

	// RGBA32 포맷으로 변환
	hr = m_pWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) { pSource->Release(); pDecoder->Release(); return nullptr; }

	hr = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,	// 알파 채널(투명도) 포함 RGBA 포맷
		WICBitmapDitherTypeNone,
		nullptr,
		0.0,
		WICBitmapPaletteTypeCustom
	);

	if(SUCCEEDED(hr))
	{
		// Direct2D Bitmap으로 변환하여 GPU에 업로드
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			&pBitmap
		);
	}

	if (pConverter) pConverter->Release();
	if (pSource) pSource->Release();
	if (pDecoder) pDecoder->Release();

	if (SUCCEEDED(hr) && pBitmap)
	{
		// 텍스처 풀에 등록
		m_texturePool[key] = pBitmap;
		return pBitmap;
	}

	return nullptr;
}

ID2D1Bitmap* ResourceManager::GetTexture(const std::wstring& key) const
{
	auto iter = m_texturePool.find(key);
	if (iter != m_texturePool.end())
	{
		return iter->second;
	}
	return nullptr;
}

void ResourceManager::Release()
{
	// 텍스처 풀에 등록된 모든 비트맵 리소스 해제
	for (auto& pair : m_texturePool)
	{
		if (pair.second)
		{
			pair.second->Release();
		}
	}

	m_texturePool.clear();

	// WIC Factory 해제
	if (m_pWICFactory)
	{
		m_pWICFactory->Release();
		m_pWICFactory = nullptr;
	}

	CoUninitialize();
}