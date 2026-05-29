#include "Engine/Core/InputManager.h"
#include <Windows.h>

bool InputManager::Initialize()
{
	m_vKeyStates.resize(256, KeyState::NONE);
	m_vPrevStates.resize(256, false);

	return true;
}

void InputManager::Release()
{
	m_actionMappings.clear();

	m_vKeyStates.clear();
	m_vPrevStates.clear();

	std::vector<KeyState>().swap(m_vKeyStates);
	std::vector<bool>().swap(m_vPrevStates);
}

void InputManager::Update(float dt)
{
	for (int vkCode = 0; vkCode < 256; ++vkCode)
	{
		// GetAsyncKeyState의 최상위 비트(0x8000)를 체크하여 현재 눌려있는지 확인
		bool isCurrentPressed = (GetAsyncKeyState(vkCode) & 0x8000) != 0;
		bool isPrevPressed = m_vPrevStates[vkCode];

		if (isCurrentPressed)
		{
			if (!isPrevPressed)
				m_vKeyStates[vkCode] = KeyState::DOWN;
			else
				m_vKeyStates[vkCode] = KeyState::PRESS;
		}
		else
		{
			if (isPrevPressed)
				m_vKeyStates[vkCode] = KeyState::UP;
			else
				m_vKeyStates[vkCode] = KeyState::NONE;
		}

		// 현재 상태를 다음 프레임을 위한 이전 상태로 백업
		m_vPrevStates[vkCode] = isCurrentPressed;
	}
}

bool InputManager::GetKeyDown(const std::string& actionName) const
{
	auto it = m_actionMappings.find(actionName);
	if (it == m_actionMappings.end()) return false; // 매핑된 키가 없으면 패스
	return GetKeyDown(it->second);

}

bool InputManager::GetKeyPress(const std::string& actionName) const
{
	auto it = m_actionMappings.find(actionName);
	if (it == m_actionMappings.end()) return false;
	return GetKeyPress(it->second);
}

bool InputManager::GetKeyUp(const std::string& actionName) const
{
	auto it = m_actionMappings.find(actionName);
	if (it == m_actionMappings.end()) return false;
	return GetKeyUp(it->second);
}