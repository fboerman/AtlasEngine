//Atlas Engine header file
//copyright WillieWonka 2014

#ifndef ATLAS_H
#define ATLAS_H

#include <list>
#include <string>
#include <map>
#include<vector>
#include <sstream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include "LuaScript.h"

template<typename T1, typename T2> class AtlasWorld;
template<typename T1, typename T2> class AtlasNode;
template<typename T> class AtlasAgent;
struct ParentInfo;

//NOTES ON USING THE ATLAS ENGINE:
//requirements to payloads:
//both agents and nodes
//ID() function which returns string of ID. this is saved to the lua file
//AtlasTick() function that is called each time tick
//for agents:
//AtlasArrived() function that is called when agent is arrived at destination. returns new route number or 0 if it needs to be destroyed.
//
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
	friend AtlasNode<T1,T2>;

protected:
	//database with routes: <routenumber, <nodeid,parentinfo struct>>
	std::map<int, std::map<std::string, ParentInfo>> _Routes;

	//map of all nodes in this world, <id,AtlasNodePointer>
	std::map<std::string, AtlasNode<T1,T2>*> _Nodes;

private:
	//the lua script eninge to be used
	LuaScript* _Engine;

	//current time in ticks
	int _Time = 0;

public:
	//constructor and destructor
	AtlasWorld();
	AtlasWorld(std::string worldname);
	~AtlasWorld();

	//Advances the whole world with numsteps ticks
	void Tick(int numsteps);

	//Saves world map to file
	void SaveMap(std::string worldname);

	//Add a node to the world
	void AddNode(AtlasNode<T1, T2>* Node);

	//Define a new route in the world, if old number is used than route is overwritten
	//route is a list of stringarrays of nodes starting with first node and than working towards end
	//array is: [0] => ID [1]=> cost in reference to previous entry
	void AddRoute(int id, std::vector<std::vector<std::string>> route);

};


//class of an individual node in the AtlasWorld, a node has one parent per route. so in total it can have more parents. agents travel to one parent according to their route
//T1: type for payload node, T2 type for payload agent
template<typename T1, typename T2> class AtlasNode
{

	friend AtlasWorld<T1,T2> ;

protected:
	//list of agents who are travelling from this node to parent(s)
	std::list<AtlasAgent<T2>*> _Agents;

private:
	//own ID
	std::string _ID;
	//the payload of this node
	T1* _Payload = nullptr;
	//payload id
	std::string _PayloadID;

public:
	//constructor and deconstructor
	AtlasNode(std::string ID);
	AtlasNode(luabridge::LuaRef* luasave);
	~AtlasNode();

	//returns onw ID
	std::string ID();

	//advances the simulation of this node with one time tick
	void Tick(AtlasWorld<T1, T2>* ThisWorld);
	
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
	//friend AtlasNode<T>;

private:
	std::string _ID;
	int _RouteNumber;
	int _TravelTime;
	int _TotalTime;
	std::string _Destination;
	T* _Payload = nullptr;
	std::string _PayloadID;
	//if this is true than this timetick has already been processed this tick and should be ignored for rest of tick
	bool _processed = false;

public:
	AtlasAgent(std::string ID, int RouteNumber, std::string Destination);
	AtlasAgent(luabridge::LuaRef* luasave);
	~AtlasAgent();

	//returns onw ID
	std::string ID();

	//advances simulation with one timetick
	void Tick();

	//saves the node to lua format
	luabridge::LuaRef Save(lua_State* L);

	//returns the payloadid
	std::string GetPayloadID();

	//adds payload data
	void AddPayload(T* p);

	//returns the destination
	std::string Destination();

	//resets the traveltime for when a node in route has been reached
	void ReachedNode();

	//returns the route number
	int RouteNumber();

	//returns time travelled
	int TimeTravelled();

	//called when arrived at destination
	int DestinationArrived();

	//Set new route number
	void SetRoute(int newroute);

	//sets the processed to false
	void ResetProcessed();

	//returns the proccessed state
	bool IsProcessed();
};

