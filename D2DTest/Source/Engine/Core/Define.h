#pragma once

#define ENABLE_RESOURCE_TEST 0
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 800
#define MOVE_LEFT "MoveLeft"
#define MOVE_RIGHT "MoveRight"
#define SPAWN_TIME 0.5f


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