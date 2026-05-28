#pragma once

#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
#include "Engine/Core/IUpdatable.h"
#include <windows.h>

class TimeManager : public Singleton<TimeManager>, public ISystem, public IUpdatable
{
	friend class Singleton<TimeManager>;

public:
	// ISystem 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Release() override;

	// IUpdatable 인터페이스 구현
	virtual void Update(float dt) override;
	
	bool AccumulateTime();
	void ConsumeFixedTick();

	// Getter
	float GetDeltaTime() const { return m_deltaTime; }
	float GetFixedDeltaTime() const { return m_fixedDeltaTime; }
	unsigned int GetFPS() const { return m_fps; }

private:
	TimeManager() = default;
	virtual ~TimeManager() = default;

private:
	LARGE_INTEGER m_frequency = {};
	LARGE_INTEGER m_prevCpuTick = {};
	LARGE_INTEGER m_currentCpuTick = {};

	// 시간 계산 변수
	float m_deltaTime = 0.0f;
	float m_timeScale = 1.0f;

	float m_fixedDeltaTime = 0.02f; // 고정 시간
	float m_accumulator = 0.0f;

	// FPS 계산용 변수
	unsigned int m_fps = 0;
	unsigned int m_frameCount = 0;
	float m_fpsFrameTime = 0.0f;

};

