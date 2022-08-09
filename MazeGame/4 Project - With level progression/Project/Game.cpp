#include "Game.h"

#include"Input.h"

Game::Game()
	: m_pStateMachine(nullptr)
{

}

void Game::Initialize(GameStateMachine* pStateMachine)
{
	Input::GetInstance().Initialize();

	if (pStateMachine != nullptr)
	{
		pStateMachine->Init();
		m_pStateMachine = pStateMachine;
	}
}

void Game::RunGameLoop()
{
	bool isGameOver = false;

	while (!isGameOver)
	{
		//TODO: add some sort of thread safe sleep function for consistent framerate
		// update with no input
		Update(false);
		// Draw
		Draw();
		// Update with input
		isGameOver = Update();
	}

	Draw();
}

void Game::Deinitialize()
{
	Input::GetInstance().Deinitialize();

	if (m_pStateMachine != nullptr)
		m_pStateMachine->Cleanup();
}

bool Game::Update(bool processInput)
{
	return m_pStateMachine->UpdateCurrentState(processInput);
}

void Game::Draw()
{
	m_pStateMachine->DrawCurrentState();
}
