//Atlas Engine header file
//copyright WillieWonka 2014

#ifndef ATLAS_H
#define ATLAS_H

#include <list>
#include <string>
#include <map>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"

template<typename T1, typename T2> class AtlasWorld;
template<typename T1, typename T2> class AtlasNode;
template<typename T> class AtlasAgent;
struct ParentInfo;

//NOTES ON USING THE ATLAS ENGINE:
//requirements to payloads:
//both agents and nodes
//ID() function which returns string of ID. this is saved to the lua file
//Tick() function that is called each time tick
//for agents:
//AtlasArrived() function that is called when agent is arrived at destination. returns new route number or 0 if it needs to be destroyed.
//payloads have to be added from the outside because the engine cant create the payload data elements
//the ID for the payload will be added to the agent or node and can be retrieved by calling GetPayloadID, than use AddPayload to add the payload from outside the engine

//struct to hold parent information of node
struct ParentInfo
{
	std::string ParentID;
	int travelcost;
};

//class that holds the whole world together
//T1: payload type for node, T2: payload type for agent
template<typename T1, typename T2> class AtlasWorld
{
	friend AtlasNode;

protected:
	//database with routes: <routenumber, <nodeid,parentinfo struct>>
	std::map<int, std::map<std::string, ParentInfo>> _Routes;

private:
	//map of all nodes in this world, <id,AtlasNodePointer>
	std::map<std::string, AtlasNode<T1,T2>*> _Nodes;

public:
	//constructor and destructor
	AtlasWorld();
	AtlasWorld(std::string worldname);
	~AtlasWorld();

	//Advances the whole world with one time tick
	void Tick();

	//Loads world map from file
	void LoadMap(std::string worldname);

	//Saves world map to file
	void SaveMap(std::string worldname);

};


//class of an individual node in the AtlasWorld, a node has one parent per route. so in total it can have more parents. agents travel to one parent according to their route
//T1: type for payload node, T2 type for payload agent
template<typename T1, typename T2> class AtlasNode
{
private:
	//own ID
	std::string _ID;
	//list of agents who are travelling from this node to parent(s)
	std::list<AtlasAgent<T2>*> _Agents;
	//the payload of this node
	T1* _Payload;
	//payload id
	std::string _PayloadID;

public:
	//constructor and deconstructor
	AtlasNode(std::string ID);
	AtlasNode(luabridge::LuaRef luasave);
	~AtlasNode();

	//advances the simulation of this node with one time tick
	void Tick(AtlasWorld<T1, T2>* ThisWorld);

	//moves an agent to another node
	void Move(AtlasAgent<T2>* agent, std::string NewParent);
	
	//saves the node to lua format
	luabridge::LuaRef Save(lua_State* L);

	//attaches agent to this node
	void AttachAgent(AtlasAgent<T2>* agent);

	//returns the payloadid
	std::string GetPayloadID();

	//adds payload data
	void AddPayload(T1* p);

};

//class of an agent traveling through the world
//the template is for the type of the payload to append your simulation data to it
template<typename T> class AtlasAgent
{
private:
	std::string _ID;
	int _RouteNumber;
	int _TravelTime;
	int _TotalTime;
	std::string _Destination;
	T* _Payload;
	std::string _PayloadID;

public:
	AtlasAgent(std::string ID);
	AtlasAgent(luabridge::LuaRef luasave);
	~AtlasAgent();

	//advances simulation with one timetick
	void Tick();

	//saves the node to lua format
	luabridge::LuaRef Save(lua_State* L);

	//returns the payloadid
	std::string GetPayloadID();

	//adds payload data
	void AddPayload(T* p);

};

#endif