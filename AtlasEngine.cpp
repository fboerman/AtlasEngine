//Atlas Engine cpp file
//copyright WillieWonka 2014

#include "AtlasEngine.h"

// AtlasWorldclass functions:

template<typename T1, typename T2> AtlasWorld<T1, T2>::AtlasWorld()
{

}

template<typename T1, typename T2> AtlasWorld<T1, T2>::AtlasWorld(std::string worldname)
{

}

template<typename T1, typename T2> AtlasWorld<T1, T2>::~AtlasWorld()
{

}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::Tick()
{

}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::LoadMap(std::string worldname)
{

}

template<typename T1, typename T2> void AtlasWorld<T1, T2>::SaveMap(std::string worldname)
{

}


// AtlasNodeclass functions:

template<typename T1, typename T2> AtlasNode<T1, T2>::AtlasNode(std::string ID)
{

}

template<typename T1, typename T2> AtlasNode<T1, T2>::AtlasNode(luabridge::LuaRef luasave)
{

}

template<typename T1, typename T2> AtlasNode<T1, T2>::~AtlasNode()
{

}

template<typename T1, typename T2> void AtlasNode<T1, T2>::Tick(AtlasWorld<T1, T2>* ThisWorld)
{

}

template<typename T1, typename T2> void AtlasNode<T1, T2>::Move(AtlasAgent<T2>* agent, std::string NewParent)
{

}

template<typename T1, typename T2> luabridge::LuaRef AtlasNode<T1, T2>::Save(lua_State* L)
{

}

template<typename T1, typename T2> void AtlasNode<T1, T2>::AttachAgent(AtlasAgent<T2>* agent)
{

}

template<typename T1, typename T2> std::string AtlasNode<T1, T2>::GetPayloadID()
{

}

template<typename T1, typename T2> void AtlasNode<T1, T2>::AddPayload(T1* p)
{

}


// AtlasAgentclass functions:

template<typename T> AtlasAgent<T>::AtlasAgent(std::string ID)
{
	
}

template<typename T> AtlasAgent<T>::AtlasAgent(luabridge::LuaRef luasave)
{

}

template<typename T> AtlasAgent<T>::~AtlasAgent()
{

}

template<typename T> void AtlasAgent<T>::Tick()
{

}

template<typename T> luabridge::LuaRef AtlasAgent<T>::Save(lua_State* L)
{

}

template<typename T> std::string AtlasAgent<T>::GetPayloadID()
{

}

template<typename T> void AtlasAgent<T>::AddPayload(T* p)
{

}