//Error class that is used to throw and display errors
class AtlasError :
	public std::runtime_error
{
public:
	AtlasError(int type, std::string c_message);
	~AtlasError();

	//displays the error message
	std::string message();

private:
	//error type
	int _type;
	//custom message
	std::string _custom;
};

//-------------------------------------
//function implementations
//-------------------------------------

// AtlasWorldclass functions:

template<typename T1, typename T2> AtlasWorld<T1, T2>::AtlasWorld()
{
	//create new empty world
	_Routes = std::map<int, std::map<std::string, ParentInfo>>();
	_Nodes = std::map<std::string, AtlasNode<T1, T2>*>();
	_Engine = new LuaScript();
}

template<typename T1, typename T2> AtlasWorld<T1, T2>::AtlasWorld(std::string worldname)
{
	_Routes = std::map<int, std::map<std::string, ParentInfo>>();
	_Nodes = std::map<std::string, AtlasNode<T1, T2>*>();

	//load the lua file and check if its loaded correctly
	_Engine = new LuaScript(worldname);
	if (_Engine->CheckLoad())
	{
		throw AtlasError(0, worldname);
	}

	luabridge::LuaRef data = _Engine->GetRef("Save");
	if (data.isNil())
	{
		throw AtlasError(1, "Save in savefile");
	}

	luabridge::LuaRef Nodes = data["Nodes"];
	if (Nodes.isNil())
	{
		throw AtlasError(1, "Nodes in savefile");
	}
	for (int i = 1; i < Nodes.length() + 1; i++)
	{
		luabridge::LuaRef NodeLuaObject = Nodes[i];
		AtlasNode<T1, T2>* NewNode;
		try
		{
			NewNode = new AtlasNode<T1, T2>(&NodeLuaObject);
		}
		catch (luabridge::LuaRefException &e1)
		{
			try
			{
				std::string ID = NodeLuaObject["ID"];
				std::stringstream ss;
				ss <<"At Node object " << ID << "'" << e1.message() << "'";
				throw AtlasError(2, ss.str());
			}
			catch (luabridge::LuaRefException &e2)
			{
				throw AtlasError(2, e2.message());
			}
		}
		_Nodes[NewNode->ID()] = NewNode;
	}

	luabridge::LuaRef Routes = data["Routes"];
	if (Routes.isNil())
	{
		throw AtlasError(1, "Routes in savefile");
	}

	for (int i = 1; i < Routes.length() + 1; i++)
	{
		luabridge::LuaRef RouteLua = Routes[i];
		luabridge::LuaRef Members = RouteLua["members"];
		if (Members.isNil())
		{
			throw new AtlasError(1, "members in a route");
		}
		std::map<std::string, ParentInfo>* Route = new std::map<std::string, ParentInfo>();
		for (int j = 1; j < Members.length() + 1; j++)
		{
			luabridge::LuaRef member = Members[j];

			ParentInfo PInfo;
			std::string ID = member["ID"].cast<std::string>();
			std::string ParentID = member["ParentID"].cast<std::string>();
			int travelcost = member["travelcost"].cast<int>();

			PInfo.ParentID = ParentID;
			PInfo.travelcost = travelcost;
			(*Route)[ID] = PInfo;

		}

		int number = RouteLua["number"].cast<int>();
		_Routes[number] = *Route;
	}

}

template<typename T1, typename T2> AtlasWorld<T1, T2>::~AtlasWorld()
{
	for (std::map<std::string, AtlasNode<T1, T2>*>::iterator it = _Nodes.begin(); it != _Nodes.end(); it++)
	{
		delete it->second;
	}
}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::Tick(int numsteps)
{
	for (int i = 0; i < numsteps; i++)
	{
		//reset all processsed flags
		for (std::map<std::string, AtlasNode<T1, T2>*>::iterator it = _Nodes.begin(); it != _Nodes.end(); it++)
		{
			for (auto it2 = it->second->_Agents.begin(); it2 != it->second->_Agents.end(); it2++)
			{
				(*it2)->ResetProcessed();
			}
		}
		//increment and report the total time passed
		_Time++;
		std::cout << "Tick: " << _Time << std::endl;
		//iterate through all nodes and invoke the tick on them
		for (std::map<std::string, AtlasNode<T1, T2>*>::iterator it = _Nodes.begin(); it != _Nodes.end(); it++)
		{
			it->second->Tick(this);
		}
		
	}
}


