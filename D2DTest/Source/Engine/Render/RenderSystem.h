#pragma once

#include "Engine/Core/IManager.h"
#include "Engine/Core/Singleton.h"
#include "Engine/Render/RenderCommand.h"
#include <vector>

class RenderSystem : public Singleton<RenderSystem>, public IManager
{
	friend class Singleton<RenderSystem>;

public:
	// IManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Update(float dt)override {}
	virtual void FixedUpdate(float fixedDt) override {}
	virtual void Release() override;

	// 객체가 그리기 요청할 함수
	void SubmitCommand(const RenderCommand& command);

	// 일괄 정렬 후 출력하는 함수
	void RenderAll();


private:
	RenderSystem() = default;
	virtual ~RenderSystem() = default;

private:
	std::vector<RenderCommand> m_commands;
};

