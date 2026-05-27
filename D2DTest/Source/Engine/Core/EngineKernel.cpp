#include "EngineKernel.h"

bool EngineKernel::Initialize()
{

	return false;
}

void EngineKernel::FixedUpdate(float fixedDt)
{
	for (IManager* manager : m_EngineManagers)
	{
		manager->FixedUpdate(fixedDt);
	}
}

void EngineKernel::Update(float dt)
{
	for (IManager* manager : m_EngineManagers)
	{
		manager->Update(dt);
	}
}

void EngineKernel::Release()
{
	for (auto it = m_EngineManagers.rbegin(); it != m_EngineManagers.rend(); ++it)
	{
		(*it)->Release();
	}
}

void EngineKernel::ProcessFrame(const EngineTime& timeInfo)
{
	float remainingTime = timeInfo.accumulator;
	

	this->FixedUpdate(timeInfo.fixedDeltaTime);

	this->Update(timeInfo.deltaTime);

	this->Render();
}

void EngineKernel::Render()
{
}
