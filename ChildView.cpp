
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "MFCGame.h"
#include "ChildView.h"
#include<conio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_PAINT 1
#define TIMER_CHECK 2
#define TIMER_MOVE 3
#define TIMER_BOSSFLY 4
#define TIMER_ADDENEMY 5
#define TIMER_COUNT 6
#define TIMER_SHOOT 7
#define TIMER_ENEMYSHOOT 8
#define TIMER_BOSSSHOOT 9



// CChildView

CChildView::CChildView()
{
	//初始化战场
	battleground.SetRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//初始化状态为开始
	m_state = START;
	
	sbg = new SBG;
}

CChildView::~CChildView()
{
	//回收资源
	if (bg != NULL)delete bg;
	bg = NULL;
	if (sg != NULL)delete sg;
	sg = NULL;

	if (chopper != NULL)delete chopper;
	chopper = NULL;
	if (boss != NULL)delete boss;
	boss = NULL;
	target = NULL;

	for (int i = 0; i < ghost.size(); i++){
		delete ghost[i];
	}

	for (int i = 0; i < spacecraft.size(); i++){
		delete spacecraft[i];
	}
	ghost.clear();
	spacecraft.clear();

	if (ebg != NULL)delete ebg;
	ebg = NULL;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	return TRUE;
}

void CChildView::OnPaint() 
{	
	// TODO:  在此处添加消息处理程序代码
	CDC *cDC = this->GetDC();   //获得当前窗口的DC   

	GetClientRect(&m_client);   //获得窗口的尺寸

	//创建缓冲DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);

	switch (m_state)
	{
		//绘制开始界面
	case START:
		startUI(&m_cacheDC);
		break;

		//绘制游戏界面
	case RUNNING:
		runningUI(&m_cacheDC);
		break;

		//绘制结束界面
	case END:
		endUI(&m_cacheDC);
		break;
	}


	
	
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);


	//在绘制完图后,使窗口区有效
	ValidateRect(&m_client);
	//释放缓冲DC
	m_cacheDC.DeleteDC();
	//释放对象
	m_cacheCBitmap.DeleteObject();

	
	ReleaseDC(cDC);           //释放DC
}



void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//nChar表示按下的键值
	if (m_state == RUNNING){
		switch (nChar)
		{
		case 'x':
		case 'X':
			if (dizzyCount>0&&target == chopper&&boss != NULL){
				target = boss;
				dizzyCount--;
				time = 0;
			}
			else target = chopper;
			break;
		case 'z':
		case 'Z':
			limit = 0;
			break;
		case 'q':
		case'Q':
			endGame();
			m_state = END;
			break;
		case ' ':{
				if (sg->isStop) {
					goOnGame();
					sg->isStop = false;
				}
				else {
					pauseGame();
					sg->isStop = true;
				}
			}
			break;
		case 'Y':
		case 'y':{
					 if(boss!=NULL)callFox = true;
					 break;
		}
		default:break;
		}
	}
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	switch (m_state)
	{
	case START:
	{
				  //如果点击了开始
				  if (sbg->rect.PtInRect(point))
				  {
					  //进入游戏状态
					  startGame();
					  m_state = RUNNING;
				  }
				  break;
	}
	case RUNNING:{
					 if (sg->rect.PtInRect(point)){
						 if (sg->isStop) {
							 goOnGame();
							 sg->isStop = false;
						 }
						 else {
							 pauseGame();
							 sg->isStop = true;
						 }
					 }
					 break;
	}
	case END:{
				 if (ebg->rRes.PtInRect(point))
				 {
					 startGame();
					 m_state = RUNNING;
				 }

				 else if (ebg->rExit.PtInRect(point))
				 {
					 if (IDYES == MessageBox(_T("\t亲, 真的要离开吗?"), _T("SpaceWar"), MB_YESNO))
					 {
						 //退出程序
						 exit(0);
					 }
				 }
				 break;
	}
	default:break;
	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
		case TIMER_PAINT:
		{
						OnPaint(); 
						break;
		}
		case TIMER_CHECK:{
							 //召唤Boss
							 if (score >= limit&&boss == NULL)boss = new Boss;
							 if (callFox&&fox == NULL)fox = new Beauty;

							manageAllCollision();

							if (chopper->life <= 0){
								endGame();
								m_state = END;
							}

							 if (boss!=NULL&&boss->life <= 0){

								 if (flower.size() == 0){
									 flower.resize(FLOWER_NUMBER);
									 for (int i = 0; i < flower.size(); i++)flower[i] = new Flower;

									 KillTimer(TIMER_CHECK);
									 KillTimer(TIMER_ADDENEMY);
									 KillTimer(TIMER_SHOOT);
									 KillTimer(TIMER_ENEMYSHOOT);
									 KillTimer(TIMER_BOSSSHOOT);
								 }

								 boss->life = 0;

								 break;
							 }

							 break;
		}
		case TIMER_MOVE:{
							autoMove();
							break;
		}
		case TIMER_BOSSFLY:
		{
						   if(boss!=NULL)boss->frame = 1-boss->frame;
						   break;
		}
		case TIMER_ADDENEMY:
		{
						  addEnemy();
						  break;
		}
		case TIMER_COUNT:{
							 if(time<10)time++;
							 if (time == 10){
								 target = chopper;
							 }
							 break; 
		}
		case TIMER_SHOOT:
		{
							chopper->shoot();
							break;
		}
		case TIMER_ENEMYSHOOT:{
								  if (spacecraft.size()){
									  int n = random(0, spacecraft.size());
									  spacecraft[n]->shoot(target->x,target->y);
								  }
								  break;

		}
		case TIMER_BOSSSHOOT:{
								 if (boss != NULL&&!callFox)boss->shoot();
		}
			break;
		}
}

