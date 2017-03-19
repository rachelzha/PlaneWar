#ifndef __BULLET_H__
#define __BULLET_H__

#include"Object.h"

class Bullet :public Object{
public:
};

class MyBullet :public Bullet{
public:
	MyBullet(int x,int y){
		character.Load(_T("bullet1.png"));
		width = character.GetWidth();
		height = character.GetHeight();

		this->x = x;
		this->y = y;
		direct = UP;

		speed = 40;
		life = 1;

		calCenter();
	}
};

class TheirBullet :public Bullet{
public:
	int directY;

	TheirBullet(int x,int y,int planeX,int planeY){
		character.Load(_T("bullet2.png"));
		width = character.GetWidth()/15;
		height = character.GetHeight()/15;

		this->x = x;
		this->y = y;
		
		if (x > planeX)direct = LEFT;
		else direct = RIGHT;

		if (y>planeY)directY = UP;
		else directY = DOWN;

		speed = 10;
		life = 1;

		calCenter();
	}

	void move(){
		if (direct == LEFT)x -= speed;
		else x += speed;

		if (directY == UP)y -= speed;
		else y += speed;

		calCenter();
	}
};

class BossBullet :public Bullet{
public:
	BossBullet(int x, int y){
		life = 20;

		character.Load(_T("tornado.png"));
		width = character.GetWidth();
		height = character.GetHeight();

		this->x = x;
		this->y = y;

		speed = 2;
		direct = LEFT;

		calCenter();
	}

	void move(){
		if (x >= WINDOW_WIDTH - width){
			direct = LEFT;
		}
		else if (x <= 0){
			direct = RIGHT;
		}

		y ++;
		if (direct == RIGHT)x += 8;
		else x -= 8;

		calCenter();
	}
};

#endif