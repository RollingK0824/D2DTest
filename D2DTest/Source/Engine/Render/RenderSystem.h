#pragma once

#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
#include "Engine/Core/IRenderable.h"
#include "Engine/Render/RenderCommand.h"
#include <vector>

class RenderSystem : public Singleton<RenderSystem>, public ISystem, public IRenderable
{
	friend class Singleton<RenderSystem>;

public:
	// IManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Release() override;

	virtual void Render() override;

	// 객체가 그리기 요청할 함수
	void SubmitCommand(const RenderCommand& command);

private:
	RenderSystem() = default;
	virtual ~RenderSystem() = default;

private:
	std::vector<RenderCommand> m_commands;
};

