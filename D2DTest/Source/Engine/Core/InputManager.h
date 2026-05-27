#pragma once
#include "Engine/Core/IManager.h"
#include "Engine/Core/Singleton.h"
#include "Engine/Core/Math.h"
#include <windows.h>
#include <cstdint>

class InputManager : public Singleton<InputManager>, public IManager
{
	friend class Singleton<InputManager>;

public:
	// IManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float fiexdDt) override {}
	virtual void Release() override;

private:
	InputManager() = default;
	virtual ~InputManager() = default;

};

