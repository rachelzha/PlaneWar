#ifndef __MEDICINE_H__
#define __MEDICINE_H__

#include"Object.h"
#include"Random.h"

class Effect :public Object{
public:
	int type;
	Effect(){
		speed = 3;
		direct = DOWN;
		life = 1;
	}
};

class Medicine :public Effect{
public:
	Medicine(int x, int y){
		character.Load(_T("medicine.png"));
		width = character.GetWidth()*2/3;
		height = character.GetHeight()*2/3;

		this->x = x;
		this->y = y;

		type = 0;

		calCenter();
	}
};

class Dizzy :public Effect{
public:
	Dizzy(int x, int y){
		character.Load(_T("dizzy.png"));
		width = character.GetWidth()*2/3;
		height = character.GetHeight()*2/3;

		this->x = x;
		this->y = y;

		type = 1;

		calCenter();
	}
};

#endif