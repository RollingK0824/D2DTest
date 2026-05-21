#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/IManager.h"
#include "Engine/Core/Math.h"
#include <bitset>
#include <vector>
#include <unordered_map>

enum class EInputContext 
{ 
	MainUI,
	InGame,
	Editor
};

enum class EAction 
{ 
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	Confirm,
	Cancel,
	Debug 
};

enum class EKeyState
{
	None = 0,
	Down = 1,
	Press = 2,
	Up = 3
};

class InputManager : public Singleton<InputManager>, public IManager
{
	friend class Singleton<InputManager>;

public:
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	// 컨텍스트 제어
	void SetContext(EInputContext context) { m_currentContext = context; }
	EInputContext GetContext() const { return m_currentContext; }

	// 액션 기반 상태 조회(컨텍스트별 매핑 적용)
	bool GetKeyDown(EAction action);
	bool GetKey(EAction action);
	bool GetKeyUp(EAction action);

	// 키 코드 기반 상태 조회 (전역 키 기능용 등)
	bool GetKeyDown(int key);
	bool GetKey(int key);
	bool GetKeyUp(int key);

	EKeyState GetKeyState(int keyCode) const { return m_keyStates[keyCode]; }

	Vector2 GetMousePos() const { return m_mousePos; }

private:
	InputManager() = default;
	virtual ~InputManager() = default;

	// 키 매핑 전용 초기화 함수
	void SetupKeyBindings();

	// 상태 레이어 및 액션 매핑 테이블
	EInputContext m_currentContext = EInputContext::MainUI;
	std::unordered_map<EInputContext, std::unordered_map<EAction, int>> m_contextMaps;

	// 입력 상태 저장 자료구조
	std::bitset<256> m_currState;
	std::bitset<256> m_prevState;
	std::vector<EKeyState>	m_keyStates;

	Vector2 m_mousePos;	// 마우스 커서 위치 값
};