template<typename T1, typename T2> void AtlasWorld<T1, T2>::SaveMap(std::string worldname)
{
	//save the world object to lua file

	lua_State* L = _Engine->getState();

	luabridge::LuaRef Save = luabridge::newTable(L);

	luabridge::LuaRef Nodes = luabridge::newTable(L);
	int i = 1;
	for (std::map<std::string, AtlasNode<T1, T2>*>::iterator it = _Nodes.begin(); it != _Nodes.end(); it++)
	{
		luabridge::LuaRef nodeLuaObject = it->second->Save(L);

		Nodes[i] = nodeLuaObject;

		i++;
	}

	Save["Nodes"] = Nodes;

	luabridge::LuaRef Routes = luabridge::newTable(L);
	i = 1;
	for (std::map<int, std::map<std::string, ParentInfo>>::iterator it = _Routes.begin(); it != _Routes.end(); it++)
	{
		luabridge::LuaRef Route = luabridge::newTable(L);
		Route["number"] = it->first;

		luabridge::LuaRef members = luabridge::newTable(L);
		int j = 1;
		for (std::map<std::string, ParentInfo>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			luabridge::LuaRef member = luabridge::newTable(L);
			member["ID"] = it2->first;
			member["ParentID"] = it2->second.ParentID;
			member["travelcost"] = it2->second.travelcost;
			members[j] = member;
			j++;
		}

		Route["members"] = members;

		Routes[i] = Route;
		i++;
	}

	Save["Routes"] = Routes;

	//push the array on the stack, name it and serialize to file
	luabridge::push(L, Save);
	lua_setglobal(L, "Save");
	std::ofstream stream = std::ofstream(worldname);
	stream << _Engine->serialize("Save");
	stream.close();
}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::AddNode(AtlasNode<T1, T2>* Node)
{
	_Nodes[Node->ID()] = Node;
}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::AddRoute(int id, std::vector<std::vector<std::string>> route)
{
	std::map<std::string, ParentInfo>* Route = new std::map<std::string, ParentInfo>();

	std::vector<std::string> previousnode = { "", "" };
	for (std::vector<std::vector<std::string>>::iterator it = route.begin(); it != route.end(); it++)
	{
		if (previousnode[0] == "")
		{
			previousnode[0] = (*it)[0];
			previousnode[1] = (*it)[1];
			continue;
		}
		ParentInfo PI;
		PI.ParentID = (*it)[0];
		PI.travelcost = std::stoi(previousnode[1]);
		(*Route)[previousnode[0]] = PI;

		previousnode[0] = (*it)[0];
		previousnode[1] = (*it)[1];
	}

	_Routes[id] = *Route;
}


// AtlasNodeclass functions:

template<typename T1, typename T2> AtlasNode<T1, T2>::AtlasNode(std::string ID)
{
	//create fresh new node
	_ID = ID;
	_Agents = std::list<AtlasAgent<T2>*>();
	_Payload = nullptr;
	_PayloadID = "";
}

template<typename T1, typename T2> AtlasNode<T1, T2>::AtlasNode(luabridge::LuaRef* luasave)
{
	//create Node from lua object
	luabridge::LuaRef data = *luasave;

	_ID = data["ID"].cast<std::string>();
	_Agents = std::list<AtlasAgent<T2>*>();
	luabridge::LuaRef Agents = data["Agents"];
	for (int i = 1; i < Agents.length()+1; i++)
	{
		luabridge::LuaRef AgentLuaObject = Agents[i];
		AtlasAgent<T2>* NewAgent = new AtlasAgent<T2>(&AgentLuaObject);
		_Agents.push_back(NewAgent);
	}
	_PayloadID = data["PayloadID"].cast<std::string>();
}

