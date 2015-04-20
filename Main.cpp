//main cpp file
//copyright WillieWonka 2014

#include "Main.h"

Node::Node(std::string id, circle* image)
{
	_ID = id;
	_image = image;
}

Node::~Node()
{

}

void Node::AtlasTick(const int newPosition[2])
{
	_image->ChangePosition(newPosition);
}

std::string Node::ID()
{
	return _ID;
}

void Node::AtlasArrivedAgent(std::string agentID)
{
	_value++;
	_image->ChangeRadius(15 + _value * 5);
}

Agent::Agent(std::string id, arrow* image)
{
	_ID = id;
	_image = image;
}

Agent::~Agent()
{

}

void Agent::AtlasTick(const float newAngle, const int newPosition[2])
{
	_image->ChangeAngle(newAngle);
	_image->ChangePosition(newPosition);
}

int Agent::AtlasArrived()
{
	std::cout << "Agent " << _ID << " arrived at destination" << std::endl;
	if (!routedone)
	{
		routedone = true;
		return 3;
	}
	else
	{
		return 0;
	}
}

std::string Agent::ID()
{
	return _ID;
}

void keypressed(unsigned char key, int x, int y)
{
	if (key == 't')
	{
		try
		{
			MyWorld->Tick(1);
		}
		catch (AtlasError &e)
		{
			std::cout << e.ErrorMessage() << std::endl;
		}
		glutPostRedisplay();
	}
	
}

void init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// Double buffering, RGB format
	glutInitWindowSize(1024,500);				
	glutInitWindowPosition(0, 0);					
	glutCreateWindow("Atlas Engine Demo");		
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1024, 0.0, 500);
	glMatrixMode(GL_MODELVIEW);

	//enable alliasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//below only for 2d
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypressed);

}

void reshape(int w, int h)
{
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (std::list<item*>::iterator it = DrawList.begin(); it != DrawList.end(); it++)
	{
		(*it)->draw();
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	MyWorld = new AtlasWorld<Node,Agent>();
	AtlasWorld<Municipal, TransportAgent>* MyWorld2 = new AtlasWorld<Municipal, TransportAgent>();

	float color[3] = {0, 0, 0};
	int p1[2] = { 100, 100 };
	int p2[2] = { 200, 200 };
	int p3[2] = { 200, 100 };

	Node* N1 = new Node("Node1", new circle(&DrawList, p1, color, 15, 1000, false));
	Node* N2 = new Node("Node2", new circle(&DrawList, p2, color, 15, 1000, false));
	Node* N3 = new Node("Node3", new circle(&DrawList, p3, color, 15, 1000, false));

	std::string ANa1 = "ANode1";
	std::string ANa2 = "ANode2";
	std::string ANa3 = "ANode3";

	AtlasNode<Node, Agent>* AN1 = new AtlasNode<Node, Agent>(ANa1,p1);
	AtlasNode<Node, Agent>* AN2 = new AtlasNode<Node, Agent>(ANa2,p2);
	AtlasNode<Node, Agent>* AN3 = new AtlasNode<Node, Agent>(ANa3,p3);

	AN1->AddPayload(N1);
	AN2->AddPayload(N2);
	AN3->AddPayload(N3);

	Agent* A1 = new Agent("Agent1", new arrow(&DrawList,p1,color,10,false,0));
	Agent* A2 = new Agent("Agent2", new arrow(&DrawList, p1, color, 10, false, 0));

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
	std::vector<std::vector<std::string>> r3 = std::vector<std::vector<std::string>>();

	std::vector<std::string> itm = { "ANode1", "5" };
	r1.push_back(itm);
	itm[0] = "ANode2";
	itm[1] = "5";
	r1.push_back(itm);
	itm[0] = "ANode3";
	itm[1] = "5";
	r1.push_back(itm);

	itm[0] = "ANode1";
	itm[1] = "5";
	r2.push_back(itm);
	itm[0] = "ANode3";
	itm[1] = "5";
	r2.push_back(itm);
	itm[0] = "ANode2";
	itm[1] = "5";
	r2.push_back(itm);

	itm[0] = "ANode3";
	itm[1] = "5";
	r3.push_back(itm);
	itm[0] = "ANode2";
	itm[1] = "5";
	r3.push_back(itm);
	itm[0] = "ANode1";
	itm[1] = "5";
	r3.push_back(itm);

	MyWorld->AddRoute(1, r1);
	MyWorld->AddRoute(2, r2);
	MyWorld->AddRoute(3, r3);
	
	glutInit(&argc, argv);
	init();
	glutMainLoop();

	return 0;
}