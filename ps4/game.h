#pragma once
#include <SDL2/SDL.h>
#include <cmath>

static const float SC=1.8f,OX=240.0f;
struct Rc{float x,y,w,h;};
struct Room{float x,y,w,h;int r,g,b;};
static const Rc WALLS[13]={{0,0,800,10},{0,590,800,10},{0,0,10,600},{790,0,10,600},{295,0,10,80},{295,150,10,300},{295,520,10,80},{0,195,100,10},{180,195,120,10},{0,395,100,10},{180,395,120,10},{300,345,200,10},{600,345,200,10}};
static const Rc FURN[7]={{470,440,120,70},{400,30,140,50},{740,380,40,60},{20,420,50,30},{680,14,70,16},{10,320,60,65},{20,510,100,70}};
static const Room ROOMS[5]={{0,0,300,200,141,110,99},{0,200,300,200,128,203,196},{0,400,300,200,159,168,218},{300,0,500,350,188,170,164},{300,350,500,250,255,224,178}};
static const float PATH[8][2]={{680,250},{550,250},{550,390},{680,390},{680,560},{680,390},{550,390},{550,250}};
struct Input{float jx,jy;bool cross,options;float axis[4];unsigned btn;bool pad;};
struct Food{float x,y;int got;};
static Food food[5];
static float dogx,dogy,ddx,ddy,anx,seen,panic,momx,momy,momdir;
static int momi,stolen,day;
static SDL_Surface *gScreen=NULL,*gBg=NULL;
static bool gNoBg=false;

static void newDay(){
 dogx=60;dogy=100;ddx=0;ddy=1;anx=10;seen=0;panic=0;stolen=0;
 momx=680;momy=250;momi=1;momdir=0;
 Food f[5]={{540,425,0},{640,485,0},{150,470,0},{430,70,0},{350,500,0}};
 for(int i=0;i<5;i++)food[i]=f[i];
}
static void gameInit(){day=0;newDay();}

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

static void gameUpdate(float dt,const Input&in){
 if(in.options){day=0;newDay();}
 float mag=sqrtf(in.jx*in.jx+in.jy*in.jy);if(mag>1)mag=1;
 bool sneak=mag>0.1f&&mag<0.55f;
 if(panic>0)panic-=dt;
 if(mag>0.1f){
  ddx=in.jx/mag;ddy=in.jy/mag;
  float sp=(sneak?55.f:130.f)*(panic>0?0.5f:1.f)*dt;
  float nx=dogx+ddx*sp,ny=dogy+ddy*sp;
  if(!hitAt(nx,dogy))dogx=nx;
  if(!hitAt(dogx,ny))dogy=ny;
 }
 float tx=PATH[momi][0]-momx,ty=PATH[momi][1]-momy,md=sqrtf(tx*tx+ty*ty);
 if(md<3)momi=(momi+1)%8;else{momdir=atan2f(ty,tx);momx+=tx/md*55.f*dt;momy+=ty/md*55.f*dt;}
 if(seeing(sneak?110.f:180.f)){seen+=dt;if(seen>0.6f){seen=0;anx+=15;dogx=60;dogy=100;}}
 else{seen-=dt;if(seen<0)seen=0;}
 anx+=1.2f*dt;
 if(in.cross){
  for(int i=0;i<5;i++){
   float dx=dogx-food[i].x,dy=dogy-food[i].y;
   if(!food[i].got&&sqrtf(dx*dx+dy*dy)<30){food[i].got=1;stolen++;anx-=25;break;}
  }
 }
 if(anx>=100){anx=60;panic=6;}
 if(anx<0)anx=0;
 if(stolen>=3){int d=day+1;newDay();day=d;}
}

static void px(SDL_Renderer*r,int x,int y,int w,int h,int R,int G,int B,int A=255){
 SDL_SetRenderDrawColor(r,R,G,B,A);SDL_Rect q={x,y,w,h};SDL_RenderFillRect(r,&q);
}
static void fr(SDL_Renderer*r,float x,float y,float w,float h,int R,int G,int B,int A=255){
 px(r,(int)(OX+x*SC),(int)(y*SC),(int)(w*SC+1),(int)(h*SC+1),R,G,B,A);
}
static void disc(SDL_Renderer*r,float cx,float cy,float rad,int R,int G,int B){
 float X=OX+cx*SC,Y=cy*SC,rr=rad*SC;
 SDL_SetRenderDrawColor(r,R,G,B,255);
 for(int dy=-(int)rr;dy<=(int)rr;dy++){
  float h=sqrtf(rr*rr-(float)dy*dy);
  SDL_Rect q={(int)(X-h),(int)(Y+dy),(int)(2*h)+1,1};SDL_RenderFillRect(r,&q);
 }
}

