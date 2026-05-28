#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
class SceneManager : public Singleton<SceneManager>, public ISystem
{
public:
	virtual bool Initialize() override {}
	virtual void Release() override {}

private:
};

