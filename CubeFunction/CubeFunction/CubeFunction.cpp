#include <iostream>

double Cube(double base);

using namespace std;
int main()
{
    double base;

    cout << "Enter base number" << endl;
    cin >> base;
    cout << endl;

    base = Cube(base);

    cout << "Base cubed is: " << base << endl;
}

double Cube(double base)
{
    return base * base * base;
}
