////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Frank Boerman
///////////////////////////////////////////////////////////

#ifndef DRAWTOOLS_H // this avoids multiple inclusion of this file
#define DRAWTOOLS_H

#include <list>
#include <GL\freeglut\freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include "LinearAlgebra.h"

class line;
class pixel;
class text;
class square;
class circle;
class arrow;

//help functions

//copies the array of a1 into a2
void copy_array(const float* a1, float* a2);

// class declarations

//base class for list purpose

class item
{
public :
	item(std::list<item*>* BaseList);
	~item(void);

	virtual void draw(void);
	virtual void print(void);

private:
	std::list<item*>* _BaseList;
};

//pixel class
class pixel :
             public item 
{
public:
	pixel(std::list<item*>* BaseList, const float p[2], const float color[3]);
	~pixel(void);

	void print() const;
	void draw(void);


private:
	float _p[2];  
	float _color[3];
};

//line class
class line :
             public item
{
public:
	line(std::list<item*>* BaseList, const float p1[2], const float p2[2], const float color[3], const float lineWidth);
	~line(void);

	void print() const;
	void draw(void);


private:
	float _p1[2];
	float _p2[2];
	float _color[3];
	float _lineWidth;
};

//text class
class text :
			public item    
{
public:
	text(std::list<item*>* BaseList, const std::string str, const float color[3], int x, int y);
	~text(void);

	void print() const;  
	void draw(void);

private:
	std::string _str;
	float _color[3];
	int _x;
	int _y;

};


//  square class
class square :
	public item
{
public:
	square(std::list<item*>* BaseList, const float p1[2], const float p2[2], const float linecolor[3], const float fillcolor[3]);
	~square(void);

	void print() const;
	void draw(void);
	void flip(void);
	//returns the filling status of the cell
	int get_status(void);
	//sets deletes, resets the neighbors
	void Set_Neighbor(char n);
	void Del_Neighbor(char n);
	void Clear_Neighbors(void);
	//sets the filling color
	void Set_Colour(const float colour[3]);

private:
	//actual drawing position
	float _p1[2];
	float _p2[2];
	//line and filling colours
	float _linecolor[3];
	float _fillcolor[3];
	int _filled; //filled status
	std::string _neighbors; //string which holds coding for neighbors(up(u)/down(d)/right(r)/left(l)
};

//circle class
class circle :
	public item
{
public:
	circle(std::list<item*>* BaseList, const float p[2], const float linecolor[3], const float radius, const int segments, const bool filled);
	~circle();

	void print() const;

	void draw();

	void ChangePosition(const float newPosition[2]);

	void ChangeRadius(const float newRadius);

private:
	float _p[2];
	float _linecolor[3];
	float _radius;
	int _segments;
	bool _filled;
};

//arrow class
class arrow :
	public item
{
public:
	arrow(std::list<item*>* BaseList, const float p[2], const float linecolor[3], const float size, const bool filled, const float angle);
	~arrow();

	void print() const;

	void draw();

	void ChangeAngle(const float newAngle);

	void ChangePosition(const float newPosition[2]);

private:
	float _p[2];
	float _linecolor[3];
	float _size;
	bool _filled;
	float _angle;
};

#endif
