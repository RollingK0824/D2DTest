#pragma once

#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
#include <string>
#include <unordered_map>

struct ID2D1Bitmap;
struct IWICImagingFactory;

class ResourceManager : public Singleton<ResourceManager>, public ISystem
{
	friend class Singleton<ResourceManager>;

public:
	// IManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Release() override;

	// 이미지를 읽어와 GPU 비트맵으로 등록
	ID2D1Bitmap* LoadTexture(const std::wstring& ket, const std::wstring& filePath);

	// 등록된 비트맵을 꺼내 사용
	ID2D1Bitmap* GetTexture(const std::wstring& key) const;

private:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	IWICImagingFactory* m_pWICFactory = nullptr;

	// 이미지들일 이름(key)으로 관리하는 텍스처 풀
	std::unordered_map<std::wstring, ID2D1Bitmap*> m_texturePool;

};

