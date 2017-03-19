#ifndef __OBJECT_H__
#define __OBJECT_H__

#include"Globals.h"
#include"Random.h"
//四个方向
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

class Object{
public:
	//图像
	CImage character;
	//大小
	int width;
	int height;
	//位置
	int x; 
	int y;
	//速度
	int speed;
	//方向
	int direct;
	//用于判断碰撞
	int xCenter;
	int yCenter;
	//生命
	int life;

	void calCenter(){
		xCenter = x + width / 2;
		yCenter = y + height / 2;
	}

	virtual void draw(CDC* cDC){
		character.Draw(*cDC, x, y, width, height, 0, 0, character.GetWidth(), character.GetHeight());
	}

	virtual void move(){
		if (direct == UP)y -= speed;
		else if (direct == DOWN)y += speed;

		calCenter();
	}

	virtual void move(int x, int y){//随飞机移动
		//水平方向靠近
		if (this->x<x)this->x += speed;
		else if (this->x>x)this->x -= speed;
		//竖直方向上靠近
		if (this->y < y)this->y += speed;
		else if (this->y>y) this->y -= speed;

		calCenter();
	}

	~Object(){
		character.Destroy();
	}
};

class Beauty :public Object{
public:
	Beauty(){
		character.Load(_T("fox.png"));

		width = character.GetWidth();
		height = character.GetHeight();
		
		x = WINDOW_WIDTH;
		y =-character.GetHeight();

		direct = UP;
		speed = 3;

		calCenter();
	}
};

class Flower :public Object{
public:
	Flower(){
		character.Load(_T("flower.png"));

		width = character.GetWidth()/4;
		height = character.GetHeight()/4;

		x = random(0, WINDOW_WIDTH);
		y = random(0, WINDOW_HEIGHT);

		speed = 1;
	}

	void move(){
		y += speed;
		if (y >= WINDOW_HEIGHT)y = 0;

		if (rand() % 2 == 0)
			x += speed;
		else x -= speed;
		if (x<0)x = WINDOW_WIDTH;
		else if (x >= WINDOW_WIDTH)x = 0;
	}
};

#endif