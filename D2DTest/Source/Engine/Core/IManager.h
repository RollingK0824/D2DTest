#pragma once

class IManager
{
public:
	IManager() = default;
	virtual ~IManager() = default;

	IManager(const IManager&) = delete;
	IManager& operator=(const IManager&) = delete;
	IManager(IManager&&) = delete;
	IManager& operator=(IManager&&) = delete;

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};