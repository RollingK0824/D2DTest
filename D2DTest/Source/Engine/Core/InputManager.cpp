#include "Engine/Core/InputManager.h"
#include <windows.h>

bool InputManager::Initialize()
{
	m_currState.reset();
	m_prevState.reset();

	SetupKeyBindings();

	return true;
}

void InputManager::Update()
{
	m_prevState = m_currState;

	for (int i = 0; i < 256; i++)
	{
		// GetAsyncKeyState 비트 연산으로 키 상태 판별
		if (GetAsyncKeyState(i) & 0x8000)m_currState.set(i);
		else m_currState.reset(i);
	}

	POINT pt;
	GetCursorPos(&pt);
	m_mousePos.x = pt.x;
	m_mousePos.y = pt.y;
}

void InputManager::Release()
{
	m_currState.reset();
	m_prevState.reset();
}

bool InputManager::GetKeyDown(EAction action)
{
	int keyCode = m_contextMaps[m_currentContext][action];
	return GetKeyDown(keyCode);
}

bool InputManager::GetKey(EAction action)
{
	int keyCode = m_contextMaps[m_currentContext][action];
	return GetKey(keyCode);
}

bool InputManager::GetKeyUp(EAction action)
{
	int keyCode = m_contextMaps[m_currentContext][action];
	return GetKeyUp(keyCode);
}

bool InputManager::GetKeyDown(int key)
{
	return m_currState.test(key) && !m_prevState.test(key); 
}

bool InputManager::GetKey(int key)
{
	return m_currState.test(key);
}

bool InputManager::GetKeyUp(int key)
{
	return !m_currState.test(key) && m_prevState.test(key);
}

void InputManager::SetupKeyBindings()
{
	// MainUI
	m_contextMaps[EInputContext::MainUI][EAction::Confirm] = VK_LBUTTON;
	m_contextMaps[EInputContext::MainUI][EAction::Cancel] = VK_ESCAPE;

	// InGame
	m_contextMaps[EInputContext::InGame][EAction::MoveUp] = 'W';
	m_contextMaps[EInputContext::InGame][EAction::MoveDown] = 'S';
	m_contextMaps[EInputContext::InGame][EAction::MoveLeft] = 'A';
	m_contextMaps[EInputContext::InGame][EAction::MoveRight] = 'D';
	m_contextMaps[EInputContext::InGame][EAction::Cancel] = VK_ESCAPE;

	// Editor
	m_contextMaps[EInputContext::Editor][EAction::Debug] = VK_F3;
}
