#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include"Object.h"

class BackGround :public Object{
public:
	CImage	sec;//第二张背景
	int y2;

	BackGround(){
		character.Load(_T("wallpaper.png"));
		sec.Load(_T("wallpaper.png"));

		x = 0;
		y = 0;

		y2 = -WINDOW_HEIGHT;

		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;

		direct = DOWN;
		speed = 2;

		life = 1;
	}

	~BackGround(){
		sec.Destroy();
	}

	void draw(CDC* cDC){
		character.Draw(*cDC, x, y, width, height, 0, 0, character.GetWidth(), character.GetHeight());
		sec.Draw(*cDC, x, y2, width, height, 0, 0, character.GetWidth(), character.GetHeight());
	}

	void move(){
		if (y == height)y = -height;
		else y+=speed;

		if (y2 == height)y2 = -height;
		else y2 += speed;
	}
};

//开始背景
class SBG :public Object{
public:
	CImage normal;
	CImage selected;
	CRect rect;

	bool   isSelected;

	SBG(){
		character.Load(_T("swallpaper.png"));
		normal.Load(_T("startNormal.png"));
		selected.Load(_T("startSelected.png"));

		width = normal.GetWidth()*3;
		height = normal.GetHeight()*3;

		x = WINDOW_WIDTH  / 4;
		y = WINDOW_HEIGHT  *2 / 3;

		isSelected = false;

		calCenter();
		rect.SetRect(x, y, x + width, y + height);
	}

	~SBG(){
		normal.Destroy();
		selected.Destroy();
	}

	void draw(CDC* cDC){
		character.Draw(*cDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, character.GetWidth(), character.GetHeight());

		if (isSelected)
		{
			selected.Draw(*cDC, x, y, width, height, 0, 0, normal.GetWidth(), normal.GetHeight());
		}
		else
		{
			normal.Draw(*cDC, x, y, width, height, 0, 0, normal.GetWidth(), normal.GetHeight());
		}
	}
};

class EBG :public Object{
public:
	CImage gameover;
	CImage againNor;//重新开始
	CImage againSel;
	CImage exitNor;//退出
	CImage exitSel;
	CRect  rGo;//gameover的rect
	CRect  rRes;//restart的rect
	CRect  rExit;//exit的rect
	bool   isExitSel;
	bool   isResSel;
	int xg; int yg;
	int xa; int ya;
	int xe; int ye;

	EBG(){
		character.Load(_T("overbg.png"));
		gameover.Load(_T("gameover.png"));
		againNor.Load(_T("againNor.png"));
		againSel.Load(_T("againSel.png"));
		exitNor.Load(_T("exitNor.png"));
		exitSel.Load(_T("exitSel.png"));

		x = 0;
		y = 0;
		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;

		xg = (WINDOW_WIDTH - gameover.GetWidth()*2) / 2; yg = WINDOW_HEIGHT / 4;
		xa = xg+againNor.GetWidth(); ya = yg + gameover.GetHeight()*6;
		xe = xg+gameover.GetWidth()*2-exitNor.GetWidth()*2; ye = ya;

		rRes.SetRect(xa, ya, xa + againNor.GetWidth(),ya + againNor.GetHeight());
		rExit.SetRect(xe, ye, xe + exitNor.GetWidth(), ye + exitNor.GetHeight());

		isExitSel = false;
		isResSel = false;
	}

	~EBG(){
	}

	void draw(CDC* cDC){
		character.Draw(*cDC, x, y, width, height, 0, 0, character.GetWidth(), character.GetHeight());
		gameover.Draw(*cDC, xg, yg, gameover.GetWidth()*2, gameover.GetHeight()*2, 0, 0, gameover.GetWidth(), gameover.GetHeight());
		if (isExitSel){
			exitSel.Draw(*cDC, xe, ye, exitSel.GetWidth(), exitSel.GetHeight(), 0, 0, exitSel.GetWidth(), exitSel.GetHeight());
		}
		else{
			exitNor.Draw(*cDC, xe, ye, exitNor.GetWidth(), exitNor.GetHeight(), 0, 0, exitNor.GetWidth(), exitNor.GetHeight());
		}

		if (isResSel){
			againSel.Draw(*cDC, xa, ya, againSel.GetWidth(), againSel.GetHeight(), 0, 0, againSel.GetWidth(), againSel.GetHeight());
		}
		else {
			againNor.Draw(*cDC, xa, ya, againNor.GetWidth(), againNor.GetHeight(), 0, 0, againNor.GetWidth(), againNor.GetHeight());
		}
	}
};

class SG :public Object{
public:
	CRect rect;
	bool isStop;

	SG(){
		character.Load(_T("play.png"));
		isStop = false;

		x = 5;
		y = 5;
		width = character.GetWidth();
		height = character.GetHeight() / 2;

		calCenter();
		rect.SetRect(x, y, x + width, y + height);
	}

	void draw(CDC* cDC){
		if (isStop){
			character.Draw(*cDC, x, y, width, height, 0, 0, width, height);
		}
		else{
			character.Draw(*cDC, x, y, width, height, 0, height, width, height);
		}
	}
};

#endif