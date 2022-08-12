#pragma once

#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <random>
#include <time.h> 

#include "IPacket.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

//#pragma optimize("", off)

using namespace std;

enum class HostConfiguration
{
    Server,
    Client,
    None
};

class NetworkHost
{
    NetworkHost()
        : m_pHost(nullptr),
        m_configuration(HostConfiguration::None)
    {

    }
    ~NetworkHost()
    {
        //TODO: destroy host when deinitialize
    }

    NetworkHost(const NetworkHost&) = delete;
    void operator=(const NetworkHost&) = delete;

    void CreateServer(string hostAddress, int hostPort = 1234)
    {
        ENetAddress tempAddress;

        if (hostAddress == "")
        {
            cout << "Using default host address." << endl;
            m_address.host = ENET_HOST_ANY;
            enet_address_set_host(&tempAddress, "localhost");
        }
        else
        {
            enet_address_set_host(&m_address, hostAddress.c_str());
            enet_address_set_host(&tempAddress, hostAddress.c_str());
        }
        m_address.port = hostPort;

        m_pHost = enet_host_create(&m_address /* the address to bind the server host to */,
            32      /* allow up to 32 clients and/or outgoing connections */,
            2      /* allow up to 2 channels to be used, 0 and 1 */,
            0      /* assume any amount of incoming bandwidth */,
            0      /* assume any amount of outgoing bandwidth */);

        if (m_pHost == nullptr)
        {
            fprintf(stderr,
                "An error occurred while trying to create an ENet server host.\n");
            exit(EXIT_FAILURE);
        }

        SetRandomNumber();

        char ipAddress[20];
        enet_address_get_host_ip(&tempAddress, ipAddress, 20);
        cout << "Successfully created server at address " << ipAddress << " port: " << m_address.port << endl << endl;
    }

    void CreateClient()
    {
        m_pHost = enet_host_create(NULL /* create a client host */,
            1 /* only allow 1 outgoing connection */,
            2 /* allow up 2 channels to be used, 0 and 1 */,
            0 /* assume any amount of incoming bandwidth */,
            0 /* assume any amount of outgoing bandwidth */);

        if (m_pHost == nullptr)
        {
            fprintf(stderr,
                "An error occurred while trying to create an ENet client host.\n");
            exit(EXIT_FAILURE);
        }

        cout << "Successfully created client." << endl;
    }

    void StartListeningForEvents()
    {
        m_shouldListenForEvents = true;
        m_eventThread = thread(&NetworkHost::ListenForEvents, this);
    }