void CChildView::addEnemy(){
	if (ghost.size() < 12){
		ghost.push_back(new EnemyGhost);
	}

	if (spacecraft.size() < 5){
		spacecraft.push_back(new EnemySC);
	}
}

void CChildView::addEffect(int x,int y){
	int n = random(0, 3);
	if (target != chopper)n = 1;
	if (n == 0){
		effect.push_back(new Dizzy(x, y));
	}
	else{
		effect.push_back(new Medicine(x, y));
	}
}

void CChildView::autoMove(){
	vector<Bullet*>::iterator b;

	//boss
	if (boss != NULL){
		boss->move();
		//bullets
		for (b = boss->bullets.begin(); b != boss->bullets.end(); ){
			(*b)->move();
			if ((*b)->life <= 0 || outOfWar((*b)->xCenter,((*b)->yCenter))){
				delete (*b);
				b = boss->bullets.erase(b);
			}
			else b++;
		}
	}

	//ghost
	for (vector<EnemyGhost*>::iterator it = ghost.begin(); it != ghost.end();){
		(*it)->move(target->x,target->y);
		if ((*it)->life <= 0){
			delete (*it);
			it = ghost.erase(it);
		}
		else it++;
	}

	//spacecraft
	for (vector<EnemySC*>::iterator it = spacecraft.begin(); it != spacecraft.end(); ){
		for (b = (*it)->bullets.begin(); b != (*it)->bullets.end(); ){
			(*b)->move();
			if ((*b)->life <= 0){
				delete (*b);
				b = (*it)->bullets.erase(b);
			}
			else b++;
		}

		(*it)->move();
		if ((*it)->life<=0){

			addEffect((*it)->x, (*it)->y);
			PlaySound(TEXT("res\\music\\die.wav"), NULL, SND_FILENAME | SND_ASYNC);

			delete (*it);
			it = spacecraft.erase(it);
		}
		else if ((*it)->y > WINDOW_HEIGHT){
			delete (*it);
			it = spacecraft.erase(it);
		}
		else it++;
	}

	//bullets
	for (b = chopper->bullets.begin(); b != chopper->bullets.end();){
		(*b)->move();

		if ((*b)->y > WINDOW_HEIGHT||(*b)->life<=0){
			delete (*b);
			b = chopper->bullets.erase(b);
		}
		else b++;
	}

	//effect
	for (vector<Effect*>::iterator it = effect.begin(); it != effect.end();){
		(*it)->move();

		if ((*it)->y > WINDOW_HEIGHT || (*it)->life <= 0){
			delete (*it);
			it = effect.erase(it);
		}
		else it++;
	}

	//fox
	if (fox != NULL)fox->move(boss->x+boss->width/2, boss->y+boss->height/2);

	//flower
	for (int i = 0; i < flower.size(); i++)flower[i]->move();

	//background
	bg->move();
}


