#pragma once
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	// 복사 이동 제한
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	// 생명 주기 함수들
	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float fiexedDt) {}
	virtual void Update(float dt) {}
	virtual void Release() = 0;

	// 활성화 상태 제어
	bool IsActive() const { return m_bIsActive; }
	void SetActive(bool active) { m_bIsActive = active; }

private:
	bool m_bIsActive = true;
};