    void ListenForEvents()
    {
        //TODO: make thread safe, especially when reading/writing to instance
        while (m_shouldListenForEvents)
        {
            ENetEvent event;
            while (enet_host_service(m_pHost, &event, 1000) > 0)
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
                    ProcessPacket(event);

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

    void ProcessPacket(ENetEvent& event)
    {
        cout << event.packet->data << endl;
        
        IPacket* pPacket = (IPacket*)(event.packet->data);
        PacketType packetType = pPacket->GetPacketType();

        BoolPacket responsePacket(m_username, false);
        int guess = 0;
        bool bCorrectGuess = false;

        switch (packetType)
        {
        case PacketType::String:
            cout << pPacket->m_username << ": ";
            cout << ((StringPacket*)pPacket)->m_string << endl;
            break;

        case PacketType::Int:
            cout << pPacket->m_username << ": ";
            guess = ((IntPacket*)pPacket)->m_int;
            cout << "Guessed " << guess << endl;

            if (CheckRandomNumber(guess))
            {
                cout << pPacket->m_username << " guessed correctly!" << endl;
                SetRandomNumber();
                responsePacket.m_bool = true;
            }

            SendPacket(responsePacket);

            break;

        case PacketType::Bool:
            bCorrectGuess = ((BoolPacket*)pPacket)->m_bool;

            if (bCorrectGuess)
            {
                cout << "You guessed correctly! Picking a new random number." << endl << endl;
            }

            cout << "Guess a number." << endl;

            break;

        default:
            cout << "Could not process packet, invalid packet type." << endl;
        }
    }

    void SetRandomNumber()
    {
        m_randNumber = rand() % 10 + 1;
    }

    bool CheckRandomNumber(int guess)
    {
        if (guess == m_randNumber)
        {
            return true;
        }

        return false;
    }

public:
    bool m_isInitialized;
    bool m_shouldListenForEvents;
    int m_randNumber;
	string m_username;
    thread m_eventThread;

	ENetAddress m_address;
	ENetHost* m_pHost;
    HostConfiguration m_configuration;

    static NetworkHost& GetInstance()
    {
        static NetworkHost instance;
        return instance;
    }

    static void Initialize(string username, HostConfiguration configuration, string hostAdress = "")
    {
        if (configuration == HostConfiguration::None)
        {
            cout << "Invalid configuration." << endl;
            return;
        }

        if (username == "")
        {
            cout << "Invalid username." << endl;
            return;
        }

        srand(time(NULL));
        NetworkHost* instance = &GetInstance();

        instance->m_username = username;
        instance->m_configuration = configuration;

        if (configuration == HostConfiguration::Server)
        {
            instance->CreateServer(hostAdress);
            instance->StartListeningForEvents();
        }
        else
        {
            instance->CreateClient();
        }

        instance->m_isInitialized = true;
    }

    static void ConnectToServer(string serverAdress, int serverPort = 1234)
    {
        NetworkHost* instance = &GetInstance();
        
        if (!instance->m_isInitialized)
        {
            cout << "Failed to connect to server. NetworkHost has not been initialized." << endl;
            return;
        }

        if (instance->m_configuration != HostConfiguration::Client)
        {
            cout << "Failed to connect to server. NetworkHost is not configured as a client." << endl;
            return;
        }

        ENetEvent event;
        ENetPeer* peer;

        /* Connect to some.server.net:1234. */
        if (serverAdress == "")
        {
            cout << "Using default server address." << endl;
            enet_address_set_host(&(instance->m_address), "localhost");
        }
        else
        {
            enet_address_set_host(&(instance->m_address), serverAdress.c_str());
            if (instance->m_address.host == 0)
            {
                cout << "Invalid address. Using default server address." << endl;
                enet_address_set_host(&(instance->m_address), "localhost");
            }
        }
        instance->m_address.port = serverPort;

        /* Initiate the connection, allocating the two channels 0 and 1. */
        peer = enet_host_connect(instance->m_pHost, &(instance->m_address), 2, 0);
        char ipAddress[20];
        enet_address_get_host_ip(&instance->m_address, ipAddress, 20);
        cout << "Attempting to connect to host at address " << ipAddress << "." << instance->m_address.port << endl;
        if (peer == NULL)
        {
            fprintf(stderr,
                "No available peers for initiating an ENet connection.\n");
            exit(EXIT_FAILURE);
        }

        /* Wait up to 5 seconds for the connection attempt to succeed. */
        if (enet_host_service(instance->m_pHost, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            puts("Connection succeeded.");
            instance->StartListeningForEvents();
        }
        else
        {
            /* Either the 5 seconds are up or a disconnect event was */
            /* received. Reset the peer in the event the 5 seconds   */
            /* had run out without any significant event.            */
            enet_peer_reset(peer);
            puts("Connection failed.");
        }
    }

    template<class T>
    static void SendPacket(T packet)
    {
        NetworkHost* instance = &GetInstance();

        if (!instance->m_isInitialized)
        {
            cout << "Failed to sent packet. NetworkHost has not been initialized." << endl;
            return;
        }

        ENetPacket* enetPacket = enet_packet_create(&packet, sizeof(packet), ENET_PACKET_FLAG_RELIABLE);
        ENetPeer*  peer = &(NetworkHost::GetInstance().m_pHost->peers[0]);

        enet_peer_send(peer, 0, enetPacket);
        enet_host_flush(instance->m_pHost);
    }

    static void BroadcastPacket(IPacket packet)
    {
        NetworkHost* instance = &GetInstance();

        if (!instance->m_isInitialized)
        {
            cout << "Failed to sent packet. NetworkHost has not been initialized." << endl;
            return;
        }
        //TODO: send packet to ALL peers w/ enet broadcast function
    }

    static void Deinitialize()
    {
        //TODO: end all threads and clear instance
    }
};
