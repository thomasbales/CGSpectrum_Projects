#pragma once
#include "PlacableActor.h"
#include "Level.h"
#include "Player.h"
class Box : public PlacableActor
{
public:
	Box(int x, int y);

	virtual ActorType GetType() override { return ActorType::Box; }
	virtual void Draw() override;
	virtual void Update(Level* level) override;

	void HandleOnCollision(int newPlayerX, int newPlayerY, Player* pPlayer, Level* pLevel);
};

