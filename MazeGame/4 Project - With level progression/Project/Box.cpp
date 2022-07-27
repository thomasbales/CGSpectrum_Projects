#include <iostream>
#include <windows.h>
#include <vector>
#include <assert.h>

#include "Level.h"
#include "PlacableActor.h"
#include "Box.h"
#include "Player.h"

Box::Box(int x, int y)
	: PlacableActor(x, y)
{

}

void Box::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)m_color);

	std::cout << "0";
	SetConsoleTextAttribute(console, (int)ActorColor::Regular);
}

void Box::Update(Level* level)
{
	
}

void Box::HandleOnCollision(int newPlayerX, int newPlayerY, Player* pPlayer, Level* pLevel)
{
	int deltaX = newPlayerX - pPlayer->GetXPosition();
	int deltaY = newPlayerY - pPlayer->GetYPosition();

	int newBoxX = this->GetXPosition() + deltaX;
	int newBoxY = this->GetYPosition() + deltaY;

	if (pLevel->IsSpace(newBoxX, newBoxY) && !(newBoxX >= pLevel->GetWidth() || newBoxX < 0 || newBoxY > pLevel->GetHeight() || newBoxY < 0))
	{
		PlacableActor* collidedActor = nullptr;

		std::vector<PlacableActor*>* actors = &(pLevel->pActors);
		for (auto actor = actors->begin(); actor != actors->end(); ++actor)
		{
			Box* pTempBox = dynamic_cast<Box*>(*actor);
			if (pTempBox != this)
			{
				if (newBoxX == (*actor)->GetXPosition() && newBoxY == (*actor)->GetYPosition())
				{
					// should only be able to collide with one actor
					assert(collidedActor == nullptr);
					collidedActor = (*actor);
				}
			}
		}

		if (collidedActor == nullptr)
		{
			this->SetPosition(newBoxX, newBoxY);
			pPlayer->SetPosition(newPlayerX, newPlayerY);
		}
	}
}