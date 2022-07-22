#include <iostream>

using namespace std;

int main()
{
    float num1;
    float num2;
    float num3;

    float sum;
    float avg;

    cout << "Enter first number" << endl;
    cin >> num1;
    cout << endl;

    cout << "Enter second number" << endl;
    cin >> num2;
    cout << endl;

    cout << "Enter third number" << endl;
    cin >> num3;
    cout << endl;

    sum = num1 + num2 + num3;
    avg = sum / 3;

    cout << "Sum is: " << sum << endl;
    cout << "Average is: " << avg << endl;
}