template<typename T1, typename T2> AtlasNode<T1, T2>::~AtlasNode()
{
	for (std::list<AtlasAgent<T2>*>::iterator it = _Agents.begin(); it != _Agents.end(); it++)
	{
		delete (*it);
	}
}

template<typename T1, typename T2> void AtlasNode<T1, T2>::Tick(AtlasWorld<T1, T2>* ThisWorld)
{
	//call the tick hook in the payload
	_Payload->AtlasTick();
	//iterate through the agents list
	std::list<AtlasAgent<T2>*>::iterator it = _Agents.begin();
	while (it != _Agents.end())
	{
		//check if this agent has already been processed this tick
		if ((*it)->IsProcessed())
		{
			it++;
			continue;
		}
		//call the tick hook in the agent payload
		(*it)->Tick();
		//retrieve the current route this agent is travelling on
		auto Route = ThisWorld->_Routes[(*it)->RouteNumber()];
		//if this node has no parent in the route than the route is invalid and throw error
		if (Route.find(_ID) == Route.end())
		{
			throw new AtlasError(3, std::to_string((*it)->RouteNumber()));
		}
		else
		{
			//get the parentinfo of current node
			ParentInfo PI = Route[_ID];
			//check if full distance has been travelled to parent node
			if (PI.travelcost == (*it)->TimeTravelled())
			{
				//reset the agent for reaching a node
				(*it)->ReachedNode();
				//check if final destination is reached
				if (PI.ParentID == (*it)->Destination())
				{
					//ask for new route by calling atlas hook AtlasArrived
					int newRoute = (*it)->DestinationArrived();
					//if there is no new route destroy the agent
					if (newRoute == 0)
					{
						delete (*it);
					}
					else
					{
						//set the new route
						(*it)->SetRoute(newRoute);
						//retrieve this new route
						Route = ThisWorld->_Routes[newRoute];
						//check if route is valid for current node
						if (Route.find(_ID) == Route.end())
						{
							throw new AtlasError(3, std::to_string(newRoute));
						}
						else
						{
							//retrieve parentinfo and move the agent to its new node
							PI = Route[_ID];
							AtlasNode<T1, T2>* newowner = ThisWorld->_Nodes[PI.ParentID];
							newowner->AttachAgent(*it);
						}
						PI = Route[_ID];

					}
					//remove current agent from this node
					_Agents.erase(it++);
				}
				else
				{
					//retrieve parentinfo and move the agent to its new node
					AtlasNode<T1, T2>* newowner = ThisWorld->_Nodes[PI.ParentID];
					newowner->AttachAgent(*it);
					_Agents.erase(it++);
				}
			}
			else
			{
				//move the iterator forward
				it++;
			}
		}
	}
}


template<typename T1, typename T2> luabridge::LuaRef AtlasNode<T1, T2>::Save(lua_State* L)
{
	//save node to lua object
	luabridge::LuaRef t = luabridge::newTable(L);

	t["ID"] = _ID;
	t["Agents"] = luabridge::newTable(L);
	luabridge::LuaRef Agents = t["Agents"];
	int i = 1;
	//iterate through the agents and call the save on them to produce a lua object which is saved in the agent list
	for (std::list<AtlasAgent<T2>*>::iterator it = _Agents.begin(); it != _Agents.end(); it++)
	{
		Agents[i] = (*it)->Save(L);
		i++;
	}
	if (_Payload == nullptr)
	{
		t["PayloadID"] = _PayloadID;
	}
	else
	{
		t["PayloadID"] = _Payload->ID();
	}

	return t;
}

template<typename T1, typename T2> void AtlasNode<T1, T2>::AttachAgent(AtlasAgent<T2>* agent)
{
	//check if agent is unique
	for (std::list<AtlasAgent<T2>*>::iterator it = _Agents.begin(); it != _Agents.end(); it++)
	{
		if ((*it)->ID() == agent->ID())
		{
			//new agent is already in list
			return;
		}
	}
	//agent is not in list so add it
	_Agents.push_back(agent);
}

template<typename T1, typename T2> std::string AtlasNode<T1, T2>::GetPayloadID()
{
	//return the payload id
	return _PayloadID;
}

