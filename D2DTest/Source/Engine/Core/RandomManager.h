#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
#include <random>

class RandomManager : public Singleton<RandomManager>, public ISystem
{
	friend class Singleton<RandomManager>;
public:
	virtual bool Initialize() override;
	virtual void Release() override {};

	int GetRandomInt(int min, int max);
	float GetRandomFloat(float min, float max);
private:
	RandomManager() = default;
	virtual ~RandomManager() = default;

private:
	std::mt19937 m_randomEngine;
};