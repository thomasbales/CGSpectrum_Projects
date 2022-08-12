#pragma once

using namespace std;

enum class PacketType
{
	Invalid,
	String,
	Int,
	Bool
};

struct IPacket 
{
protected:
	PacketType _packetType = PacketType::Invalid;

	IPacket(string username)
		:m_username(username)
	{

	}

public:
	PacketType GetPacketType()
	{
		return _packetType;
	}

	string m_username;
};

struct StringPacket : public IPacket
{
	StringPacket(string username, string _string)
		:IPacket(username),
		m_string(_string)
	{
		_packetType = PacketType::String;
	}

	string m_string = "";
};

struct IntPacket : public IPacket
{
	IntPacket(string username, int _int)
		:IPacket(username),
		m_int(_int)
	{
		_packetType = PacketType::Int;
	}

	int m_int = 0;
};

struct BoolPacket : public IPacket
{
	BoolPacket(string username, bool _bool)
		:IPacket(username),
		m_bool(_bool)
	{
		_packetType = PacketType::Bool;
	}

	bool m_bool;
};
