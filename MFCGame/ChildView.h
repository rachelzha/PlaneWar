
// ChildView.h : CChildView 类的接口
//


#pragma once
#include "atltypes.h"
#include "afxwin.h"
#include "atlimage.h"

#include"Object.h"
#include"Plane.h"
#include"BackGround.h"
#include"Medicine.h"
#include<vector>
#include<mciapi.h>
#include<playsoundapi.h>
using namespace std;

#pragma comment (lib, "winmm.lib")

enum	STATE{ START, RUNNING, END };

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	STATE	m_state;

	Plane* target;
	Chopper* chopper;
	Boss* boss;
	Beauty* fox;

	vector<Flower*> flower;

	vector<EnemyGhost*> ghost;
	vector<EnemySC*> spacecraft;
	
	vector<Effect*> effect;

	SBG* sbg;
		
	BackGround* bg;
	SG* sg;

	EBG* ebg;
	//战场
	CRect battleground;

	int limit;
	bool callFox;

	int score;
	
	//计数
	int dizzyCount;
	int time;

	CRect m_client;//保存客户区大小

	CDC m_cacheDC;//缓冲dc
	CBitmap m_cacheCBitmap;//缓冲位图

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool isCollided(Object* a, Object* b);
	void manageAllCollision();

	void autoMove();
	void addEnemy();
	void addEffect(int x,int y);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void startUI(CDC* hDC);
	void runningUI(CDC* hDC);
	void endUI(CDC* hDC);
	void startGame();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void goOnGame();
	void pauseGame();
	void endGame();
	bool outOfWar(Object* o);
	bool outOfWar(int x, int y);
	void clearBullets();
};

