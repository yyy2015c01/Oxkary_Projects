#include <bits/stdc++.h>
#define minR 2
#define maxR 30
#define minBetweenPercent 0.5
#define maxFlyFrames
#define eps 1
#define maxPowPerSec 0.1
#define Range 0.5
struct Ship {
	double x,y;
	bool isFailed;
	int color;//��ɫ��6λ16������ɫ���룩 
    int shootCounts;//�䱬����
    Ship(double x=0,double y=0,int color=0):x(x),y(y),isFailed(false),color(color),shootCounts(0){}
};
struct Bomb {
	double x,y;//���� 
	double vx,vy;//ˮƽ/��ֱ�����ٶ� 
	int id;//�����ɴ�
	bool isCrashed;//�Ƿ�����ײ 
	int color;//��ɫ��6λ16������ɫ���룩 
    Bomb(double x=0,double y=0,double vx=0,double vy=0,int id=-1,int color=0):x(x),y(y),vx(vx),vy(vy),id(id),isCrashed(false),color(color){}
};
struct Planet {
	double x,y;//���� 
	double r,M;//�뾶,���� 
	int color;//��ɫ��6λ16������ɫ���룩 
//	bool isDeleted;
    Planet(double x=0,double y=0,double r=0,int color=0):x(x),y(y),r(r),M(r*r),color(color){}
};
class Info {
	private:
		int	minPlanets,maxPlanets,playerCounts,minDis,height,width;//�������������������������������������������������߶ȣ����� 
//		double minR;
		std::vector <Ship> ships;//�ɴ� 
//		std::vector <Planet> planets;//���� 
		Bomb bomb;
    public:std::vector <Planet> planets;//����
		
