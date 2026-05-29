#include "GameManager.h"
#include "Engine/Core/InputManager.h"
#include "Engine/Render/RenderGizmo.h"
#include "Engine/Core/RandomManager.h"
#include <Windows.h>
bool GameManager::Initialize()
{
	PlayerData playerData;
	playerData.size = 40.0f;
	playerData.speed = 300.0f;
	playerData.position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT -playerData.size };
	m_player = new PlayerData(playerData);

	if (!m_player)return false;

	m_vBalls.clear();

	m_spawnTimer = 1.0f;
	m_isGameOver = false;

	if (!InputManager::GetInstance()->BindAction(MOVE_LEFT, VK_LEFT))return false;
	if (!InputManager::GetInstance()->BindAction(MOVE_RIGHT, VK_RIGHT)) return false;

	return true;
}

void GameManager::Release()
{
	if (m_player != nullptr)
	{
		delete m_player;
		m_player = nullptr;
	}

	while (!m_vBalls.empty())
	{
		if (m_vBalls.back() != nullptr)
		{
			delete m_vBalls.back();
		}
		m_vBalls.pop_back();
	}

	std::vector<BallData*>().swap(m_vBalls);
}

void GameManager::Update(float dt)
{
	if (m_isGameOver)
	{
		if (InputManager::GetInstance()->GetKeyDown('R'))
			Initialize();
		return;
	}

	if (InputManager::GetInstance()->GetKeyPress(MOVE_LEFT))
	{
		if (m_player->position.x <= m_player->size/2+2.5f)
		{

		}
		else
		{
			m_player->position.x -= m_player->speed * dt;
		}
	}
	if (InputManager::GetInstance()->GetKeyPress(MOVE_RIGHT))
	{
		if (m_player->position.x >= SCREEN_WIDTH - (m_player->size / 2+2.5f))
		{

		}
		else
		{
			m_player->position.x += m_player->speed * dt;
		}
	}

	SpawnBall(dt);
	for (auto it = m_vBalls.begin(); it != m_vBalls.end();)
	{
		(*it)->position.y += (*it)->speedY * dt;

		if ((*it)->position.y > SCREEN_HEIGHT)
		{
			it = m_vBalls.erase(it);
		}
		else
		{
			++it;
		}
	}

	CheckCollision();
}

void GameManager::Render()
{
	if (m_isGameOver)
	{
		RenderGizmo::DrawLine({ 100,100 }, { SCREEN_WIDTH - 100,SCREEN_HEIGHT - 100 }, D2D1::ColorF(D2D1::ColorF::Red));
		RenderGizmo::DrawLine({ SCREEN_WIDTH - 100 ,100 }, { 100,SCREEN_HEIGHT - 100 }, D2D1::ColorF(D2D1::ColorF::Red));
		return;
	}

	RenderGizmo::DrawRect(m_player->position, m_player->size, m_player->size, D2D1::ColorF(D2D1::ColorF::Blue));

	for (const auto& it : m_vBalls)
	{
		RenderGizmo::DrawCircle(it->position, it->radius, D2D1::ColorF(D2D1::ColorF::Yellow));
	}
}

void GameManager::SpawnBall(float dt)
{
	m_spawnTimer += dt;

	if (m_spawnTimer >= SPAWN_TIME)
	{
		m_spawnTimer = 0.0f;

		BallData* newBall = new BallData();

		newBall->speedY = 300.0f;
		newBall->radius = 15.0f;
		newBall->position.x = RandomManager::GetInstance()->GetRandomFloat(20+newBall->radius, SCREEN_WIDTH - 20-newBall->radius);
		newBall->position.y = -30.0f;

		m_vBalls.push_back(newBall);
	}
}

void GameManager::CheckCollision()
{
}
