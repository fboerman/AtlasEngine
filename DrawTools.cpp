////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Frank Boerman
///////////////////////////////////////////////////////////
using namespace std;

#include "DrawTools.h"

//help functions

//copies the array of a1 into a2
void copy_array(const float* a1, float* a2)
{
	int size = sizeof *a1;

	for (int i=0; i < size; i++)
	{
		*(a2 + i) = *(a1 + i);
	}
}

// class definitions:

//item base class
item::item(std::list<item*>* BaseList)
{
	_BaseList = BaseList;
	_BaseList->push_back(this);
}

item::~item()
{
	_BaseList->remove(this);
}

void item::draw()
{

}

void item::print()
{

}

//pixel class
pixel::pixel(std::list<item*>* BaseList, const float p[2], const float color[3])
:item(BaseList)
{
	assert(p);   // if you crash here then you called this with no coordinates
	copy_array(p, _p);

	if(!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else {
		copy_array(color, _color);
	}
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void pixel::draw(void)
{
	glBegin(GL_POINTS);            // Start drawing mode

		// If a color was passed to the function, set it
		glColor3f(_color[0],_color[1], _color[2]);
		glVertex2fv(this->_p);        // Draw the pixel at (_p[0], _p[1])

	glEnd();
}
//---------------------------------------------------------------------------
//
// Print myself in ELIF format
//
void pixel::print(void) const
{
	printf(".pixel %.0f %.0f %.5f %.5f %.5f\n", _p[0], _p[1], _color[0],
															_color[1], _color[2]);
}
//---------------------------------------------------------------------------
//
// destructor of a pixel
//
pixel::~pixel(void)
{

}


//---------------------------------------------------------------------------
//line class
line::line(std::list<item*>* BaseList, const float p1[2], const float p2[2], const float color[3], const float lineWidth)
:item(BaseList)
{
	//check coordinates
	assert(p1);
	assert(p2);
	//copy it into the private members
	copy_array(p1, _p1);
	copy_array(p2, _p2);


	if (!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else { //else copy it over
		copy_array(color, _color);
	}

	_lineWidth = lineWidth;
}
//---------------------------------------------------------------------------
//
// destructor of a line
//
line::~line(void)
{
	
}
//---------------------------------------------------------------------------
//
// Print myself in EDIF++ format
//
void line::print(void) const
{
	cout << ".line " << _p1[0] << " " << _p1[1] << " " << _p2[0] << " " << _p2[1] << " " << _color[0] << " " << _color[1] << " " << _color[2] << " " << _lineWidth << endl;
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void line::draw(void)
{
	glLineWidth(_lineWidth);
	glBegin(GL_LINES);
		glColor3f(_color[0], _color[1], _color[2]);
		glVertex2fv(_p1);
		glVertex2fv(_p2);
	glEnd();
}

//text class
text::text(std::list<item*>* BaseList, const string str, const float color[3], int x, int y)
:item(BaseList)
{	
	if (str == "")
	{
		assert(0);
	}

	_str = str;

	_x = x;
	_y = y;

	if (!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
		
	}
	else { //else copy it over
		copy_array(color, _color);
	}
}

text::~text()
{

}

void text::print() const
{
	cout << ".text " << _x << " " << _y << " " << _color[0] << " " << _color[1] << " " << _color[2] << " \"" << _str << "\"" << endl;
}

void text::draw()
{
	glColor3f(_color[0], _color[1], _color[2]);
	glRasterPos2f(_x, _y);
	for (char& c : _str)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

//  square class.
// for function description see header file

square::square(std::list<item*>* BaseList, const float p1[2], const float p2[2], const float linecolor[3], const float fillcolor[3])
:item(BaseList)
{
	assert(p1);
	assert(p2);
	
	copy_array(p1, _p1);
	copy_array(p2, _p2);

	if (!linecolor)
	{
		_linecolor[0] = 0;
		_linecolor[1] = 0;
		_linecolor[2] = 0;
	}
	else
	{
		copy_array(linecolor, _linecolor);
	}

	if (!linecolor)
	{
		_linecolor[0] = 0;
		_linecolor[1] = 0;
		_linecolor[2] = 0;
	}
	else
	{
		copy_array(linecolor, _linecolor);
	}

	if (!fillcolor)
	{
		_fillcolor[0] = 0;
		_fillcolor[1] = 0;
		_fillcolor[2] = 0;
	}
	else
	{
		copy_array(fillcolor, _fillcolor);
	}

	_filled = 0;

}

square::~square()
{
	//remove_from_list();
}

void square::print() const
{
	cout << ".square " << _p1[0] << " " << _p1[1] << " " << _p2[0] << " " << _p2[1] << " " << _linecolor[0] << " " << _linecolor[1] << " " << _linecolor[2] << " " << _fillcolor[0] << " " << _fillcolor[1] << " " << _fillcolor[2] << endl;
}


void square::draw()
{
	float p3[2], p4[2];
	//////////
	//p4	p2
	//p1	p3
	//////////
	p4[0] = _p1[0];
	p4[1] = _p2[1];
	p3[0] = _p2[0];
	p3[1] = _p1[1];

	glLineWidth(1);

	if (_filled)
	{
		glBegin(GL_QUADS);
			glColor3fv(_fillcolor);
			glVertex2fv(_p1);
			glVertex2fv(p4);
			glVertex2fv(_p2);
			glVertex2fv(p3);
		glEnd();
		//check the whitelines
		glBegin(GL_LINES);
		float whiteline[3] = { 1 , 1 , 1 };
		//check all sides, if neighbor than draw whiteline, otherwise draw the linecolor line
		//up
		if (_neighbors.find("u") != string::npos)
		{
			glColor3fv(whiteline);
		}
		else
		{
			glColor3fv(_linecolor);
		}
		glVertex2fv(p4);
		glVertex2fv(_p2);

		//right
		if (_neighbors.find("r") != string::npos)
		{
			glColor3fv(whiteline);
		}
		else
		{
			glColor3fv(_linecolor);
		}
		glVertex2fv(p3);
		glVertex2fv(_p2);

		//down
		if (_neighbors.find("d") != string::npos)
		{
			glColor3fv(whiteline);
		}
		else
		{
			glColor3fv(_linecolor);
		}
		glVertex2fv(_p1);
		glVertex2fv(p3);

		//left
		if (_neighbors.find("l") != string::npos)
		{
			glColor3fv(whiteline);
		}
		else
		{
			glColor3fv(_linecolor);
		}
		glVertex2fv(_p1);
		glVertex2fv(p4);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
			glColor3fv(_linecolor);
			glVertex2fv(_p1);
			glVertex2fv(p3);
			glVertex2fv(p3);
			glVertex2fv(_p2);
			glVertex2fv(_p2);
			glVertex2fv(p4);
			glVertex2fv(p4);
			glVertex2fv(_p1);
		glEnd();
	}
}

void square::flip()
{
	_filled = !_filled;
	if (!_filled)
	{
		Clear_Neighbors();
	}
}

int square::get_status()
{
	return _filled;
}

void square::Set_Neighbor(char n)
{
	for (char& c : _neighbors)
	{
		if (c == n)
		{
			return;
		}
	}
	stringstream stream;
	stream << _neighbors << n;
	_neighbors = stream.str();
}

void square::Del_Neighbor(char n)
{
	stringstream stream;
	for (char& c : _neighbors)
	{
		if (c != n)
		{
			stream << c;
		}
	}
	_neighbors = stream.str();
}

void square::Clear_Neighbors()
{
	_neighbors = "";
}

void square::Set_Colour(const float colour[3])
{
	_fillcolor[0] = (float)colour[0] / (float)255;
	_fillcolor[1] = (float)colour[1] / (float)255;
	_fillcolor[2] = (float)colour[2] / (float)255;
}

circle::circle(std::list<item*>* BaseList, const float p[2], const float linecolor[3], const float radius, const int segments, const bool filled)
	:item(BaseList)
{
	assert(p);
	assert(linecolor);

	copy_array(p, _p);
	copy_array(linecolor, _linecolor);
	_radius = radius;
	_segments = segments;
	_filled = filled;
}

circle::~circle()
{

}

void circle::ChangePosition(const float newPosition[2])
{
	_p[0] = newPosition[0];
	_p[1] = newPosition[1];
}

void circle::ChangeRadius(const float newRadius)
{
	_radius = newRadius;
}

void circle::draw()
{
	glColor3fv(_linecolor);
	glLineWidth(1);
	if (_filled)
	{
		glBegin(GL_TRIANGLE_FAN);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
	}
	for (int i = 0; i < _segments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(_segments);//get the current angle

		float x = _radius * cosf(theta);//calculate the x component
		float y = _radius * sinf(theta);//calculate the y component

		glVertex2f(x + _p[0], y + _p[1]);//output vertex

	}
	glEnd();
}

void circle::print() const
{

}

//arrow class

arrow::arrow(std::list<item*>* BaseList, const float p[2], const float linecolor[3], const float size, const bool filled, const float angle)
	:item(BaseList)
{
	assert(p);
	assert(linecolor);

	copy_array(p, _p);
	copy_array(linecolor, _linecolor);
	_size = size;
	_filled = filled;
	_angle = angle;
}

arrow::~arrow()
{

}

void arrow::print() const
{

}

void arrow::ChangeAngle(const float newAngle)
{
	_angle = ToRadians(newAngle);
}

void arrow::ChangePosition(const float newPosition[2])
{
	_p[0] = newPosition[0];
	_p[1] = newPosition[1];
}

void arrow::draw()
{
	//	 p2
	//	 _p
	//p1	p3
	Point p1, p2, p3;
	p1.x = -10;
	p1.y = -10;
	p1.z = 0;

	p2.x = 0;
	p2.y = 20;
	p2.z = 0;

	p3.x = 10;
	p3.y = -10;
	p3.z = 0;

	//rotate z axis
	p1 = RotateVector(_angle, p1, 'z');
	p2 = RotateVector(_angle, p2, 'z');
	p3 = RotateVector(_angle, p3, 'z');

	glColor3fv(_linecolor);
	glLineWidth(1);
	if (_filled)
	{
		glBegin(GL_TRIANGLE_FAN);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
	}

	glVertex2f(_p[0], _p[1]);
	glVertex2f(_p[0] + p1.x, _p[1] + p1.y);
	glVertex2f(_p[0] + p2.x, _p[1] + p2.y);
	glVertex2f(_p[0] + p3.x, _p[1] + p3.y);

	glEnd();
}