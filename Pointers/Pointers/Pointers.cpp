#include <iostream>

using namespace std;

template<typename T>
T GetInput(string message);
bool CheckIsValidInput();
void ClearCin();

int main()
{
	int a;
	int b;

	int* pA = &a;
	int* pB = &b;

	*pA = GetInput<int>("Enter integer A.");
	*pB = GetInput<int>("Enter integerB.");

	cout << "pA points to: " << *pA << endl;
	cout << "pB points to: " << *pB << endl;
}

template<typename T>
T GetInput(string message)
{
	T input;
	do
	{
		cout << message << endl;
		cin >> input;
		cout << endl;
	} while (!CheckIsValidInput());
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	return input;
}

bool CheckIsValidInput()
{
	if (cin.fail())
	{
		ClearCin();
		cout << "Invalid input." << endl;

		return false;
	}

	return true;
}

void ClearCin()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
