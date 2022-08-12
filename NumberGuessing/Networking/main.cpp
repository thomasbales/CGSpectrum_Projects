#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>

#include "NetworkHost.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

using namespace std;

int main(int argc, char** argv)
{
    int intInput;
    string username;
    string serverAddress;
    int serverPort;
    HostConfiguration configuration = HostConfiguration::None;

    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        cout << "An error occurred while initializing ENet.\n" << endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);
    
    cout << "Please enter your username." << endl;
    getline(cin, username);
    cout << endl;

    do
    {
        cout << "1) Create server" << endl;
        cout << "2) Create client" << endl;
        cin >> intInput;
        cin.ignore(200, '\n');
        cout << endl;

        if (intInput == 1)
        {
            configuration = HostConfiguration::Server;
            break;
        }
        else if (intInput == 2)
        {
            configuration = HostConfiguration::Client;
            break;
        }
        else
        {
            cout << "Invalid input" << endl << endl;
        }
    } while (1);

    

    NetworkHost::Initialize(username, configuration);

    if (configuration == HostConfiguration::Server)
    {
        while (1)
        {
            //TODO: get user input, if ESC, close
            cin >> intInput;
        }
    }

    if (configuration == HostConfiguration::Client)
    {
        cout << "Please enter server address." << endl;
        getline(cin, serverAddress);
        cout << endl;

        cout << "Please enter server port." << endl;
        cin >> serverPort;
        cin.ignore(200, '\n');
        cout << endl;

        NetworkHost::ConnectToServer(serverAddress, serverPort);

        cout << "Guess a number." << endl;

        while (1)
        {
            cin >> intInput;
            //TODO: if input is ESC, close

            IntPacket packet(username, intInput);
            NetworkHost::SendPacket(packet);
        }
    }

    

    cout << "Exiting..." << endl << endl;
    NetworkHost::Deinitialize();
    
    return EXIT_SUCCESS;
}

