#ifndef __PLANE_H__
#define __PLANE_H__

#include"Bullet.h"
#include"Random.h"
#include<vector>
using namespace std;

class Plane:public Object{
public:
	//子弹
	vector<Bullet*> bullets;

	virtual void shoot(){
	}

	~Plane(){
		for (int i = 0; i < bullets.size(); i++){
			delete bullets[i];
		}
		bullets.clear();
	}
};


class Chopper :public Plane{
public:
	Chopper(){
		character.Load(_T("chopper.png"));
		width = 181;
		height = 141;

		//初始位置
		x = WINDOW_WIDTH / 2;
		y = WINDOW_HEIGHT - height - 100;

		speed = 50;
		direct = UP;
		life = 200;

		calCenter();
	}

	void draw(CDC* cDC){//重写
		character.Draw(*cDC, x, y, width, height, 0, direct*height, width, height);
	}

	void shoot(){
		MyBullet* mb = new MyBullet(x + width / 2, y);
		bullets.push_back(mb);
	}
};

class EnemyGhost :public Plane{
public:
	EnemyGhost(){
		character.Load(_T("ghost.png"));
		width = character.GetWidth()*2/3;
		height = character.GetHeight()*2/3;

		x = random(0, WINDOW_WIDTH - width);
		y = random(0, WINDOW_HEIGHT / 2);

		speed = 5;
		life = 2;
		calCenter();
	}
};

class EnemySC :public Plane{
public:
	EnemySC(){
		character.Load(_T("spacecraft.png"));
		width = character.GetWidth()*2/3;
		height = character.GetHeight()*2/3;

		x = random(0, WINDOW_WIDTH - width);
		y = random(0, height / 2);

		speed = 1;
		direct = DOWN;

		life = 20;

		calCenter();
	}

	void shoot(int planeX, int planeY){
		TheirBullet* tb = new TheirBullet(x + width / 2, y + height, planeX, planeY);
		bullets.push_back(tb);
	}

};

class Boss :public Plane{
public:
	int frame;
	int destX;
	int destY;

	Boss(){
		character.Load(_T("boss.png"));
		width = character.GetWidth() / 2;
		height = character.GetHeight();

		x = random(0, WINDOW_WIDTH - width);
		y = 60;

		destX = WINDOW_WIDTH / 2;
		destY = WINDOW_HEIGHT / 3;

		direct = RIGHT;

		speed = 3;
		life = 1000;

		frame = 0;
		calCenter();
	}

	void draw(CDC* cDC){//重写
		character.Draw(*cDC, x, y, width, height, frame*width, 0, width, height);
	}

	void move(){//左右
		if (x >= WINDOW_WIDTH - width)direct = LEFT;
		else if (x <= 0)direct = RIGHT;

		if (direct == RIGHT)x += speed;
		else x -= speed;

		calCenter();
	}

	void moveToFox(){
		if (x < destX)x += speed;
		else if(x>destX)x -= speed;

		if (y < destY)y += speed;
		else if(y>destY) y -= speed;

		calCenter();
	}

	void shoot(){
		BossBullet* tb = new BossBullet(x, y + height-100);
		bullets.push_back(tb);
	}
};

#endif