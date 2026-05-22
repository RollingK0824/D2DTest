#pragma once
#include "Engine/Core/Singleton.h"
#include <vector>

class Component;

class EngineKernel : public Singleton<EngineKernel>
{
	friend class Singleton<EngineKernel>;

public:
	bool Initialize();
	void FixedUpdate(float fixedDt);
	void Update(float dt);
	void Render();
	void Release();

private:
	EngineKernel() = default;
	virtual ~EngineKernel() = default;

private:
	std::vector<Component*> m_engineManagers;
};

