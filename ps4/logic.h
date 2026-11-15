#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define SAVEPATH "/data/baksyumpelnoe.sav"
struct Rc{float x,y,w,h;};
struct Room{float x,y,w,h;int r,g,b;};
static const Rc WALLS[13]={{0,0,800,10},{0,590,800,10},{0,0,10,600},{790,0,10,600},{295,0,10,80},{295,150,10,300},{295,520,10,80},{0,195,100,10},{180,195,120,10},{0,395,100,10},{180,395,120,10},{300,345,200,10},{600,345,200,10}};
static const Rc FURN[7]={{470,440,120,70},{400,30,140,50},{740,380,40,60},{20,420,50,30},{680,14,70,16},{10,320,60,65},{20,510,100,70}};
static const Room ROOMS[5]={{0,0,300,200,141,110,99},{0,200,300,200,128,203,196},{0,400,300,200,159,168,218},{300,0,500,350,188,170,164},{300,350,500,250,255,224,178}};
static const float PATH[8][2]={{680,250},{550,250},{550,390},{680,390},{680,560},{680,390},{550,390},{550,250}};
static const float EVPOS[3][2]={{150,100},{30,60},{620,40}};
struct Input{float jx,jy;bool cross,circle,square,options,up,down;float axis[4];unsigned btn;bool pad;};
struct Food{float x,y;int got;};

static Food food[5];
static float dogx,dogy,ddx,ddy,anx,seen,panic,momx,momy,momdir;
static float toyx,toyy,shakeCd,catx,caty,cattx,catty,catT,catCd,catFleeT,evT,evx,evy,evLeft,fxT;
static int momi,stolen,catSt,evType,fx;
static bool toyCarried,menuOpen,sneakNow;
static int lvl=1,xp=0,sp=0,sk[3]={0,0,0},trust=50,day=1,menuSel=0;

static void saveGame(){
 FILE*f=fopen(SAVEPATH,"w");if(!f)return;
 fprintf(f,"%d %d %d %d %d %d %d %d\n",lvl,xp,sp,sk[0],sk[1],sk[2],trust,day);
 fclose(f);
}
static void loadGame(){
 FILE*f=fopen(SAVEPATH,"r");if(!f)return;
 int a[8];
 if(fscanf(f,"%d %d %d %d %d %d %d %d",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7])==8){
  lvl=a[0]<1?1:a[0];xp=a[1]<0?0:a[1];sp=a[2]<0?0:a[2];
  for(int i=0;i<3;i++)sk[i]=a[3+i]<0?0:(a[3+i]>5?5:a[3+i]);
  trust=a[6]<0?0:(a[6]>100?100:a[6]);day=a[7]<1?1:a[7];
 }
 fclose(f);
}
static void setFx(int c){fx=c;fxT=0.6f;}
static void gain(int n){
 xp+=n;
 while(xp>=lvl*40){xp-=lvl*40;lvl++;sp++;setFx(3);}
 saveGame();
}
static float mult(){return 1.0f-0.12f*sk[2];}
static float viewRange(bool sneak){return (sneak?110.f:180.f)*(1.0f-0.1f*sk[0])*(trust>=60?0.85f:1.0f);}
static float dist(float ax,float ay,float bx,float by){return sqrtf((ax-bx)*(ax-bx)+(ay-by)*(ay-by));}

static void newDay(){
 dogx=60;dogy=100;ddx=0;ddy=1;anx=10;seen=0;panic=0;stolen=0;
 momx=680;momy=250;momi=1;momdir=0;
 Food f[5]={{540,425,0},{640,485,0},{150,470,0},{430,70,0},{350,500,0}};
 for(int i=0;i<5;i++)food[i]=f[i];
 toyx=200;toyy=300;toyCarried=false;shakeCd=0;
 catx=700;caty=150;cattx=700;catty=150;catT=3;catCd=15;catSt=0;catFleeT=0;
 evType=0;evT=22;
}
static void gameInit(){srand(SDL_GetTicks());loadGame();menuOpen=false;newDay();}

