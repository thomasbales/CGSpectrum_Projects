#include <iostream>

using namespace std;

int main()
{
    int year;

    cout << "Please enter the current year." << endl;
    cin >> year;
    cout << endl;

    if (year % 4 == 0)
    {
        cout << year << " is a leap year." << endl;
    }
    else
    {
        cout << year << " is not a leap year." << endl;
    }

    return 0;
}