        Info(){}
		void init(int minPlanets,int maxPlanets,int playerCounts,int minDis,int height,int width) {
//			this->minR=minR;
			this->minPlanets=minPlanets;
			this->maxPlanets=maxPlanets;
			this->playerCounts=playerCounts;
			this->minDis=minDis;
			this->height=height;
			this->width=width;
            ships.clear();
            planets.clear();
		}
		int getWidth(){return width;}
		int getHeight(){return height;}
		int getShipsNum() {	return ships.size();}
        int getPlanetsNum() {return planets.size();}
		void setBomb(double x,double y,double vx,double vy,int id,bool crashed) 
		{
			bomb.x=x;
			bomb.y=y;
			bomb.vx=vx;
			bomb.vy=vy;
			bomb.id=id;
			bomb.isCrashed=crashed;
		}
		Bomb getBomb() {return bomb;}
		void failShip(int i) 
		{
            if (i>=(int)ships.size()) assert(0);
			ships[i].isFailed=true;
		}
		void plusShootCnt(int id) 
		{
            if (id>=(int)ships.size()) assert(0);
			ships[id].shootCounts++;
		}
		void crashBomb() {bomb.isCrashed=true;}
		int getNowShip() 
		{
			if (bomb.isCrashed) return 0;
			else return bomb.id;
		}
		bool flying() {return !bomb.isCrashed;} 
        void addPlanet(double x,double y,double r) {planets.push_back(Planet (x,y,r)); }
        Planet getPlanet(int i)
		{
            if (i>=(int)planets.size()) assert(0);
            return planets[i];
		}
		Ship getShip(int i) 
		{
            if (i>=(int)ships.size()) {
                printf("Error ships[%u] but ask %d\n", (int)ships.size(), i);
                assert(0);
            }
            return ships[i];
		}
        std::vector<Planet> getPlanets() {
            return planets;
        }
        std::vector<Ship> getShips() {
            return ships;
        }
		Ship addShip(double x,double y,int color) 
		{
			Ship now(x,y,color);
			ships.push_back(now);
			return now;
		}
		
//		void
};
class GravityShooter {
//	private:
		
//		int	minPlanets,maxPlanets,playerCounts,minDis,height,width;
	public:Info shooter;
		inline double rand01() {//���ؽ���0~1֮�������������� 
			return rand() / double(RAND_MAX);
		}
		inline double random(double Min,double Max) {
			return rand01()*(Max-Min)+Min; 
		}
		inline double dis(double x1,double y1,double x2,double y2) {
			return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
		}
		void init(int minPlanets,int maxPlanets,int playerCounts,int minDis,int height,int width,std::vector <int> colors) {
			
			srand(time(0));
			shooter.init(minPlanets,maxPlanets,playerCounts,minDis,height,width);
			for (int i=0;i<playerCounts;i++) { 
				bool access=true;
				double x,y;
				do {
					x=rand01()*width,y=rand01()*height;
					access=true;
					for (int j=0;j<i;j++) {
						Ship now=shooter.getShip(j);
						if (dis(now.x,now.y,x,y)<minDis)  
							access=false;
					}
				}while(!access);
				shooter.addShip(x,y,colors[i]);
			}
			int planetCnt=random(minPlanets,maxPlanets);
			for (int i=0;i<planetCnt;i++) {
//				std::cout<<"MAKE!"<<std::endl;
				double x,y,nowMaxR;
				bool access=true;
                int num=shooter.getShipsNum();
				do {
					access=true;
					x=rand01()*width,y=rand01()*height;
					nowMaxR=999999999.0;
                    for (int j=0;j<num;j++) {
						Ship now=shooter.getShip(j);
						nowMaxR=std::min(nowMaxR,dis(now.x,now.y,x,y));
					}
					for (int j=0;j<i;j++) {
						Planet now=shooter.getPlanet(j);
						nowMaxR=std::min(nowMaxR,dis(now.x,now.y,x,y)-now.r);
					}
					if (nowMaxR*minBetweenPercent<minR) access=false;
				}while(!access);
				double r=random(minR,std::min((double)maxR,nowMaxR)*minBetweenPercent);
				shooter.addPlanet(x,y,r);
			}
		}
		int calculate()
		{
			int w=shooter.getWidth(),h=shooter.getHeight();
			Bomb bomb=shooter.getBomb();
			if (bomb.isCrashed||bomb.id==-1) return -2;//�쳣 
			int planetsCnt=shooter.getPlanetsNum();
			double ax=0,ay=0;
			std::cout<<"planetsCnt:"<<planetsCnt<<std::endl; 
			for (int i=0;i<planetsCnt;i++) {
                Planet now=shooter.getPlanet(i);
                double s=dis(bomb.x,bomb.y,now.x,now.y);
                ax+=(now.M/(s*s))*((now.x-bomb.x)/s);
                ay+=(now.M/(s*s))*((now.y-bomb.y)/s);
			}
            if (bomb.x<-w*Range||bomb.y<-h*Range||bomb.x>shooter.getWidth()+w*Range||bomb.y>shooter.getHeight()+h*Range) {
				shooter.crashBomb();
				puts("-1");
				return -1;
			}
			for (int i=0;i<planetsCnt;i++) {
                Planet now=shooter.getPlanet(i);
				double s=dis(bomb.x,bomb.y,now.x,now.y);
//				std::printf("%lf %lf %lf %lf %lf\n",bomb.x,now.x,bomb.y,now.y,now.r);
				if (std::fabs(s)<now.r) {
					shooter.crashBomb();
					puts("-1");
					return -1;
				}
			}
			int shipsCnt=shooter.getShipsNum();
			for (int i=0;i<shipsCnt;i++) {
				Ship now=shooter.getShip(i);
				double s=dis(bomb.x,bomb.y,now.x,now.y);
				if (std::fabs(s)<eps&&now.isFailed==false) {
					shooter.failShip(i);
					shooter.crashBomb();
					puts("iiiiiiiiiiiiii");
					return i;
				}
			}
            std::cout<<"ax:"<<bomb.x+bomb.vx<<"    ay:"<<bomb.y+bomb.vy<<std::endl;
            shooter.setBomb(bomb.x+bomb.vx,bomb.y+bomb.vy,bomb.vx+ax,bomb.vy+ay,bomb.id,bomb.color);
			return 0;
		}
		int shoot(int id,double power,double deg) //DegΪ������ 
		{
			shooter.plusShootCnt(id);
            double Sin=std::sin(deg),Cos=std::cos(deg);
			Ship now=shooter.getShip(id);
			shooter.setBomb(now.x+(2*eps*Sin),now.y+(2*eps*Cos),maxPowPerSec*Sin*power,maxPowPerSec*Cos*power,id,0);
			
			
			//debug
//			std::cout<<"hello:"<<shooter.getBomb().id<<std::endl;
            return 0;
		}
		Bomb getBomb()//���� 
		{
			return shooter.getBomb();
		}
};//Game;
/*
int main()
{
	std::vector <int> a;
	a.push_back(666);
	a.push_back(666);
	a.push_back(666);
	Game.init(1,1,2,10,100,100,a);
    std::cout<<"cnt:"<<Game.shooter.planets.size()<<std::endl;
	
    Game.shoot(0,0,0.25*3.1415926536);
	while(1) {
		Game.calculate();
		std::cout<<Game.getBomb().x<<"  "<<Game.getBomb().y<<"   "<<Game.getBomb().vx<<"   "<<Game.getBomb().vy<<"   "<<"   "<<Game.getBomb().isCrashed<<std::endl;
        std::cout<< Game.shooter.planets[0].x<<","<<Game.shooter.planets[0].y<<std::endl;
		for (int i=1;i<=300000000;i++) ;
	}
}*/
