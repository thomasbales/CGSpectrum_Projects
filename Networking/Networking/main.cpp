#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

using namespace std;

bool shouldExit = false;
string username;

ENetAddress address;
ENetHost* server;
ENetHost* client;

void CreateServer();
void CreateClient();
void ConnectToServer();
void StartListeningForEvents(ENetHost*);
void StopListeningForEvents(thread eventThread);
void SendUsermessage(ENetHost* host);
void SendPacket(ENetHost* host, ENetPeer* peer, string message);

int main(int argc, char** argv)
{
    int intInput;

    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        cout << "An error occurred while initializing ENet.\n" << endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);
    
    //TODO: ask for username
    cout << "Please enter your username." << endl;
    getline(cin, username);
    cout << endl;

    cout << "1) Create server" << endl;
    cout << "2) Create client" << endl;
    cin >> intInput;
    cout << endl;

    if (intInput == 1)
    {
        CreateServer();
        thread eventThread = thread(StartListeningForEvents, server);

        cout << "Server created. Press ESC at anytime to exit the program." << endl << endl;

        while (!shouldExit)
        {
            SendUsermessage(server);
        }
    }
    else if (intInput == 2)
    {
        CreateClient();
        ConnectToServer();
        //TODO: fix issue where client immediatly sends message after connecting
            //something to do w/ the /n character after entering an option
        thread eventThread = thread(StartListeningForEvents, client);

        cout << "Client created. Press ESC at anytime to exit the program." << endl << endl;

        while (!shouldExit)
        {
            SendUsermessage(client);
        }
    }
    else
    {
        cout << "Invalid input" << endl;
    }

    cout << "Exiting..." << endl << endl;
    //TODO: join threads

    //Close host and client and exit program
    if (server != nullptr)
    {
        enet_host_destroy(client);
    }
    
    if (client != nullptr)
    {
        enet_host_destroy(server);
    }
    
    return EXIT_SUCCESS;
}

void CreateServer()
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 1234;
    server = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);

    if (server == nullptr)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }
}

void CreateClient()
{
    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    if (client == nullptr)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }
}

void ConnectToServer()
{
    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "localhost");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        fprintf(stderr,
            "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to localhost:1234 succeeded.");
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        puts("Connection to localhost:1234 failed.");
    }
}

void StartListeningForEvents(ENetHost* host)
{
    //TODO: listen for events on a seperate thread
    while (!shouldExit)
    {
        //This while loop listens to events from the client for the server to respond to
        ENetEvent event;
        while (enet_host_service(host, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                    event.peer->address.host,
                    event.peer->address.port);
                /* Store any relevant client information here. */
                event.peer->data = (void*)"Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                cout << event.packet->data << endl;
                  
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
        }
    }
}

void StopListeningForEvents(thread eventThread)
{
    //TODO: shut off listening to event thread
}

void SendUsermessage(ENetHost* host)
{
    string stringInput;

    getline(cin, stringInput);
    cout << endl;

    //TODO: check for escape key, if pressed, quit

    string message = username + ": " + stringInput;

    SendPacket(host, &(host->peers[0]), message);
}

void SendPacket(ENetHost* host, ENetPeer* peer, string message)
{
    ENetPacket* packet = enet_packet_create(message.c_str(),
        message.size() + 1,
        ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(peer, 0, packet);
    enet_host_flush(host);
}