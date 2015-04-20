//main header file
//copyright WillieWonka 2014

#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <ctime>
#include "AtlasEngine.h"
#include "DrawTools.h"
#include "DataStructures.h"

class Node;
class Agent;

//global variables
std::list<item*> DrawList = std::list<item*>();
AtlasWorld<Node, Agent>* MyWorld;

//classes

class Node
{
private:
	std::string _ID;
	circle* _image;
	int _value = 1;

public:
	Node(std::string id, circle* image);
	~Node();

	void AtlasTick(const int newPosition[2]);

	void AtlasArrivedAgent(std::string agentID);

	std::string ID();
	
};

class Agent
{
private:
	std::string _ID;
	arrow* _image;
	bool routedone = false;

public:
	Agent(std::string id, arrow* image);
	~Agent();

	void AtlasTick(const float newAngle, const int newPosition[2]);
	
	int AtlasArrived();

	std::string ID();
};

//functions

int main(int argc, char* argv[]);

void init();

//glut callbacks

void reshape(int w, int h);

void display(void);

void Time_Tick(int id);

void keypressed(unsigned char key, int x, int y);

#endif