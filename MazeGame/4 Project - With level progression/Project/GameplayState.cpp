#include "GameplayState.h"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <assert.h>
#include <thread>
#include <chrono>

#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Money.h"
#include "Goal.h"
#include "Box.h"
#include "AudioManager.h"
#include "Utility.h"
#include "StateMachineExampleGame.h"
#include "Input.h"

#pragma optimize("", off)

using namespace std;

GameplayState::GameplayState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_beatLevel(false)
	, m_skipFrameCount(0)
	, m_currentLevel(0)
	, m_pLevel(nullptr)
{
	m_LevelNames.push_back("Level1.txt");
	m_LevelNames.push_back("Level2.txt");
	m_LevelNames.push_back("Level3.txt");
}

GameplayState::~GameplayState()
{
	delete m_pLevel;
	m_pLevel = nullptr;
}

bool GameplayState::Load()
{
	if (m_pLevel)
	{
		delete m_pLevel;
		m_pLevel = nullptr;
	}

	m_pLevel = new Level();
	
	return m_pLevel->Load(m_LevelNames.at(m_currentLevel), m_player.GetXPositionPointer(), m_player.GetYPositionPointer());

}

void GameplayState::Enter()
{
	Load();

	Input::GetInstance().OnKeyDownLEFT.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownRIGHT.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownUP.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownDOWN.AddListener(this, &GameplayState::MovePlayer);

	Input::GetInstance().OnKeyDownW.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownA.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownS.AddListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownD.AddListener(this, &GameplayState::MovePlayer);

	Input::GetInstance().OnKeyDownZ.AddListener(this, &GameplayState::DropKey);
	Input::GetInstance().OnKeyDownESC.AddListener(this, &GameplayState::LoadMainMenu);
}

bool GameplayState::Update(bool processInput)
{
	//TODO: incorporate processInput and beatLevel bools into input

	m_pLevel->UpdateActors();

	//if (processInput && !m_beatLevel)
	//{
	//	
	//}

	//if (m_beatLevel)
	//{
	//	++m_skipFrameCount;
	//	if (m_skipFrameCount > kFramesToSkip)
	//	{
	//		m_beatLevel = false;
	//		m_skipFrameCount = 0;
	//		++m_currentLevel;
	//		if (m_currentLevel == m_LevelNames.size())
	//		{
	//			Utility::WriteHighScore(m_player.GetMoney());

	//			AudioManager::GetInstance()->PlayWinSound();
	//			
	//			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Win);
	//		}
	//		else
	//		{
	//			// On to the next level
	//			Load();
	//		}

	//	}
	//}


	this_thread::sleep_for(chrono::milliseconds(200));
	return false;
}

void GameplayState::HandleCollision(int newPlayerX, int newPlayerY)
{
	//TODO: update handle collision logic

	if (m_pLevel->IsSpace(newPlayerX, newPlayerY)) // no collision
	{
		m_player.SetPosition(newPlayerX, newPlayerY);
	}
	else if (m_pLevel->IsWall(newPlayerX, newPlayerY))
	{
		// wall collision, do nothing
	}
}

void GameplayState::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	Utility::ClearScreen();

	m_pLevel->Draw();

	// Set cursor position for player 
	COORD actorCursorPosition;
	actorCursorPosition.X = m_player.GetXPosition();
	actorCursorPosition.Y = m_player.GetYPosition();
	SetConsoleCursorPosition(console, actorCursorPosition);
	m_player.Draw();

	// Set the cursor to the end of the level
	COORD currentCursorPosition;
	currentCursorPosition.X = 0;
	currentCursorPosition.Y = m_pLevel->GetHeight();
	SetConsoleCursorPosition(console, currentCursorPosition);

	DrawHUD(console);
}

void GameplayState::DrawHUD(const HANDLE& console)
{
	cout << endl;

	// Top Border
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;

	// Left Side border
	cout << Level::WAL;

	cout << " wasd-move " << Level::WAL << " z-drop key " << Level::WAL;

	cout << " $:" << m_player.GetMoney() << " " << Level::WAL;
	cout << " lives:" << m_player.GetLives() << " " << Level::WAL;
	cout << " key:";
	if (m_player.HasKey())
	{
		m_player.GetKey()->Draw();
	}
	else
	{
		cout << " ";
	}

	// RightSide border
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);

	COORD pos;
	pos.X = m_pLevel->GetWidth() - 1;
	pos.Y = csbi.dwCursorPosition.Y;
	SetConsoleCursorPosition(console, pos);

	cout << Level::WAL;
	cout << endl;

	// Bottom Border
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;
}

void GameplayState::Exit()
{
	Input::GetInstance().OnKeyDownLEFT.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownRIGHT.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownUP.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownDOWN.RemoveListener(this, &GameplayState::MovePlayer);

	Input::GetInstance().OnKeyDownW.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownA.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownS.RemoveListener(this, &GameplayState::MovePlayer);
	Input::GetInstance().OnKeyDownD.RemoveListener(this, &GameplayState::MovePlayer);

	Input::GetInstance().OnKeyDownZ.RemoveListener(this, &GameplayState::LoadMainMenu);
	Input::GetInstance().OnKeyDownESC.RemoveListener(this, &GameplayState::DropKey);
}

void GameplayState::MovePlayer(char key)
{
	int newPlayerX = m_player.GetXPosition();
	int newPlayerY = m_player.GetYPosition();

	if ((key == kLeftArrow) || key == 'A')
	{
		newPlayerX--;
	}
	else if ((key == kRightArrow) || (char)key == 'D')
	{
		newPlayerX++;
	}
	else if ((key == kUpArrow) || (char)key == 'W')
	{
		newPlayerY--;
	}
	else if ((key == kDownArrow) || (char)key == 'S')
	{
		newPlayerY++;
	}

	// If position changed
	if (newPlayerX != m_player.GetXPosition() || newPlayerY != m_player.GetYPosition())
	{
		HandleCollision(newPlayerX, newPlayerY);
	}
}

void GameplayState::LoadMainMenu(char key)
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
}

void GameplayState::DropKey(char key)
{
	m_player.DropKey();
}