template<typename T1, typename T2> void AtlasNode<T1, T2>::AddPayload(T1* p)
{
	//attach the payload pointer
	_Payload = p;
}

template<typename T1, typename T2> std::string AtlasNode<T1, T2>::ID()
{
	return _ID;
}


// AtlasAgentclass functions:

template<typename T> AtlasAgent<T>::AtlasAgent(std::string ID, int RouteNumber, std::string Destination)
{
	//create new fresh agent
	_ID = ID;
	_RouteNumber = RouteNumber;
	_Destination = Destination;
	_TotalTime = 0;
	_TravelTime = 0;
	_Payload = nullptr;
}

template<typename T> AtlasAgent<T>::AtlasAgent(luabridge::LuaRef* luasave)
{
	//load agent data from lua data object
	luabridge::LuaRef data = *luasave;

	_ID = data["ID"].cast<std::string>();
	_RouteNumber = data["RouteNumber"].cast<int>();
	_TotalTime = data["TotalTime"].cast<int>();
	_TravelTime = data["TravelTime"].cast<int>();
	_Destination = data["Destination"].cast<std::string>();
	_PayloadID = data["PayloadID"].cast<std::string>();
}

template<typename T> AtlasAgent<T>::~AtlasAgent()
{

}

template<typename T> void AtlasAgent<T>::Tick()
{
	//increment the time and call the custom function on the tick
	_TotalTime++;
	_TravelTime++;
	_Payload->AtlasTick();
	_processed = true;
}

template<typename T> luabridge::LuaRef AtlasAgent<T>::Save(lua_State* L)
{
	//save the current data object to a lua object
	luabridge::LuaRef t = luabridge::newTable(L);

	t["ID"] = _ID;
	t["TotalTime"] = _TotalTime;
	t["TravelTime"] = _TravelTime;
	t["Destination"] = _Destination;
	if (_Payload == nullptr)
	{
		t["PayloadID"] = _PayloadID;
	}
	else
	{
		t["PayloadID"] = _Payload->ID();
	}
	t["RouteNumber"] = _RouteNumber;

	return t;
}

template<typename T> std::string AtlasAgent<T>::GetPayloadID()
{
	//return the payloadid to enable outside engine to attach correct payload
	return _PayloadID;
}

template<typename T> void AtlasAgent<T>::AddPayload(T* p)
{
	//attach the payload pointer
	_Payload = p;
}

template<typename T> std::string AtlasAgent<T>::ID()
{
	return _ID;
}

template<typename T> std::string AtlasAgent<T>::Destination()
{
	return _Destination;
}

template<typename T> void AtlasAgent<T>::ReachedNode()
{
	_TravelTime = 0;
}

template<typename T> int AtlasAgent<T>::RouteNumber()
{
	return _RouteNumber;
}

template<typename T> int AtlasAgent<T>::TimeTravelled()
{
	return _TravelTime;
}

template<typename T> int AtlasAgent<T>::DestinationArrived()
{
	return _Payload->AtlasArrived();
}

template<typename T> void AtlasAgent<T>::SetRoute(int newroute)
{
	_RouteNumber = newroute;
}

template<typename T> void AtlasAgent<T>::ResetProcessed()
{
	_processed = false;
}

template<typename T> bool AtlasAgent<T>::IsProcessed()
{
	return _processed;
}

// AtlasErrorclass functions:

AtlasError::AtlasError(int type, std::string c_message)
	:runtime_error("AtlasEngine Error")
{
	_type = type;
	_custom = c_message;
}

AtlasError::~AtlasError()
{

}

std::string AtlasError::message()
{
	std::stringstream ss;
	switch (_type)
	{
	case 0://file error
		ss << "Could not parse lua file: " << _custom;
		break;
	case 1://key error when parsing
		ss << "Missing key when parsing: '" << _custom << "'";
		break;
	case 2://parsing error with luaref
		ss << "Parsing error because of luaref exception: " << _custom;
		break;
	case 3://invalid route
		ss << "Invalid route with number: " << _custom;
		break;
	}

	return ss.str();
}

#endif