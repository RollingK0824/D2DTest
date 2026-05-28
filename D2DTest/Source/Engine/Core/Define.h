#pragma once

#define ENABLE_RESOURCE_TEST 1

struct Pos2D
{
	float x = 0.0f;
	float y = 0.0f;
};

struct EngineTime
{
	float deltaTime = 0.0f;
	float fixedDeltaTime = 0.0f;
	float accumulator = 0.0f;
};