#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <list>
#include <queue>
#include "json/json.h"
#include "json/json-forwards.h"
#include "jsoncpp.cpp"

int langeliu_dydis = 300;
int prideti_eile_kas = 10;
bool meniu = true;

void* memory;
int ClientWidth;
int ClientHeight;
bool running = true;
struct camera{
	float x = -1100;
	float y = -3000;
	float z = 2500;
	float roty = 0;
	float rotz = -0.09;
	bool left = 0;
	bool right = 0;
	bool up = 0;
	bool down = 0;
	bool front = 0;
	bool back = 0;
	float speed = -10;
	float mousespeed = -0.00872665/10;
	//float zoom = 1;
	int frame = GetTickCount();
	bool locked = false;
};camera cam;

void DrawPixel(int x, int y, int Color) {
	if(x<0)return;
	if(y<0)return;
	if(x>=ClientWidth)return;
	if(y>=ClientHeight)return;
	((int*)memory)[y * ClientWidth + x] = Color;
}

int cross(int x1, int y1, int x2, int y2, int x3, int y3) {
	return (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
}
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,int color = 0xFFFFFFFF) {
	int minX = std::min({x1, x2, x3});
    int minY = std::min({y1, y2, y3});
    int maxX = std::max({x1, x2, x3});
    int maxY = std::max({y1, y2, y3});
	for (int y = minY; y <= maxY; y++) {
		for (int x = minX; x <= maxX; x++) {
			int a = cross(x1, y1, x2, y2, x, y);
			int b = cross(x2, y2, x3, y3, x, y);
			int c = cross(x3, y3, x1, y1, x, y);
			if(a>=0&&b>=0&&c>=0)DrawPixel(x,y,color);
			if(a<=0&&b<=0&&c<=0)DrawPixel(x,y,color);
		}
	}
}


