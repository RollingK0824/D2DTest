#include "Engine/Core/EngineKernel.h"
#include "Engine/Core/ISystem.h"
#include "Engine/Core/IUpdatable.h"
#include "Engine/Core/IRenderable.h"
#include "Engine/Core/TimeManager.h"

#if ENABLE_RESOURCE_TEST
#include "ResourceTest.h"
#endif // ENABLE_RESOURCE_TEST

bool EngineKernel::Initialize()
{
	for (const auto& it : m_vAllSystems)
	{
		if (!it->Initialize())return false;
	}
	return true;
}

void EngineKernel::Release()
{
	for (auto it = m_vAllSystems.rbegin(); it != m_vAllSystems.rend(); ++it)
	{
		(*it)->Release();
	}

	m_vAllSystems.clear();
	m_vUpdatableSystems.clear();
	m_vRenderableSystems.clear();
}

void EngineKernel::ProcessFrame()
{
	TimeManager* pTime = TimeManager::GetInstance();
	
	pTime->Update(0.0f);

	float dt = pTime->GetDeltaTime();
	float fixedDt = pTime->GetFixedDeltaTime();

	while (pTime->AccumulateTime())
	{
		for (auto* sys : m_vUpdatableSystems) sys->FixedUpdate(fixedDt);
		pTime->ConsumeFixedTick();
	}

#if ENABLE_RESOURCE_TEST
	Test_Update(dt);
#endif // ENABLE_RESOURCE_TEST


	for (auto* sys : m_vUpdatableSystems)sys->Update(dt);
	for (auto* sys : m_vUpdatableSystems)sys->LateUpdate(dt);

	for (auto* sys : m_vRenderableSystems)sys->PreRender();
#if ENABLE_RESOURCE_TEST
	Test_Render();
#endif // ENABLE_RESOURCE_TEST
	for (auto* sys : m_vRenderableSystems)sys->Render();
	for (auto* sys : m_vRenderableSystems)sys->PostRender();
}


void EngineKernel::RegisterManager(ISystem* manager)
{
	if (manager == nullptr)return;
	m_vAllSystems.push_back(manager);

	if (auto* updatable = dynamic_cast<IUpdatable*>(manager))
	{
		m_vUpdatableSystems.push_back(updatable);
	}

	if (auto* renderable = dynamic_cast<IRenderable*>(manager))
	{
		m_vRenderableSystems.push_back(renderable);
	}
}