bool CChildView::isCollided(Object* a, Object* b){
	if (a->xCenter< b->xCenter + (b->width / 2 + a->width / 2) && a->xCenter>b->xCenter - (b->width / 2 + a->width / 2) &&
		a->yCenter<b->yCenter + (b->height / 2 + a->height / 2) && a->yCenter>b->yCenter - (b->height / 2 + a->height / 2))return true;
	return false;
}

void CChildView::manageAllCollision(){
	vector<Bullet*>::iterator b;

	//target
	for (vector<EnemyGhost*>::iterator p = ghost.begin(); p != ghost.end();p++){
		if (isCollided((*p), target)){//target 和 ghost
			(*p)->life--;
			target->life--;

			score++;
		}
	}

	for (vector<EnemySC*>::iterator p = spacecraft.begin(); p != spacecraft.end(); p++){
		if (isCollided((*p), target)){//target 和 spacecraft
			(*p)->life--;
			target->life--;

			score++;
		}

		for (b = (*p)->bullets.begin(); b != (*p)->bullets.end(); b++){
			if (isCollided((*b), target)){//target 和 spacecraft的子弹
				(*b)->life--;
				target->life--;
			}
		}
	}

	//target bullets
	for ( b = target->bullets.begin(); b != target->bullets.end(); b++){
		for (vector<EnemyGhost*>::iterator p = ghost.begin(); p != ghost.end();p++ ){
			if (isCollided((*p), (*b))){//target bullets and ghost
				(*p)->life--;
				if (target == chopper)(*b)->life--;

				score++;
			}
		}

		for (vector<EnemySC*>::iterator p = spacecraft.begin(); p != spacecraft.end(); p++){
			if (isCollided((*p), (*b))){//target bullets and spacecraft
				(*p)->life--;
				if (target == chopper)(*b)->life--;

				score++;
			}
		}
	}

	//effect
	for (vector<Effect*>::iterator m = effect.begin(); m != effect.end(); m++){
		if (isCollided((*m), chopper)){//plane 和 effect
			(*m)->life--;
			if((*m)->type==0)chopper->life+=10;//10
			else dizzyCount++;
		}
	}

	// plane and boss
	if (boss != NULL){
		if (isCollided(boss, chopper)){//boss and chopper
			boss->life--;
			chopper->life --;
		}
		
		for (b = chopper->bullets.begin(); b != chopper->bullets.end(); b++){
			if (isCollided((*b), boss)){//boss and chopper bullet
				(*b)->life--;
				boss->life--;
				score++;
			}
		}

		for (b = boss->bullets.begin(); b != boss->bullets.end(); b++){
			if (isCollided((*b), chopper)){//chopper and boss bullet
				chopper->life--;
			}
		}
	}

	//fox
	if (fox != NULL){
		if (isCollided(fox, boss)){
			boss->life--;
		}
	}
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (START == m_state)
	{
		if(sbg->rect.PtInRect(point))sbg->isSelected = true;
		else sbg->isSelected = false;
	}
	else if (RUNNING == m_state){
		if (point.x > chopper->x)chopper->direct = RIGHT;
		else if (point.x < chopper->x)chopper->direct = LEFT;
		else chopper->direct = UP;

		chopper->x = point.x;
		chopper->y = point.y;
		chopper->calCenter();
	}
	else if (END == m_state)
	{
		if (ebg->rRes.PtInRect(point))
		{
			ebg->isResSel = true;
		}
		else
		{
			ebg->isResSel = false;
		}

		if (ebg->rExit.PtInRect(point))
		{
			ebg->isExitSel = true;
		}
		else
		{
			ebg->isExitSel = false;
		}
	}
}


void CChildView::startUI(CDC* cDC)
{
	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\startbg.mp3 alias startbg"), NULL, 0, NULL);
	mciSendString(_T("Play startbg repeat"), NULL, 0, NULL);

	//贴开始背景
	sbg->draw(cDC);
	
}


