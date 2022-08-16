#include "MainMenuState.h"

#include <iostream>
#include<thread>
#include<chrono>

#include "StateMachineExampleGame.h"
#include "Utility.h"
#include"Input.h"

using namespace std;

MainMenuState::MainMenuState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
{
}

void MainMenuState::Enter()
{
	Input::GetInstance().OnKeyDownESC.AddListener(this, &MainMenuState::ShouldQuit);
	Input::GetInstance().OnKeyDown1.AddListener(this, &MainMenuState::LoadLevel);
	Input::GetInstance().OnKeyDown2.AddListener(this, &MainMenuState::LoadHighScore);
	Input::GetInstance().OnKeyDown3.AddListener(this, &MainMenuState::LoadSettings);
	Input::GetInstance().OnKeyDown4.AddListener(this, &MainMenuState::ShouldQuit);
}

bool MainMenuState::Update(bool processInput)
{
	//TODO: incorporate processInput and beatLevel bools into input
	this_thread::sleep_for(chrono::milliseconds(100));
	return m_shouldQuit;
}

void MainMenuState::Draw()
{
	Utility::ClearScreen();
	cout << endl << endl << endl;
	cout << "          - - - MAIN MENU - - -" << endl << endl;
	cout << "             " << 1 << ". Play " << endl;
	cout << "             " << 2 << ". High Score " << endl;
	cout << "             " << 3 << ". Settings " << endl;
	cout << "             " << 4 << ". Quit " << endl;
}

void MainMenuState::Exit()
{
	Input::GetInstance().OnKeyDownESC.RemoveListener(this, &MainMenuState::ShouldQuit);
	Input::GetInstance().OnKeyDown1.RemoveListener(this, &MainMenuState::LoadLevel);
	Input::GetInstance().OnKeyDown2.RemoveListener(this, &MainMenuState::LoadHighScore);
	Input::GetInstance().OnKeyDown3.RemoveListener(this, &MainMenuState::LoadSettings);
	Input::GetInstance().OnKeyDown4.RemoveListener(this, &MainMenuState::ShouldQuit);
}

void MainMenuState::LoadLevel(char key)
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Gameplay);
}

void MainMenuState::ShouldQuit(char key)
{
	m_shouldQuit = true;
}

void MainMenuState::LoadSettings(char key)
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::HighScore);
}

void MainMenuState::LoadHighScore(char key)
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Settings);
}
