# PlaneWar
use MFC to implement PlaneWar Game

编译环境：VS2013

资源文件：MFCGame目录下的PNG文件为游戏图片，MFCGame/res/music/中为游戏音乐文件

游戏说明：（有音效，需要开声音）
游戏开始：点击开始
游戏操作：
	移动：鼠标移动
	
	发射子弹：自动发射
	
	暂停：点击左上角图标或键盘空格
	
	结束暂停：再次点击结束暂停
	
	退出游戏：键盘q
	
	召唤大天狗：键盘z
	
	召唤妖狐：键盘y
	
	眩晕敌方：键盘x

游戏NPC说明：

	幽灵：不发射子弹，但是他会追踪你
	
	宇宙飞船：会追踪你的位置发射子弹，击灭它可以得到特效药
	
	特效药：1.红色钢铁侠特效药：生命+10
	
		2.绿色绿巨人特效药：在Boss出现后方可使用，可以使得地方小兵攻击敌方Boss10秒钟。
			（左下角可以查看有多少绿巨人特效药）
	
	大天狗：地方大Boss.血量厚，大招：龙卷
	
	妖狐：可以沉默大天狗，大天狗无法发大招

游戏胜利：会出现樱花，键盘Q退出

游戏结束：可以选择重新开始和退出
	

代码文件说明：

Object.h
	
	Object类:描述物体的类，所有类的基类
	
	Beauty类：妖狐的类,继承Object
	
	Flower类：樱花的类,继承Object

Plane.h
	
	Plane类：描述所有飞行物的类，继承Object
	
	Chopper类：玩家类，继承Plane
	
	EnemyGhost类：敌方幽灵类，继承Plane
	
	EnemySC类：敌方宇宙飞船类，继承Plane
	
	Boss类：敌方Boss类，继承Plane

Bullet.h
	
	Bullet类:子弹类
	
	MyBullet类：己方子弹类，继承Bullet
	
	TheirBullet类：宇宙飞船子弹类，继承Bullet
	
	BossBullet类：大天狗龙卷风，继承Bullet

Medicine.h
	
	Effect类：特效类
	
	Medicine类：加血工具类，继承Effect
	
	Dizzy类：眩晕工具类，继承Effect

Random.h 随机函数random

Globals.h 全局变量
