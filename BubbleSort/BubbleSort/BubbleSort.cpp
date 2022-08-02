#include <iostream>

using namespace std;

void FillArray(int arrayToFill[], int size);
void BubbleSort(int arrayToSort[], int size);

int main()
{
    srand(time(NULL));

    int bunchOfNumbers[10];

    FillArray(bunchOfNumbers, 10);
    BubbleSort(bunchOfNumbers, 10);

    cout << "Sorted numbers:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << bunchOfNumbers[i] << ", ";
    }

    cout << endl;
}

void FillArray(int arrayToFill[], int size)
{
    cout << "Unsorted numbers:" << endl;
    for (int i = 0; i < size; i++)
    {
        arrayToFill[i] = rand() % 10;
        cout << arrayToFill[i] << ", ";
    }
    
    cout << endl;
}

void BubbleSort(int arrayToSort[], int size)
{
    bool doAnotherPass = false;

    do
    {
        doAnotherPass = false;

        for (int i = 0; i < size - 1; i++)
        {
            if (arrayToSort[i] > arrayToSort[i + 1])
            {
                int temp = arrayToSort[i];
                arrayToSort[i] = arrayToSort[i + 1];
                arrayToSort[i + 1] = temp;

                doAnotherPass = true;
            }
        }
    } while (doAnotherPass);
}

