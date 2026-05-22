#pragma once
#include "Engine/Core/Component.h"
#include "Engine/Core/Singleton.h"
#include "Engine/Core/Math.h"
#include <windows.h>
#include <cstdint>



class InputManager : public Singleton<InputManager>, public Component
{
	friend class Singleton<InputManager>;

public:
	virtual bool Initialize() override;
	virtual void FixedUpdate(float fiexdDt) override {}
	virtual void Update(float dt) override;
	virtual void Release() override;

	

private:
	InputManager() = default;
	virtual ~InputManager() = default;

};

