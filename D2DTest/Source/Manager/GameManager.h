#pragma once
#include "Engine/Core/Singleton.h"
#include "Engine/Core/ISystem.h"
#include "Engine/Core/IUpdatable.h"
#include "Engine/Core/IRenderable.h"
#include "Engine/Core/Define.h"
#include <vector>

struct PlayerData
{
	Pos2D position;
	float size;
	float speed;
};

struct BallData
{
	Pos2D position;
	float radius;
	float speedY;
};

class GameManager : public Singleton<GameManager>, public ISystem, public IUpdatable, public IRenderable
{
	friend class Singleton<GameManager>;
public:
	virtual bool Initialize() override;
	virtual void Release() override;

	virtual void Update(float dt)override;

	virtual void Render() override;

private:
	GameManager() = default;
	virtual ~GameManager() = default;

	void SpawnBall(float dt);
	void CheckCollision();

	PlayerData* m_player = nullptr;
	std::vector<BallData*> m_vBalls;

	float m_spawnTimer = 0.0f;
	bool m_isGameOver = false;
};

