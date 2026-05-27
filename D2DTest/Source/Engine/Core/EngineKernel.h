#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/IManager.h"
#include <vector>

struct EngineTime
{
	float deltaTime = 0.0f;
	float fixedDeltaTime = 0.0f;
	float accumulator = 0.0f;
};

class EngineKernel : public Singleton<EngineKernel>, public IManager
{
	friend class Singleton<EngineKernel>;

public:
	// IManager 인터페이스 구현
	bool Initialize() override;
	void FixedUpdate(float fixedDt) override;
	void Update(float dt) override;
	void Release() override;

	void ProcessFrame(const EngineTime& timeInfo);

	void Render();

private:
	EngineKernel() = default;
	virtual ~EngineKernel() = default;

private:
	std::vector<IManager*> m_EngineManagers;
};

