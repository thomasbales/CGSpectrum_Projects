#pragma once
#include "GameState.h"

class StateMachineExampleGame;

class MainMenuState : public GameState
{
	StateMachineExampleGame* m_pOwner;
	bool m_shouldQuit = false;

	void LoadLevel(char key);
	void ShouldQuit(char key);
	void LoadSettings(char key);
	void LoadHighScore(char key);

public:
	MainMenuState(StateMachineExampleGame* pOwner);
	~MainMenuState() = default;

	virtual bool Update(bool processInput = true) override;
	virtual void Draw() override;
	void Enter() override;
	void Exit() override;
};

