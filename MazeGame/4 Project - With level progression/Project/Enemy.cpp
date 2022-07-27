#include "Enemy.h"
#include "Level.h"
#include "Player.h"
#include <iostream>
#include <assert.h>

Enemy::Enemy(int x, int y, int deltaX, int deltaY)
	: PlacableActor(x, y)
	, m_currentMovementX(0)
	, m_currentMovementY(0)
	, m_directionX(0)
	, m_directionY(0)
	, m_movementInX(deltaX)
	, m_movementInY(deltaY)
{
	InitDirection();
}

void Enemy::InitDirection()
{
	if (m_movementInX != 0)
	{
		m_directionX = 1;
	}
	if (m_movementInY != 0)
	{
		m_directionY = 1;
	}
}

void Enemy::Draw()
{
	std::cout << (char)153;
}

void Enemy::Update(Level* level)
{
	if (m_movementInX != 0)
	{
		UpdateDirection(m_currentMovementX, m_directionX, m_movementInX, level);
	}
	if (m_movementInY != 0)
	{
		UpdateDirection(m_currentMovementY, m_directionY, m_movementInY, level);
	}
}

void Enemy::UpdateDirection(int& current, int& direction, int& movement, Level* level)
{
	current += direction;
	if (std::abs(current) > movement)
	{
		current = movement * direction;
		direction *= -1;
	}

	int newX = m_pPosition->x + m_directionX;
	int newY = m_pPosition->y + m_directionY;

	if (CheckForCollision(newX, newY, level))
	{
		direction *= -1;
		newX = m_pPosition->x + m_directionX;
		newY = m_pPosition->y + m_directionY;

		if (CheckForCollision(newX, newY, level))
		{
			direction *= -1;
			newX = m_pPosition->x;
			newY = m_pPosition->y;
		}
	}

	this->SetPosition(newX, newY);
}

bool Enemy::CheckForCollision(int newX, int newY, Level* pLevel)
{
	if (pLevel->IsSpace(newX, newY) && !(newX >= pLevel->GetWidth() || newX < 0 || newY > pLevel->GetHeight() || newY < 0))
	{
		PlacableActor* collidedActor = nullptr;

		std::vector<PlacableActor*>* actors = &(pLevel->pActors);
		for (auto actor = actors->begin(); actor != actors->end(); ++actor)
		{
			Enemy* pTempEnemy = dynamic_cast<Enemy*>(*actor);
			Player* pTempPlayer = dynamic_cast<Player*>(*actor);
			if (pTempEnemy != this && pTempPlayer == nullptr)
			{
				if (newX == (*actor)->GetXPosition() && newY == (*actor)->GetYPosition())
				{
					// should only be able to collide with one actor
					assert(collidedActor == nullptr);
					collidedActor = (*actor);
				}
			}
		}

		if (collidedActor == nullptr)
		{
			return false;
		}
		return true;
	}
}

