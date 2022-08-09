#pragma once

#include<thread>
#include<iostream>
#include <conio.h>

#include"Event.h"

using namespace std;

constexpr int kEscapeKey = 27;

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;

class Input
{
	Input()
	{

	}

	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	thread* m_pThread = nullptr;

	static void HandleInput()
	{
		while (1)
		{
			int input = _getch();
			int arrowInput = 0;

			switch (input)
			{
			case kEscapeKey:
				GetInstance().OnKeyDownESC(kEscapeKey);
				break;
			case '1':
				GetInstance().OnKeyDown1('1');
				break;

			case '2':
				GetInstance().OnKeyDown2('2');
				break;

			case '3':
				GetInstance().OnKeyDown3('3');
				break;

			case '4':
				GetInstance().OnKeyDown4('4');
				break;

			case 'w':
			case 'W':
				GetInstance().OnKeyDownW('W');
				break;

			case 'a':
			case 'A':
				GetInstance().OnKeyDownA('A');
				break;

			case 's':
			case 'S':
				GetInstance().OnKeyDownS('S');
				break;

			case 'd':
			case 'D':
				GetInstance().OnKeyDownD('D');
				break;

			case 'z':
			case 'Z':
				GetInstance().OnKeyDownZ('Z');
				break;

			case kArrowInput:
				arrowInput = _getch();

				switch (arrowInput)
				{
				case kLeftArrow:
					GetInstance().OnKeyDownLEFT(kLeftArrow);
					break;
				case kRightArrow:
					GetInstance().OnKeyDownRIGHT(kRightArrow);
					break;
				case kUpArrow:
					GetInstance().OnKeyDownUP(kUpArrow);
					break;
				case kDownArrow:
					GetInstance().OnKeyDownDOWN(kDownArrow);
					break;
				default:
					;
				}
				break;

			default:
				;
			}
		}
	}

public:
	//TODO: add input events that other functions can subscribe too
	Event<char> OnKeyDownESC;

	Event<char> OnKeyDown1;
	Event<char> OnKeyDown2;
	Event<char> OnKeyDown3;
	Event<char> OnKeyDown4;

	Event<char> OnKeyDownW;
	Event<char> OnKeyDownA;
	Event<char> OnKeyDownS;
	Event<char> OnKeyDownD;
	Event<char> OnKeyDownZ;

	Event<char>OnKeyDownLEFT;
	Event<char>OnKeyDownRIGHT;
	Event<char>OnKeyDownUP;
	Event<char>OnKeyDownDOWN;

	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

	void Initialize()
	{
		if (m_pThread == nullptr)
		{
			m_pThread = new thread(HandleInput);
		}

	}

	void Deinitialize()
	{
		//TODO: add a function that SAFELY closes the current input thread, if there is one
	}

	//TODO: current thread should lock if we add/remove listeners
};
