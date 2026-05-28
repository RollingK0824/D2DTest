#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/Define.h"
#include <vector>

class ISystem;
class IUpdatable;
class IRenderable;

class EngineKernel : public Singleton<EngineKernel>
{
	friend class Singleton<EngineKernel>;

public:
	bool Initialize();
	void ProcessFrame();
	void Release();

	void RegisterManager(ISystem* manager);

private:
	EngineKernel() = default;
	virtual ~EngineKernel() = default;

private:
	std::vector<ISystem*> m_vAllSystems;
	std::vector<IUpdatable*> m_vUpdatableSystems;
	std::vector<IRenderable*> m_vRenderableSystems;
};