static void drawStatic(SDL_Renderer*r){
 px(r,0,0,1920,1080,30,30,34);
 for(int i=0;i<5;i++)fr(r,ROOMS[i].x,ROOMS[i].y,ROOMS[i].w,ROOMS[i].h,ROOMS[i].r,ROOMS[i].g,ROOMS[i].b);
 for(int i=0;i<13;i++)fr(r,WALLS[i].x,WALLS[i].y,WALLS[i].w,WALLS[i].h,78,52,46);
 for(int i=0;i<7;i++)fr(r,FURN[i].x,FURN[i].y,FURN[i].w,FURN[i].h,121,85,72);
}

static void gameDraw(SDL_Renderer*r,const Input&in){
 if(!gBg&&!gNoBg&&gScreen){
  gBg=SDL_ConvertSurface(gScreen,gScreen->format,0);
  if(gBg){
   SDL_Renderer*br=SDL_CreateSoftwareRenderer(gBg);
   if(br){drawStatic(br);SDL_DestroyRenderer(br);}
   else{SDL_FreeSurface(gBg);gBg=NULL;gNoBg=true;}
  }else gNoBg=true;
 }
 if(gBg&&gScreen)SDL_BlitSurface(gBg,NULL,gScreen,NULL);
 else drawStatic(r);

 for(int i=0;i<5;i++)if(!food[i].got)disc(r,food[i].x,food[i].y,7,229,57,53);

 bool sneak=sqrtf(in.jx*in.jx+in.jy*in.jy)<0.55f;
 float range=(sneak?110.f:180.f)*SC;
 float mx=OX+momx*SC,my=momy*SC;
 float dirx=cosf(momdir),diry=sinf(momdir),ck=cosf(0.6f);
 SDL_SetRenderDrawColor(r,255,seen>0?40:235,seen>0?40:59,80);
 int R=(int)range;
 for(int by=-R;by<=R;by+=3){
  int x0=1<<30,x1=-(1<<30);
  for(int bx=-R;bx<=R;bx+=6){
   float d=sqrtf((float)(bx*bx+by*by));
   if(d>range||d<1)continue;
   if(bx*dirx+by*diry>=d*ck){if(bx<x0)x0=bx;if(bx>x1)x1=bx;}
  }
  if(x0<=x1){SDL_Rect q={(int)mx+x0,(int)my+by,x1-x0+6,3};SDL_RenderFillRect(r,&q);}
 }

 disc(r,momx,momy,12,236,64,122);
 float sh=panic>0?sinf(SDL_GetTicks()/30.f)*2:0;
 disc(r,dogx+sh,dogy,10,10,10,10);
 disc(r,dogx+sh+ddx*9,dogy+ddy*9,6,10,10,10);
 disc(r,dogx+sh+ddx*4,dogy+ddy*4,3.5f,245,245,245);

 px(r,20,1030,404,28,0,0,0);
 px(r,24,1034,(int)(anx<100?anx*4:400),20,anx>70?244:255,anx>70?67:179,anx>70?54:0);
 for(int i=0;i<day&&i<20;i++)px(r,20+i*26,990,20,20,255,213,79);
 for(int i=0;i<5;i++)px(r,20+i*26,950,20,20,food[i].got?60:229,food[i].got?60:57,food[i].got?60:53);
 for(int i=0;i<20;i++){bool on=(in.btn>>i)&1;px(r,1500+i*18,20,14,14,on?255:70,on?255:70,on?255:70);}
 for(int i=0;i<4;i++){
  px(r,1500,50+i*18,360,12,50,50,50);
  px(r,1680+(int)(in.axis[i]*170),50+i*18,8,12,100,200,255);
 }
 if(!in.pad)px(r,1500,140,40,40,255,0,0);
}