static bool hitAt(float x,float y){
 const float r=10;
 for(int k=0;k<20;k++){
  const Rc&q=k<13?WALLS[k]:FURN[k-13];
  float cx=fmaxf(q.x,fminf(x,q.x+q.w)),cy=fmaxf(q.y,fminf(y,q.y+q.h));
  if((x-cx)*(x-cx)+(y-cy)*(y-cy)<r*r)return true;
 }
 return false;
}
static bool inWall(float x,float y){
 for(int k=0;k<13;k++){const Rc&q=WALLS[k];if(x>q.x&&x<q.x+q.w&&y>q.y&&y<q.y+q.h)return true;}
 return false;
}
static bool seeing(float range){
 float dx=dogx-momx,dy=dogy-momy,d=sqrtf(dx*dx+dy*dy);
 if(d>range)return false;
 float a=atan2f(dy,dx)-momdir;a=atan2f(sinf(a),cosf(a));
 if(fabsf(a)>0.6f)return false;
 for(float s=0;s<d;s+=8)if(inWall(momx+dx*s/d,momy+dy*s/d))return false;
 return true;
}
static void moveTo(float&x,float&y,float tx,float ty,float s,float dt){
 float dx=tx-x,dy=ty-y,d=sqrtf(dx*dx+dy*dy);
 if(d>3){x+=dx/d*s*dt;y+=dy/d*s*dt;}
}
static void updateCat(float dt){
 catCd-=dt;
 if(catSt==0){
  catT-=dt;
  if(catT<=0){cattx=320+rand()%460;catty=20+rand()%560;catT=3+rand()%3;}
  moveTo(catx,caty,cattx,catty,40,dt);
  bool any=false;for(int i=0;i<5;i++)if(!food[i].got)any=true;
  if(catCd<=0&&any)catSt=1;
 }else if(catSt==1){
  int b=-1;float bd=1e9f;
  for(int i=0;i<5;i++)if(!food[i].got){float d=dist(catx,caty,food[i].x,food[i].y);if(d<bd){bd=d;b=i;}}
  if(b<0){catSt=0;catCd=20;}
  else{
   moveTo(catx,caty,food[b].x,food[b].y,85,dt);
   if(bd<10){food[b].got=2;anx+=8*mult();catSt=0;catCd=25;setFx(2);}
  }
 }else{
  catFleeT-=dt;
  float dx=catx-dogx,dy=caty-dogy,d=sqrtf(dx*dx+dy*dy);if(d<1)d=1;
  catx=fmaxf(20,fminf(780,catx+dx/d*130*dt));
  caty=fmaxf(20,fminf(580,caty+dy/d*130*dt));
  if(catFleeT<=0){catSt=0;catCd=12;}
 }
}

static void gameUpdate(float dt,const Input&in){
 if(fxT>0)fxT-=dt;
 if(in.options){menuOpen=!menuOpen;menuSel=0;if(!menuOpen)saveGame();}
 if(menuOpen){
  if(in.up&&menuSel>0)menuSel--;
  if(in.down&&menuSel<2)menuSel++;
  if(in.cross&&sp>0&&sk[menuSel]<5){sk[menuSel]++;sp--;saveGame();}
  return;
 }
 float wdt=dt*0.7f;
 float mag=sqrtf(in.jx*in.jx+in.jy*in.jy);if(mag>1)mag=1;
 sneakNow=mag>0.1f&&mag<0.55f;
 if(panic>0)panic-=wdt;
 if(mag>0.1f){
  ddx=in.jx/mag;ddy=in.jy/mag;
  float sp2=(sneakNow?55.f+8.f*sk[0]:130.f)*(panic>0?0.5f:1.f)*dt;
  float nx=dogx+ddx*sp2,ny=dogy+ddy*sp2;
  if(!hitAt(nx,dogy))dogx=nx;
  if(!hitAt(dogx,ny))dogy=ny;
 }
 if(toyCarried){toyx=dogx;toyy=dogy;anx-=3*wdt;}
 float tx=PATH[momi][0]-momx,ty=PATH[momi][1]-momy,md=sqrtf(tx*tx+ty*ty);
 if(md<3)momi=(momi+1)%8;else{momdir=atan2f(ty,tx);momx+=tx/md*55.f*wdt;momy+=ty/md*55.f*wdt;}
 if(seeing(viewRange(sneakNow))){
  seen+=wdt;
  if(seen>0.6f){seen=0;anx+=15*mult();trust=trust>5?trust-5:0;dogx=60;dogy=100;setFx(2);saveGame();}
 }else{seen-=wdt;if(seen<0)seen=0;}
 updateCat(wdt);
 evT-=wdt;
 if(evType==0&&evT<=0){evType=1+rand()%3;evx=EVPOS[evType-1][0];evy=EVPOS[evType-1][1];evLeft=14;}
 if(evType){
  evLeft-=wdt;anx+=4*mult()*wdt;
  if(evLeft<=0){anx+=10*mult();evType=0;evT=20+rand()%8;setFx(2);}
 }
 anx+=1.2f*mult()*wdt;
 if(shakeCd>0)shakeCd-=wdt;
 if(in.circle){
  bool used=false;
  if(evType&&dist(dogx,dogy,evx,evy)<90){evType=0;evT=20+rand()%8;anx-=14;gain(8);setFx(1);used=true;}
  if(catSt!=2&&dist(dogx,dogy,catx,caty)<110){catSt=2;catFleeT=5;gain(4);if(!used)setFx(1);}
 }
 if(in.cross){
  for(int i=0;i<5;i++){
   if(!food[i].got&&dist(dogx,dogy,food[i].x,food[i].y)<30){food[i].got=1;stolen++;anx-=25;gain(10);setFx(1);break;}
  }
 }
 if(in.square){
  if(!toyCarried&&dist(dogx,dogy,toyx,toyy)<30)toyCarried=true;
  else if(toyCarried&&shakeCd<=0){anx-=10;shakeCd=2;gain(2);setFx(1);}
 }
 if(anx>=100){anx=60;panic=6;}
 if(anx<0)anx=0;
 if(stolen>=3){
  day++;trust=trust+3>100?100:trust+3;gain(25);setFx(3);saveGame();newDay();
 }else{
  bool all=true;for(int i=0;i<5;i++)if(!food[i].got)all=false;
  if(all)newDay();
 }
}