void CChildView::runningUI(CDC* m_cacheDC)
{
	//贴背景
	bg->draw(m_cacheDC);
	sg->draw(m_cacheDC);
	
	//子弹
	for (int i = 0; i < chopper->bullets.size(); i++)chopper->bullets[i]->draw(m_cacheDC);
	if(boss!=NULL)for (int i = 0; i < boss->bullets.size(); i++)boss->bullets[i]->draw(m_cacheDC);

	//贴enemy
	for (int i = 0; i < ghost.size(); i++)ghost[i]->draw(m_cacheDC);

	for (int i = 0; i < spacecraft.size(); i++){
		spacecraft[i]->draw(m_cacheDC);
		for (int j = 0; j < spacecraft[i]->bullets.size(); j++){
			spacecraft[i]->bullets[j]->draw(m_cacheDC);
		}
	}

	//medicine
	for (int i = 0; i < effect.size(); i++){
		effect[i]->draw(m_cacheDC);
	}

	//贴人物  
	if (boss != NULL)boss->draw(m_cacheDC);
	if (fox != NULL)fox->draw(m_cacheDC);
	chopper->draw(m_cacheDC);

	for (int i = 0; i < flower.size(); i++)flower[i]->draw(m_cacheDC);

	CImage temp;
	temp.Load(_T("dizzy.png"));
	temp.Draw(*m_cacheDC, 10, WINDOW_HEIGHT - 150, temp.GetWidth() / 2, temp.GetHeight() / 2, 0, 0, temp.GetWidth(), temp.GetHeight());

	CString str;
	CString str2;
	CString str3;
	CString str4;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = 40;

	CFont font;
	font.CreateFontIndirectW(&lf);

	if(boss==NULL)str.Format(_T("Score: %d    Life:%d"), score,chopper->life);
	else str.Format(_T("Score: %d    Life:%d    Boss:%d"), score, chopper->life,boss->life);

	//设置文本属性
	m_cacheDC->SelectObject(font);
	m_cacheDC->SetTextColor(RGB(255, 127, 39));
	m_cacheDC->SetBkMode(TRANSPARENT);

	m_cacheDC->TextOutW(sg->character.GetWidth() + 20, 10, str);

	str3 = "Z-召唤Boss    Y-召唤妖狐";
	str4 = "X-眩晕敌方";
	m_cacheDC->TextOutW(0, WINDOW_HEIGHT - 250, str3);
	m_cacheDC->TextOutW(0, WINDOW_HEIGHT - 200, str4);

	m_cacheDC->SetTextColor(RGB(0, 255, 0));
	for (int i = 0; i < dizzyCount; i++)str2.AppendChar('0');
	m_cacheDC->TextOutW(temp.GetWidth(), WINDOW_HEIGHT - 150, str2);

	//释放字体资源
	font.DeleteObject();
}


void CChildView::endUI(CDC* cDC)
{
	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\overbg.mp3 alias overbg"), NULL, 0, NULL);
	mciSendString(_T("Play overbg"), NULL, 0, NULL);

	ebg->draw(cDC);

	CString str;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -50;

	CFont font;
	font.CreateFontIndirectW(&lf);

	if (boss==NULL||boss->life>0)str.Format(_T("  You lose.  Your Score: %d"), score);
	else str.Format(_T("  You WIN!  Your Score: %d"), score);

	//设置文本属性
	cDC->SelectObject(font);
	cDC->SetTextColor(RGB(255, 0, 39));
	cDC->SetBkMode(TRANSPARENT);

	cDC->TextOutW( ebg->xg, ebg->yg+ebg->rGo.Height()+200, str);

	//释放字体资源
	font.DeleteObject();
}


void CChildView::startGame()
{
	//先关闭开始、结束背景音乐
	mciSendString(_T("Close startbg"), NULL, 0, NULL);
	mciSendString(_T("Close overbg"), NULL, 0, NULL);

	//回收资源
	if(sbg!=NULL)delete sbg;
	sbg = NULL;

	//初始化
	bg = new BackGround;
	sg = new SG;

	chopper = new Chopper;
	boss = NULL;
	target = chopper;
	//妖狐
	fox = NULL;

	limit = 1000;
	callFox = false;

	score = 0;

	dizzyCount = 0;
	time = 0;

	ghost.resize(3);
	for (int i = 0; i < ghost.size(); i++){
		ghost[i] = new EnemyGhost();
	}

	spacecraft.resize(2);
	for (int i = 0; i < spacecraft.size(); i++){
		spacecraft[i] = new EnemySC();
	}

	
	//创建MOVE的定时器
	SetTimer(TIMER_MOVE, 10, NULL);

	SetTimer(TIMER_CHECK, 10, NULL);

	//创建boss 飞的定时器
	SetTimer(TIMER_BOSSFLY, 100, NULL);
	//创建增加敌方的定时器
	SetTimer(TIMER_ADDENEMY, 1000, NULL);
	//创建发射子弹的定时器
	SetTimer(TIMER_SHOOT, 100, NULL);
	//创建敌方发射子弹的定时器
	SetTimer(TIMER_ENEMYSHOOT, 500, NULL);
	//创建Boss发射子弹的定时器
	SetTimer(TIMER_BOSSSHOOT, 12000, NULL);
	SetTimer(TIMER_COUNT, 1000, NULL);//1s


	//打开背景音乐
	mciSendString(_T("Open res\\music\\gamebg.mp3 alias gamebg"), NULL, 0, NULL);
	//播放背景音乐(重复播放)
	mciSendString(_T("Play gamebg repeat"), NULL, 0, NULL);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//创建一个10毫秒产生一次消息的定时器  
	SetTimer(TIMER_PAINT, 10, NULL);

	return 0;
}


