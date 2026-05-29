#include "Engine/Core/RandomManager.h"

bool RandomManager::Initialize()
{
	std::random_device rd;
	m_randomEngine.seed(rd());
	return true;
}

int RandomManager::GetRandomInt(int min, int max)
{
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution<int> dist(min, max);
	return dist(m_randomEngine);
}

float RandomManager::GetRandomFloat(float min, float max)
{
	if (min > max) std::swap(min, max);
	std::uniform_real_distribution<float> dist(min, max);
	return dist(m_randomEngine);
}
