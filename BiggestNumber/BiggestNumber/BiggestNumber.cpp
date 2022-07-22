#include <iostream>

using namespace std;

bool CheckIsValidInput();
void ClearCin();
float FindBiggestNumber(float array[], int arrayLength);

int main()
{
    int arrayLength;
    float biggestNumber;
    float* array;

    do 
    {
        cout << "Enter array length." << endl;
        cin >> arrayLength;
        cout << endl;
    } while (!CheckIsValidInput() || arrayLength <= 0);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    array = new float[arrayLength];

    for (int i = 0; i < arrayLength; i++)
    {
        do
        {
            cout << "Enter number " << i + 1 << ":" << endl;
            cin >> array[i];
        } while (!CheckIsValidInput());
    }

    cout << endl;

    biggestNumber = FindBiggestNumber(array, arrayLength);

    cout << "The biggest number is: " << biggestNumber << endl;

    delete[] array;
    return 0;
}

bool CheckIsValidInput()
{
    if (cin.fail())
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
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

float FindBiggestNumber(float array[], int arrayLength)
{
    float currentBiggestNumber = array[0];

    for (int i = 1; i < arrayLength; i++)
    {
        if (array[i] > currentBiggestNumber)
        {
            currentBiggestNumber = array[i];
        }
    }

    return currentBiggestNumber;
}