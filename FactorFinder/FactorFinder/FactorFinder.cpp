#include <iostream>
#include <stdlib.h>

using namespace std;

bool CheckIsValidInput(int input);
void ClearCin();
void PrintFactors(int number);

int main()
{
    int input;

    do
    {
        cout << "Please enter a positive integer." << endl;
        cin >> input;
        cout << endl;
    } while (!CheckIsValidInput(input));

    cout << "Factors of " << input << " are:" << endl;
    PrintFactors(input);

    return 0;
}

bool CheckIsValidInput(int input)
{
    if (cin.fail() || input <= 0)
    {
        ClearCin();
        system("cls");
        cout << "Invalid input." << endl;

        return false;
    }

    return true;
}

void ClearCin()
{
    //clear fail flag on cin
    cin.clear();
    //ignore extra cin input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void PrintFactors(int number)
{
    if (number == 1)
    {
        cout << 1 << endl;
        return;
    }

    int upperFactor = number;
    int lowerFactor = 1;

    while (lowerFactor < upperFactor)
    {
        if (number % lowerFactor != 0)
        {
            lowerFactor++;
            continue;
        }

        upperFactor = number / lowerFactor;

        cout << lowerFactor << " and " << upperFactor << endl;
        lowerFactor++;
    }
}
