#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/Define.h"
#include "Engine/Core/ISystem.h"
#include "Engine/Core/IUpdatable.h"
#include <vector>
#include <string>
#include <map>

enum class KeyState { NONE, DOWN, PRESS, UP };

class InputManager : public Singleton<InputManager>, public ISystem, public IUpdatable
{
	friend class Singleton<InputManager>;

public:
	virtual bool Initialize() override;
	virtual void Release() override;
	virtual void Update(float dt) override;

	bool BindAction(const std::string& actionName, int vkCode)
	{
		auto it = m_actionMappings.find(actionName);
		if (it != m_actionMappings.end()) return false;

		m_actionMappings[actionName] = vkCode;

		return true;
	}


	bool GetKeyDown(int vkCode) const { return m_vKeyStates[vkCode] == KeyState::DOWN; }
	bool GetKeyPress(int vkCode) const { return m_vKeyStates[vkCode] == KeyState::PRESS; }
	bool GetKeyUp(int vkCode) const { return m_vKeyStates[vkCode] == KeyState::UP; }

	bool GetKeyDown(const std::string& actionName) const;
	bool GetKeyPress(const std::string& actionName) const;
	bool GetKeyUp(const std::string& actionName) const;

private:
	InputManager() = default;
	virtual ~InputManager() = default;

	std::vector<KeyState> m_vKeyStates;
	std::vector<bool> m_vPrevStates;
	std::map<std::string, int> m_actionMappings;
};

