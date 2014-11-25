//main header file
//copyright Frank Boerman 2014

#ifndef MAIN_H
#define MAIN_H

#include<string>
#include<list>
#include<map>
#include<iostream>
#include<ctime>
#include "AtlasEngine.h"

class Node;
class Agent;

class Node
{
private:
	std::string _ID;

public:
	Node(std::string id);
	~Node();

	void AtlasTick();

	std::string ID();
	
};

class Agent
{
private:
	std::string _ID;

public:
	Agent(std::string id);
	~Agent();

	void AtlasTick();
	
	int AtlasArrived();

	std::string ID();
};

int main();

#endif