struct daiktai{
	virtual void draw() = 0;
};
const int dydis = 8;
int skaitmuo[10][dydis][dydis] = {
	{//0
		{0,0,0,0,0,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//1
		{0,0,0,0,0,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//2
		{0,0,0,0,0,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//3
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//4
		{0,0,0,0,0,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//5
		{0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//6
		{0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//7
		{0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//8
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
	{//9
		{0,0,0,0,0,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,1,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0},
	},
};
struct point : public daiktai {
	float x = 0;
	float y = 0;
	float z = 0;
	int color = 0xFFFFFFFF;
	int size = 5;
	int skaicius = 0;
	point(float a, float b, float c,int e = 0,int d = 0xFFFFFFFF) : x(a), y(b),z(c),color(d),skaicius(e){}
	float getx(){
		float dx = cam.x-x;
		float dz = cam.z-z;
		float droty = atan2(dx,dz);// -cam.roty
		if(cos(droty)<0.707)return -9999999;//   0.707
		
		return ClientWidth/2*(abs(cos(droty-0.785))-abs(sin(droty-0.785))+1);
		//return ClientWidth/2*(sin(2*droty)+1);
		
	}
	float gety(){
		float dy = ((cam.y-y)*cos(cam.rotz))-((cam.z-z)*sin(cam.rotz)*10);
		float dz = ((cam.z-z)*cos(cam.rotz))+((cam.y-y)*sin(cam.rotz));
		float drotz = atan2(dy,dz);//-cam.rotz
		if(cos(drotz)<0.707)return -999999999;
		//return ClientHeight/2;
		return ClientHeight/2*(abs(cos(drotz-0.785))-abs(sin(drotz-0.785))+1);
		//return ClientHeight/2*(sin(2*drotz)+1);
		
	}
	void draw() override {
		float fx = getx();
		float fy = gety();
		int tskaicius = skaicius+1;
		int offsetas = 0;
		while(tskaicius>0) {
			for(int x = offsetas;x<dydis+offsetas;x++){
				for(int y = 0;y<dydis;y++){
					DrawPixel(fx+x,fy-y,(skaitmuo[tskaicius%10][y][x-offsetas]*0xFFFFFFFF));
				}
			}
			offsetas-=dydis;
			tskaicius/=10;
		}
	}
	void draw_by_nr(int nr){
		update_by_nr(nr,0);
		skaicius = nr;
		draw();
	}
	void update_by_nr(int numeris,int as_kelintas){
		int nz = -(langeliu_dydis*(numeris/prideti_eile_kas));
		int nx;
		if(numeris/prideti_eile_kas %2 == 1)nx = -(langeliu_dydis*((prideti_eile_kas-1)-numeris%prideti_eile_kas));
		else nx = -(langeliu_dydis*(numeris%prideti_eile_kas));
		
		switch(as_kelintas){
			case(0):{
				x = nx+langeliu_dydis/2;
				z = nz+langeliu_dydis/2;
				y = -100;
			}break;
			case(1):{
				x = nx;
				z = nz;
			}break;
			case(2):{
				x = nx;
				z = nz+langeliu_dydis;
			}break;
			case(3):{
				x = nx+langeliu_dydis;
				z = nz;
			}break;
			case(4):{
				x = nx+langeliu_dydis;
				z = nz+langeliu_dydis;
			}break;
		}
	}
	bool candraw(){
		if(getx()<0)return 0;
		if(gety()<0)return 0;
		if(getx()>ClientWidth)return 0;
		if(gety()>ClientHeight)return 0;
		return 1;
	}
};
struct trikampis : public daiktai {
	point* first;
	point* second;
	point* third;
	int color;
	trikampis(point* a, point* b, point* c,int d = 0xFFFFFFFF) : first(a), second(b),third(c),color(d){}
	void draw() override {
		if(first->candraw() == false)return;
		if(second->candraw() == false)return;
		if(third->candraw() == false)return;
		int a = first->getx();
		int b = first->gety();
		int c = second->getx();
		int d = second->gety();
		int e = third->getx();
		int f = third->gety();
		drawTriangle(a,b,c,d,e,f,color);
	}
	void draw_by_nr(int nr,int as_kelintas){
		if(as_kelintas == 1){
			first->update_by_nr(nr,1);
			second->update_by_nr(nr,2);
			third->update_by_nr(nr,3);
		}
		else{
			first->update_by_nr(nr,2);
			second->update_by_nr(nr,3);
			third->update_by_nr(nr,4);
		}
		
		draw();
	}
};
struct tile : public daiktai {
	trikampis* first;
	trikampis* second;
	int color;
	tile(trikampis* a, trikampis* b) : first(a), second(b){}
	tile(float a,float b,float c,int spalva,float dydis = 300){
		first = new trikampis(new point(a,b,c),new point(a,b,c+dydis),new point(a+dydis,b,c),spalva);
		second = new trikampis(new point(a+dydis,b,c),new point(a,b,c+dydis),new point(a+dydis,b,c+dydis),spalva);
		color = spalva;
	}
	void draw() override {
		first->draw();
		second->draw();
	}
	void draw_by_nr(int nr){
		first->draw_by_nr(nr,1);
		second->draw_by_nr(nr,2);
	}
	void perkelt(int a,int b,int c){
		delete first;
		delete second;
		first = new trikampis(new point(a,b,c),new point(a,b,c+dydis),new point(a+dydis,b,c),color);
		second = new trikampis(new point(a+dydis,b,c),new point(a,b,c+dydis),new point(a+dydis,b,c+dydis),color);
	}
};

struct jumpas;
struct zaidejas;
struct laukelis{
	int numeris;
	int spalva = rand()*rand();
	//daiktai* jump = 0;
	jumpas* jump = 0;
	int kurjumpas = -1;
	tile* langelis = 0;
	point* label = 0;
	zaidejas* jaustovi = 0;
	laukelis(int spalvagauta,int kurjumpasvar,int nr){
		numeris = nr;
		kurjumpas = kurjumpasvar;
		if(spalvagauta != -1)spalva = spalvagauta;
		langelis = new tile(0,0,0,spalva);
		label = new point(0,0,0,0);
		//jump = new jump(this,);
	}
};

struct jumpas : public daiktai{
	laukelis* is = 0;
	laukelis* i = 0;
	trikampis* modelis = 0;
	bool argyvatea;
	jumpas(laukelis* iskur,laukelis* ikur,bool argyvate){
		is = iskur;
		i = ikur;
		argyvatea = argyvate;
		if(argyvate){
			modelis = new trikampis(new point(0,0,0),new point(0,0,0),new point(0,0,0),0xFF00FF00);
		}
		else{
			
		}
	}
	void draw() override{
		if(argyvatea){
			delete modelis;
			modelis = new trikampis(new point(is->label->x-65,-50,is->label->z),new point(is->label->x+65,-50,is->label->z),new point(i->label->x,-50,i->label->z),0xFF00FF00);
			modelis->draw();
			//extra
			auto dantis = new trikampis(new point(is->label->x-45,-50,is->label->z),new point(is->label->x-35,-50,is->label->z),new point(is->label->x-40,-50,is->label->z-40),0xFFFFFFFF);
			dantis->draw();
			delete dantis;
			dantis = new trikampis(new point(is->label->x+45,-50,is->label->z),new point(is->label->x+35,-50,is->label->z),new point(is->label->x+40,-50,is->label->z-40),0xFFFFFFFF);
			dantis->draw();
			delete dantis;
			dantis = new trikampis(new point(is->label->x-25,-50,is->label->z+15),new point(is->label->x-15,-50,is->label->z+15),new point(is->label->x-20,-50,is->label->z+30),0xFF000000);
			dantis->draw();
			delete dantis;
			dantis = new trikampis(new point(is->label->x+25,-50,is->label->z+15),new point(is->label->x+15,-50,is->label->z+15),new point(is->label->x+20,-50,is->label->z+30),0xFF000000);
			dantis->draw();
			delete dantis;
			dantis = new trikampis(new point(is->label->x+5,-50,is->label->z),new point(is->label->x-5,-50,is->label->z),new point(is->label->x,-50,is->label->z-50),0xFFFF0000);
			dantis->draw();
			delete dantis;
		}
		else{
			delete modelis;
			modelis = new trikampis(new point(is->label->x-100,-50,is->label->z),new point(is->label->x-60,-50,is->label->z),new point(i->label->x-60,-50,i->label->z),0xFF964B00);
			modelis->draw();
			delete modelis;
			modelis = new trikampis(new point(is->label->x-100,-50,is->label->z),new point(i->label->x-60,-50,i->label->z),new point(i->label->x-100,-50,i->label->z),0xFF964B00);
			modelis->draw();
			
			delete modelis;
			modelis = new trikampis(new point(is->label->x+100,-50,is->label->z),new point(is->label->x+60,-50,is->label->z),new point(i->label->x+60,-50,i->label->z),0xFF964B00);
			modelis->draw();
			delete modelis;
			modelis = new trikampis(new point(is->label->x+100,-50,is->label->z),new point(i->label->x+60,-50,i->label->z),new point(i->label->x+100,-50,i->label->z),0xFF964B00);
			modelis->draw();
		}
	}
};

std::list<laukelis*> lenta;
struct zaidejas:public daiktai{
	trikampis* first = 0;
	trikampis* second = 0;
	point* label = 0;
	laukelis* grindys = 0;
	std::list<laukelis*>::iterator it;
	int manonumeris = 1;
	int manospalva = 0;
	int dareit = 0;
	zaidejas(int kelintas){
		manonumeris = kelintas;
		manospalva = rand()*rand();
		first = new trikampis(new point(50,0,100),new point(100,0,50),new point(50,-150,50),manospalva);
		second = new trikampis(new point(50,0,100),new point(0,0,50),new point(50,-150,50),manospalva-25);
		label = new point(50,-150,50,kelintas);
	}
	void draw() override{
		first->draw();
		second->draw();
		label->draw();
	}
	void perkurt(){
		delete first;
		delete second;
		delete label;
		
		int nr = grindys->numeris;
		
		int nz = -(langeliu_dydis*(nr/prideti_eile_kas));
		int nx;
		if(nr/prideti_eile_kas %2 == 1)nx = -(langeliu_dydis*((prideti_eile_kas-1)-nr%prideti_eile_kas));
		else nx = -(langeliu_dydis*(nr%prideti_eile_kas));
		
		first = new trikampis(new point(nx+50,0,nz+100),new point(nx+100,0,nz+50),new point(nx+50,-150,nz+50),manospalva);
		second = new trikampis(new point(nx+50,0,nz+100),new point(nx+0,0,nz+50),new point(nx+50,-150,nz+50),manospalva-25);
		label = new point(nx+50,-150,nz+50,manonumeris);
	}
};

struct workspace{
	std::queue<zaidejas*> zaidejai;
	std::vector<daiktai*> objektai;
	int zaideju_skaicius = 1;
	bool galima_ridenti = 1;
	bool persokszaidejastuoj = 0;
	int kadaivykopaskutinisejimas = 0;
	void add(daiktai* ka){objektai.push_back(ka);}
	void draw(){
		int koks = 0;
		for(auto i : lenta)i->langelis->draw_by_nr(koks++);
		for(auto i : lenta)if(i->jump != 0)i->jump->draw();
		koks = 0;
		for(int i = 0;i<zaidejai.size();i++){
			zaidejai.front()->draw();
			zaidejai.push(zaidejai.front());
			zaidejai.pop();
		}
		for(auto i : lenta){
			i->label->draw_by_nr(koks);
			i->numeris = koks++;
		}
		for(auto i : objektai)i->draw();
	}
	void add_zaidejas(){if(zaideju_skaicius<3)zaideju_skaicius++;}
	void remove_zaidejas(){if(zaideju_skaicius>1)zaideju_skaicius--;}
	void spawn_players(){for(int i = 0;i<zaideju_skaicius;i++)zaidejai.push(new zaidejas(i));}
	void finish_jumps(){
		int koks = -1;
		for(auto i : lenta){
			koks++;
			if(i->kurjumpas <= -1)continue;
			std::list<laukelis*>::iterator it = lenta.begin();
			std::advance(it,i->kurjumpas);
			i->jump = new jumpas(i,*it,koks>i->kurjumpas);
			i->kurjumpas = -1;
		}
	}
	void ridenti(){
		if(!galima_ridenti)return;
		
		int isridenau = (rand() % 6 + 1);
		zaidejai.front()->dareit = isridenau;
		std::cout<<"isridenai "<<isridenau<<"\n";
		galima_ridenti = 0;
		if(zaidejai.front()->grindys == 0){
			zaidejai.front()->grindys = lenta.front();
			zaidejai.front()->it = lenta.begin();
		}
		else{
			if(zaidejai.front()->grindys->jaustovi == zaidejai.front())zaidejai.front()->grindys->jaustovi = 0;
		}
		kadaivykopaskutinisejimas = GetTickCount();
	}
	void eit(){
		if(galima_ridenti)return;
		if(GetTickCount()-500<kadaivykopaskutinisejimas)return;
		if(persokszaidejastuoj){
			if(zaidejai.front()->grindys->jump->argyvatea){
					while((*(--zaidejai.front()->it))->numeris!=zaidejai.front()->grindys->jump->i->numeris){}
					zaidejai.front()->grindys = (*(zaidejai.front()->it));
					zaidejai.front()->perkurt();
				}
				else{
					while((*(++zaidejai.front()->it))->numeris!=zaidejai.front()->grindys->jump->i->numeris){}
					zaidejai.front()->grindys = (*(zaidejai.front()->it));
					zaidejai.front()->perkurt();
				}
			galima_ridenti = 1;
			if(zaidejai.front()->grindys->jaustovi != 0){
					zaidejai.front()->grindys->jaustovi->grindys = *(--((zaidejai.front()->grindys->jaustovi->it)));
					zaidejai.front()->grindys->jaustovi->perkurt();
				}
				zaidejai.front()->grindys->jaustovi = zaidejai.front();
			zaidejai.push(zaidejai.front());
			zaidejai.pop();
			persokszaidejastuoj = 0;
			return;
		}
		zaidejai.front()->grindys = (*(++zaidejai.front()->it));
		if(zaidejai.front()->grindys == *(lenta.rbegin())){
			std::cout<<"laimejo zaidejas nr "<<zaidejai.front()->label->skaicius+1<<"\n";
			meniu = true;
			for(int i = 0;i<zaidejai.size();i++)zaidejai.pop();
			return;
		}
		zaidejai.front()->perkurt();
		zaidejai.front()->dareit--;
		if(zaidejai.front()->dareit == 0){
			if(zaidejai.front()->grindys->jump>0){
				persokszaidejastuoj = 1;
			}
			else{
				galima_ridenti = 1;
				if(zaidejai.front()->grindys->jaustovi != 0){
					zaidejai.front()->grindys->jaustovi->grindys = *(--((zaidejai.front()->grindys->jaustovi->it)));
					zaidejai.front()->grindys->jaustovi->perkurt();
				}
				zaidejai.front()->grindys->jaustovi = zaidejai.front();
				zaidejai.push(zaidejai.front());
				zaidejai.pop();
			}
		}
		kadaivykopaskutinisejimas = GetTickCount();
	}
	void pasiduodu(){
		for(int i = 0;i<zaidejai.size();i++)zaidejai.pop();
	}
};workspace ws;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_KEYDOWN:{
			switch (wParam) {
				case 'S':{cam.back=1;}break;
				case 'A':{cam.left=1;}break;
				case 'D':{cam.right=1;}break;
				case 'W':{cam.front=1;}break;
				case VK_SHIFT:{cam.down=1;}break;
				case VK_SPACE:{cam.up=1;}break;
				case VK_RETURN:{
					if(meniu){
						meniu = false;
						ws.spawn_players();
					}
					else{
						ws.ridenti();
					}
				}break;
				case VK_ADD:{ws.add_zaidejas();}break;
				case VK_SUBTRACT:{ws.remove_zaidejas();}break;
				case 'P':{
					std::string komanda;
					int arg;
					int arg2;
					std::cin>>komanda;
					if(komanda == "insert"){std::cin>>arg;
						std::list<laukelis*>::iterator it = lenta.begin();
						std::advance(it,arg-1);
						lenta.insert(it,new laukelis(rand()*rand(),-1,arg-1));
						std::cout<<"prideta naujas langelis numeriu "<<arg<<"\n";
					}
					if(komanda == "delete"){std::cin>>arg;
						std::list<laukelis*>::iterator it = lenta.begin();
						std::advance(it,arg-1);
						lenta.erase(it);
						std::cout<<"istrintas langelis numeriu "<<arg<<"\n";
					}
					if(komanda == "clear"){
						lenta.clear();
					}
					if(komanda == "pasiduodu"){
						meniu = true;
						ws.pasiduodu();
					}
					if(komanda == "iseiti"){
						running = false;
						PostQuitMessage(0);
					}
					if(komanda == "prideti"){std::cin>>arg;std::cin>>arg2;
						std::list<laukelis*>::iterator it = lenta.begin();
						std::advance(it,arg-1);
						
						std::list<laukelis*>::iterator it2 = lenta.begin();
						std::advance(it2,arg2-1);
						(*it)->jump = new jumpas(*it,*it2,arg>arg2);
					}
				}break;
			}}break;
		case WM_KEYUP:{
			switch (wParam) {
				case 'S':{cam.back=0;}break;
				case 'A':{cam.left=0;}break;
				case 'D':{cam.right=0;}break;
				case 'W':{cam.front=0;}break;
				case VK_SHIFT:{cam.down=0;}break;
				case VK_SPACE:{cam.up=0;}break;
			}}break;
		case WM_DESTROY:{
			running = false;
			PostQuitMessage(0);
			}break;
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	RECT monitor;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &monitor, 0);//GetWindowRect(GetDesktopWindow(), &monitor);
    ClientWidth = monitor.right - monitor.left;
    ClientHeight = monitor.bottom - monitor.top;
	POINT pele;float pele2;
	SetCursorPos(ClientWidth/2, ClientHeight/2);
	GetCursorPos(&pele);
	pele2 = pele.y;

	WNDCLASS wc = {0};
	MSG msg;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(0, "MyWindowClass", "Gyvates ir kopecios", WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0,ClientWidth,ClientHeight, 0, 0, hInstance, 0);
	ShowWindow(hwnd, SW_MAXIMIZE);
	
	memory = malloc(ClientWidth * ClientHeight * 4);
	
	BITMAPINFO BitmapInfo;
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = ClientWidth;
    BitmapInfo.bmiHeader.biHeight = ClientHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
	
	HDC HDCa = GetDC(hwnd);
	MSG Message;
	ShowCursor(FALSE);
	
	Json::Value root;
	std::ifstream failas("settings.json");
	failas>>root;
	const Json::Value plugins = root["laukeliai"];
	for (int i = 0;i<plugins.size();++i){
		laukelis* tailas = new laukelis(plugins[i]["spalva"].asInt(),plugins[i]["jump"].asInt()-1,i);
		lenta.push_back(tailas);
	}
	ws.finish_jumps();
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, ("meniu.bmp"), IMAGE_BITMAP, ClientWidth, ClientHeight, LR_LOADFROMFILE);
	
	while(running){while(GetTickCount() != cam.frame){
		while(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
		
		if(!cam.locked){
			GetCursorPos(&pele);
			//cam.roty += (ClientWidth/2-pele.x)*cam.mousespeed;
			cam.rotz -= ((pele2-pele.y))*cam.mousespeed;
			pele2 = pele.y;
			if(cam.rotz > 0)cam.rotz = 0;
			else if(cam.rotz < -0.13)cam.rotz = -0.13;
			//if (GetForegroundWindow() == hwnd)SetCursorPos(ClientWidth/2, ClientHeight/2);
			if(cam.down){cam.y-=cam.speed;}
			if(cam.up){cam.y+=cam.speed;}
			if(cam.left){cam.x-=cam.speed*cos(cam.roty);cam.z+=cam.speed*sin(cam.roty);}
			if(cam.right){cam.x+=cam.speed*cos(cam.roty);cam.z-=cam.speed*sin(cam.roty);}
			if(cam.front){cam.z+=cam.speed*cos(cam.roty);cam.x+=cam.speed*sin(cam.roty);}
			if(cam.back){cam.z-=cam.speed*cos(cam.roty);cam.x-=cam.speed*sin(cam.roty);}
		}
		if(meniu){
			for(int x = 0;x<ClientWidth;x++){for(int y = 0;y<ClientHeight;y++){DrawPixel(x, y,0xFF333333);}}
			GetBitmapBits(hBitmap, ClientWidth * ClientHeight * 4, memory);
			for(int i = ClientWidth/64*41;i<ClientWidth/64*42;i++){
				for(int ii = ClientHeight/7;ii<ClientHeight/4;ii++){
					DrawPixel(i, ii,0xFF000000);
				}
			}
			if(ws.zaideju_skaicius>=2){
				for(int i = ClientWidth/64*43;i<ClientWidth/64*44;i++){
				for(int ii = ClientHeight/7;ii<ClientHeight/4;ii++){
					DrawPixel(i, ii,0xFF000000);
				}
			}
			}
			if(ws.zaideju_skaicius>=3){
				for(int i = ClientWidth/64*45;i<ClientWidth/64*46;i++){
				for(int ii = ClientHeight/7;ii<ClientHeight/4;ii++){
					DrawPixel(i, ii,0xFF000000);
				}
			}
			}
		}else{
			for(int x = 0;x<ClientWidth;x++){for(int y = 0;y<ClientHeight;y++){DrawPixel(x, y,0xFF000002);}}
			ws.eit();
			ws.draw();
		}
		StretchDIBits(HDCa,0,0,ClientWidth,ClientHeight,0,0,ClientWidth,ClientHeight,memory,&BitmapInfo,DIB_RGB_COLORS,SRCCOPY);
		cam.frame = GetTickCount();
		//Sleep(999999999);
		}Sleep(7);}
	return msg.wParam;
}