void CChildView::goOnGame()
{
		//创建MOVE的定时器
		SetTimer(TIMER_MOVE, 10, NULL);
		//创建boss 飞的定时器
		SetTimer(TIMER_BOSSFLY, 100, NULL);
		SetTimer(TIMER_COUNT, 1000, NULL);

		if (boss == NULL || boss->life > 0){
			SetTimer(TIMER_CHECK, 10, NULL);
			//创建增加敌方的定时器
			SetTimer(TIMER_ADDENEMY, 1000, NULL);
			//创建发射子弹的定时器
			SetTimer(TIMER_SHOOT, 100, NULL);
			//创建敌方发射子弹的定时器
			SetTimer(TIMER_ENEMYSHOOT, 500, NULL);
			//创建Boss发射子弹的定时器
			SetTimer(TIMER_BOSSSHOOT, 12000, NULL);
		}


		//播放音乐
		mciSendString(_T("Play gamebg repeat"), NULL, 0, NULL);
}


void CChildView::pauseGame()
{
	//关闭计时器
	KillTimer(TIMER_MOVE);
	KillTimer(TIMER_CHECK);
	KillTimer(TIMER_BOSSFLY);
	KillTimer(TIMER_ADDENEMY);
	KillTimer(TIMER_SHOOT);
	KillTimer(TIMER_ENEMYSHOOT);
	KillTimer(TIMER_BOSSSHOOT);
	KillTimer(TIMER_COUNT);

	//暂停音乐
	mciSendString(_T("Pause die"), NULL, 0, NULL);
	mciSendString(_T("Pause gamebg"), NULL, 0, NULL);
}

void CChildView::endGame()
{
	//关闭计时器
	KillTimer(TIMER_MOVE);
	KillTimer(TIMER_CHECK);
	KillTimer(TIMER_BOSSFLY);
	KillTimer(TIMER_ADDENEMY);
	KillTimer(TIMER_SHOOT);
	KillTimer(TIMER_ENEMYSHOOT);
	KillTimer(TIMER_BOSSSHOOT);
	KillTimer(TIMER_COUNT);

	// 回收资源
	if (bg != NULL)delete bg;
	if (sg != NULL)delete sg;
	
	if (chopper != NULL)delete chopper;
	if (fox != NULL)delete fox;

	for (int i = 0; i < ghost.size(); i++){
		delete ghost[i];
	}

	for (int i = 0; i < spacecraft.size(); i++){
		delete spacecraft[i];
	}

	for (int i = 0; i < flower.size(); i++){
		delete flower[i];
	}
	ghost.clear();
	spacecraft.clear();
	flower.clear();

	//关闭音乐
	mciSendString(_T("Close attack"), NULL, 0, NULL);
	mciSendString(_T("Close die"), NULL, 0, NULL);
	mciSendString(_T("Close gamebg"), NULL, 0, NULL);

	ebg = new EBG;
}


bool CChildView::outOfWar(Object* o)
{
	if (o->y >= battleground.top&&o->y+o->height <= battleground.bottom
		&&o->x >= battleground.left&&o->x+o->width <= battleground.right)return false;

	return true;
}

bool CChildView::outOfWar(int x, int y){
	if (y >= battleground.top&&y <= battleground.bottom&&x >= battleground.left&&x <= battleground.right)return false;
	return true;
}


void CChildView::clearBullets()
{
	for (int i = 0; i < chopper->bullets.size(); i++){
		delete chopper->bullets[i];
	}
	chopper->bullets.clear();

	for (int i = 0; i < boss->bullets.size(); i++){
		delete boss->bullets[i];
	}
	boss->bullets.clear();
}

