// LuaScript.h

#ifndef LUASCRIPT_H
#define LUASCRIPT_H
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h> 
#include "LuaBridge.h"

class LuaScript {
public:
	LuaScript(std::string filename);
	LuaScript();
	~LuaScript();
	lua_State* getState() { return L; }
	bool load(const std::string& filename);
	void close();
	void reload();
	std::string getFilename() { return filename; }
	void printError(const std::string& variableName, const std::string& reason);
	std::vector<std::string> getTableKeys(const std::string& name);
	luabridge::LuaRef GetRef(std::string variablename);
	std::string serialize(std::string table);
	void saveConfig(std::vector<std::string> variables, std::string fname);

	inline void clean() {
		int n = lua_gettop(L);
		lua_pop(L, n);
	}

	template<typename T>
	T get(const std::string& variableName, T* defaultValue = nullptr) {
		if (!L) {
			printError(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		T result;
		if (lua_gettostack(variableName, defaultValue != nullptr)) { // variable succesfully on top of stack
			result = lua_get<T>(variableName);
		}
		else {
			if (defaultValue) {
				result = *defaultValue;
			}
			else {
				result = lua_getdefault<T>();
			}
		}

		clean();
		return result;
	}

	bool lua_gettostack(const std::string& variableName, bool hasDefaultValue = false);

	// Generic get
	template<typename T>
	T lua_get(const std::string& filename) {
		return 0;
	}

	template<typename T>
	T lua_getdefault() {
		return 0;
	}

	template<typename T>
	std::vector<T> getArray(const std::string& name) {
		std::vector<T> v;
		if (!lua_gettostack(name.c_str())) {
			// printError(name, "Array not found");
			clean();
			return std::vector<T>();
		}
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			v.push_back((T)lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		clean();
		return v;
	}

	bool CheckLoad();
	
private:
	lua_State* L;
	std::string filename;
	int level;
	bool _loadfailed = false;
};

// Specializations

template <>
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
	return lua_toboolean(L, -1) != 0;
}

template <>
inline float LuaScript::lua_get<float>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		printError(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		printError(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& variableName) {
	std::string s = "null";
	if (lua_isstring(L, -1)) {
		s = std::string(lua_tostring(L, -1));
	}
	else {
		printError(variableName, "Not a string");
	}
	return s;
}


// default return values specializations 
template<>
inline std::string LuaScript::lua_getdefault<std::string>() {
	return "null";
}
// get array specialization for std::string

template<>
inline std::vector<std::string> LuaScript::getArray(const std::string& name) {
	std::vector<std::string> v;
	if (!lua_gettostack(name.c_str())) {
		//printError(name, "Array not found");
		clean();
		return std::vector<std::string>();
	}
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		v.push_back(std::string(lua_tostring(L, -1)));
		lua_pop(L, 1);
	}
	clean();
	return v;
}

#endif