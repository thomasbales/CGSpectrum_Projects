#include <iostream>

using namespace std;

bool CheckIsValidInput();
void ClearCin();

class Vehicle
{
protected:
    int m_totalMiles = 0;
    string m_name;

public:
    Vehicle(string name)
    {
        m_name = name;
    }

    virtual void Drive(int miles)
    {
        m_totalMiles += miles;
    }
};

class Boat : public Vehicle
{
public:
    Boat(string name) : Vehicle(name)
    {

    }

    void Drive(int miles) override
    {
        Vehicle::Drive(miles);

        cout << m_name << " (boat) drove " << miles << " miles." << endl;
        cout << m_name << " has driven a total of " << m_totalMiles << " miles." << endl;
    }
};

class Car : public Vehicle
{
public:
    Car(string name) : Vehicle(name)
    {

    }

    void Drive(int miles) override
    {
        Vehicle::Drive(miles);

        cout << m_name << " (car) drove " << miles << " miles." << endl;
        cout << m_name << " has driven a total of " << m_totalMiles << " miles." << endl;
    }
};

class Airplane : public Vehicle
{
public:
    Airplane(string name) : Vehicle(name)
    {

    }

    void Drive(int miles) override
    {
        Vehicle::Drive(miles);

        cout << m_name << " (airplane) flew " << miles << " miles." << endl;
        cout << m_name << " has flown a total of " << m_totalMiles << " miles." << endl;
    }
};

int main()
{
    string carName;
    string boatName;
    string airplaneName;

    int carMiles;
    int boatMiles;
    int airplaneMiles;

    do
    {
        cout << "Enter car name." << endl;
        cin >> carName;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do
    {
        cout << "Enter amount of miles the car has driven." << endl;
        cin >> carMiles;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    Car car = Car(carName);
    car.Drive(carMiles);

    do
    {
        cout << "Enter boat name." << endl;
        cin >> boatName;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do
    {
        cout << "Enter amount of miles the boat has driven." << endl;
        cin >> boatMiles;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Boat boat = Boat(boatName);
    boat.Drive(boatMiles);

    do
    {
        cout << "Enter airplane name." << endl;
        cin >> airplaneName;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do
    {
        cout << "Enter amount of miles the airplane has driven." << endl;
        cin >> airplaneMiles;
        cout << endl;
    } while (!CheckIsValidInput());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Airplane airplane = Airplane(airplaneName);
    airplane.Drive(airplaneMiles);
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
    //clear fail flag on cin
    cin.clear();
    //ignore extra cin input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
