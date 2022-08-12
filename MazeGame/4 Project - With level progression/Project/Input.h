#pragma once

#include<thread>
#include<iostream>
#include <conio.h>
#include <mutex>

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
	template<class... Args>
	class InputEvent
	{
		Event<Args...>* m_pEvent;

	public:

		InputEvent()
			:m_pEvent(new Event<Args...>())
		{

		}

		~InputEvent()
		{
			delete m_pEvent;
			m_pEvent = nullptr;
		}

		static mutex& GetMutex()
		{
			static mutex inputThreadLock;
			return inputThreadLock;
		}

		void AddListener(void (*pStaticFunction)(Args...))
		{
			//TODO: make thread safe w/ lock
			GetMutex().lock();
			m_pEvent->AddListener(pStaticFunction);
			GetMutex().unlock();
		}

		template<class C>
		void AddListener(C* pInstance, void (C::* pMemberFunction)(Args...))
		{
			//TODO: make thread safe w/ lock
			GetMutex().lock();
			m_pEvent->AddListener(pInstance, pMemberFunction);
			GetMutex().unlock();
		}

		void RemoveListener(void (*pStaticFunction)(Args...))
		{
			//TODO: make thread safe w/ lock
			GetMutex().lock();
			m_pEvent->RemoveListener(pStaticFunction);
			GetMutex().unlock();
		}

		template<class C>
		void RemoveListener(C* pInstance, void (C::* pMemberFunction)(Args...))
		{
			//TODO: make thread safe w/ lock
			GetMutex().lock();
			m_pEvent->RemoveListener(pInstance, pMemberFunction);
			GetMutex().unlock();
		}

		void Invoke(Args... args)
		{
			GetMutex().lock();
			m_pEvent->Invoke(args...);
			GetMutex().unlock();
		}

		void operator()(Args... args)
		{
			Invoke(args...);
		}
	};

	InputEvent<char> OnKeyDownESC = InputEvent<char>();

	InputEvent<char> OnKeyDown1 = InputEvent<char>();
	InputEvent<char> OnKeyDown2 = InputEvent<char>();
	InputEvent<char> OnKeyDown3 = InputEvent<char>();
	InputEvent<char> OnKeyDown4 = InputEvent<char>();

	InputEvent<char> OnKeyDownW = InputEvent<char>();
	InputEvent<char> OnKeyDownA = InputEvent<char>();
	InputEvent<char> OnKeyDownS = InputEvent<char>();
	InputEvent<char> OnKeyDownD = InputEvent<char>();
	InputEvent<char> OnKeyDownZ = InputEvent<char>();

	InputEvent<char> OnKeyDownLEFT = InputEvent<char>();
	InputEvent<char> OnKeyDownRIGHT = InputEvent<char>();
	InputEvent<char> OnKeyDownUP = InputEvent<char>();
	InputEvent<char> OnKeyDownDOWN = InputEvent<char>();

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
