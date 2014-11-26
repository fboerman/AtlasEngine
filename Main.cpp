//main cpp file
//copyright Frank Boerman 2014

#include "Main.h"

Node::Node(std::string id)
{
	_ID = id;
}

Node::~Node()
{

}

void Node::AtlasTick()
{
	//std::cout << "Node " << _ID << " Tick" << std::endl;
}

std::string Node::ID()
{
	return _ID;
}

Agent::Agent(std::string id)
{
	_ID = id;
}

Agent::~Agent()
{

}

void Agent::AtlasTick()
{
	//std::cout << "Agent " << _ID << " Tick" << std::endl;
}

int Agent::AtlasArrived()
{
	std::cout << "Agent " << _ID << " arrived at destination" << std::endl;
	return 0;
}

std::string Agent::ID()
{
	return _ID;
}


int main()
{
	AtlasWorld<Node,Agent>* MyWorld = new AtlasWorld<Node,Agent>();
	
	Node* N1 = new Node("Node1");
	Node* N2 = new Node("Node2");
	Node* N3 = new Node("Node3");

	AtlasNode<Node, Agent>* AN1 = new AtlasNode<Node, Agent>("ANode1");
	AtlasNode<Node, Agent>* AN2 = new AtlasNode<Node, Agent>("ANode2");
	AtlasNode<Node, Agent>* AN3 = new AtlasNode<Node, Agent>("ANode3");

	AN1->AddPayload(N1);
	AN2->AddPayload(N2);
	AN3->AddPayload(N3);

	Agent* A1 = new Agent("Agent1");
	Agent* A2 = new Agent("Agent2");

	AtlasAgent<Agent>* AA1 = new AtlasAgent<Agent>("AAgent1",1,"ANode3");
	AtlasAgent<Agent>* AA2 = new AtlasAgent<Agent>("AAgent2", 2, "ANode2");

	AA1->AddPayload(A1);
	AA2->AddPayload(A2);

	AN1->AttachAgent(AA1);
	AN1->AttachAgent(AA2);

	MyWorld->AddNode(AN1);
	MyWorld->AddNode(AN2);
	MyWorld->AddNode(AN3);
	
	std::vector<std::vector<std::string>> r1 = std::vector<std::vector<std::string>>();
	std::vector<std::vector<std::string>> r2 = std::vector<std::vector<std::string>>();

	std::vector<std::string> itm = { "ANode1", "5" };
	r1.push_back(itm);
	itm[0] = "ANode2";
	itm[1] = "2";
	r1.push_back(itm);
	itm[0] = "ANode3";
	itm[1] = "5";
	r1.push_back(itm);

	itm[0] = "ANode1";
	itm[1] = "5";
	r2.push_back(itm);
	itm[0] = "ANode3";
	itm[1] = "10";
	r2.push_back(itm);
	itm[0] = "ANode2";
	itm[1] = "14";
	r2.push_back(itm);

	MyWorld->AddRoute(1, r1);
	MyWorld->AddRoute(2, r2);

	MyWorld->Tick(5);
	MyWorld->SaveMap("Dump1.lua");
	MyWorld->Tick(5);
	MyWorld->SaveMap("Dump2.lua");
	MyWorld->Tick(5);
	MyWorld->SaveMap("Dump3.lua");
	MyWorld->Tick(5);
	MyWorld->SaveMap("Dump4.lua");

	delete MyWorld;
	//AtlasWorld<Node, Agent>* MyWorld = new AtlasWorld<Node,Agent>("TestWorld1.lua");

	return 0;
}