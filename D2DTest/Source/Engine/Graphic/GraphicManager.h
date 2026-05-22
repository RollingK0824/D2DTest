#pragma once

#include "Engine/Core/Component.h"
#include "Engine/Core/Singleton.h"	

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

class GraphicManager : public Singleton<GraphicManager>, public Component
{
	friend class Singleton<GraphicManager>;

public:
	// IManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void FixedUpdate(float fixedDt) override {}
	virtual void Update(float dt) override;
	virtual void Release() override;
	ID2D1Factory* GetFactory() const { return m_pFactory; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

	void BeginDraw();
	void EndDraw();

	void Clear(const D2D1_COLOR_F& color);

private:
	GraphicManager() = default;
	virtual ~GraphicManager() = default;

	ID2D1Factory* m_pFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
};