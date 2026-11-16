#pragma once
#include "render3d.h"
static void drawDog(float t,float dt){
 static float dA=1.5708f,px0=0,py0=0;
 static bool init=false;
 if(!init){px0=dogx;py0=dogy;init=true;}
 bool mv=fabsf(dogx-px0)+fabsf(dogy-py0)>0.05f;
 px0=dogx;py0=dogy;
 float target=atan2f(ddy,ddx),d=target-dA;
 d=atan2f(sinf(d),cosf(d));
 dA+=d*fminf(1.f,dt*12.f);
 float bob=mv?fabsf(sinf(t*10))*0.04f:0;
 float sh=panic>0?sinf(t*60)*0.03f:0;
 const uint32_t BK=0x141414,WH=0xf5f5f5;
 setM(dA,dogx*WK+sh,0,dogy*WK,1.0f);gYS=1;
 gAlpha=255;
 ell(0,0.03,0,0.6,0.01,0.34,0x1c1c26);
 gPy=bob;
 gYS=(sneakNow&&mv)?0.85f:1.0f;
 ell(0,0.52,0,0.45,0.26,0.25,BK);
 ell(-0.3,0.55,0,0.29,0.28,0.25,BK);
 ell(0.22,0.58,0,0.31,0.33,0.29,BK);
 ell(0.38,0.44,0,0.22,0.24,0.21,WH);
 ell(0.5,0.72,0,0.17,0.2,0.17,BK);
 ell(0.44,0.67,0,0.06,0.21,0.21,0xd32f2f);
 ell(0.66,0.86,0,0.21,0.19,0.22,BK);
 ell(0.84,0.80,0,0.13,0.09,0.1,0x3a3a3a);
 ell(0.95,0.84,0,0.04,0.035,0.05,0x050505);
 for(int s=-1;s<=1;s+=2){
  ell(0.79,0.93,s*0.12,0.035,0.035,0.03,0xd08a1e);
  ell(0.6,1.04,s*0.16,0.06,0.1,0.04,BK);
 }
 if(anx<35&&panic<=0)ell(0.88,0.71,0,0.05,0.015,0.035,0xf06292);
 for(int i=0;i<4;i++){
  float lx=i<2?0.34f:-0.3f,lz=(i%2?-1:1)*(i<2?0.17f:0.18f);
  uint32_t c=i<2?WH:BK;
  float sw=mv?sinf(t*10+(i%2?3.14159f:0)+(i>1?1.57f:0))*0.1f:0;
  ell(lx+sw,0.21,lz,0.075,0.22,0.075,c);
  ell(lx+sw+0.04f,0.05,lz,0.11,0.05,0.085,c);
 }
 float wag=sinf(t*(anx<50?14.f:6.f))*(anx<50?0.12f:0.04f);
 ell(-0.6,0.7,wag*0.4f,0.14,0.04,0.04,BK);
 ell(-0.72,0.78,wag,0.1,0.035,0.035,BK);
 gYS=1;
}

static void person(float x,float y,float dir,uint32_t shirt,uint32_t pants,uint32_t hair,float sc,float lift,float t,bool walk){
 setM(dir,x*WK,lift+(walk?fabsf(sinf(t*6))*0.03f:0),y*WK,sc);gYS=1;gAlpha=255;
 ell(0.02,0.0,0,0.35,0.01,0.28,0x1c1c26);
 ell(0,0.9,0,0.25,0.45,0.2,shirt);
 for(int s=-1;s<=1;s+=2)ell(walk?s*sinf(t*6)*0.12f:0,0.25,s*0.1,0.09,0.25,0.09,pants);
 ell(0,1.6,0,0.2,0.22,0.2,0xffcc99);
 ell(-0.03,1.7,0,0.21,0.14,0.21,hair);
 ell(0.2,1.58,0,0.04,0.04,0.04,0xe0a070);
}

static void drawCat(float t){
 static float cA=0,pcx=0,pcy=0;
 float dx=catx-pcx,dy=caty-pcy;
 if(dx*dx+dy*dy>0.01f)cA=atan2f(dy,dx);
 pcx=catx;pcy=caty;
 const uint32_t GR=0x8e9aa6;
 setM(cA,catx*WK,0,caty*WK,1.0f);gYS=1;gAlpha=255;
 ell(0,0.25,0,0.25,0.16,0.14,GR);
 ell(0.28,0.36,0,0.13,0.13,0.14,GR);
 for(int s=-1;s<=1;s+=2){
  ell(0.28,0.5,s*0.08,0.04,0.07,0.04,GR);
  ell(0.39,0.39,s*0.06,0.025,0.025,0.02,0xffb300);
 }
 ell(-0.35,0.4,0,0.18,0.03,0.03,GR);
 for(int i=0;i<4;i++)ell(i<2?0.15f:-0.15f,0.08,(i%2?-1:1)*0.08f,0.04,0.08,0.04,GR);
}

static void drawStaticScene(){
 for(int r=0;r<5;r++){
  const Room&R=ROOMS[r];
  for(int i=0;i*50<R.w;i++)for(int j=0;j*50<R.h;j++){
   float k=((i+j)&1)?0.88f:1.0f;
   uint32_t c=((unsigned)(R.r*k)<<16)|((unsigned)(R.g*k)<<8)|(unsigned)(R.b*k);
   float x0=(R.x+i*50)*WK,x1=(R.x+i*50+50)*WK,z0=(R.y+j*50)*WK,z1=(R.y+j*50+50)*WK;
   tri3(x0,0,z0,x1,0,z0,x1,0,z1,c);tri3(x0,0,z0,x1,0,z1,x0,0,z1,c);
  }
 }
 gbox(40,120,180,50,0.03f,0xa1574f,0.02f);
 gbox(520,120,200,150,0.03f,0x5c7a8c,0.02f);
 gbox(330,400,110,60,0.03f,0xc0a060,0.02f);
 gbox(130,440,140,60,0.03f,0x7e57a0,0.02f);
 for(int i=0;i<13;i++)gbox(WALLS[i].x,WALLS[i].y,WALLS[i].w,WALLS[i].h,0.95f,0x9c8472);
 gbox(470,440,120,70,0.12f,0x8d5a3a,0.78f);
 gbox(472,442,8,8,0.78f,0x6d4a2a);gbox(580,442,8,8,0.78f,0x6d4a2a);
 gbox(472,500,8,8,0.78f,0x6d4a2a);gbox(580,500,8,8,0.78f,0x6d4a2a);
 gbox(400,30,140,50,0.5f,0x5c6bc0);gbox(400,30,140,12,1.0f,0x3f51b5);
 gbox(740,380,40,60,2.0f,0xeceff1);
 gbox(20,420,50,30,0.8f,0x8d6e63);
 gbox(680,14,70,16,0.5f,0x5d4037);gbox(690,18,50,6,0.7f,0x111111,0.5f);
 gbox(20,510,100,70,0.4f,0x7986cb);gbox(24,514,24,62,0.15f,0xffffff,0.4f);
 gbox(730,540,30,30,0.6f,0x455a64);
}

static void renderWorld3D(float t,float dt){
 static float camx=0,camz=0;
 static bool ci=false;
 if(!ci){camx=dogx*WK;camz=dogy*WK;ci=true;}
 float k=fminf(1.f,dt*5.f);
 camx+=(dogx*WK-camx)*k;camz+=(dogy*WK-camz)*k;
 camLook(camx,6.6f,camz+4.9f,camx,0.5f,camz-1.0f);
 for(int i=0;i<RW*RH;i++){fbuf[i]=0x20202a;zbuf[i]=0;}
 gAlpha=255;gNoZ=false;gYS=1;gSc=1;
 bool blink=((int)(t*5))%2==1;
 drawStaticScene();
 gbox(10,320,60,65,0.1f,0xffffff,0);
 gbox(140,90,20,20,0.12f,(evType==1&&blink)?0xf44336:0x333333);
 gbox(10,40,4,40,1.2f,(evType==2||evType==4||evType==5)&&blink?0xf44336:0x5d4037);
 gbox(590,10,60,3,0.9f,(evType==3&&blink)?0xf44336:0x81d4fa,1.0f);
 gbox(440,240,20,20,0.45f,(evType==6&&blink)?0xf44336:0x1e88e5);
 static const float FH[5]={0.2f,0.2f,0.2f,0.65f,0.2f};
 for(int i=0;i<5;i++)if(!food[i].got){
  setM(t*2,food[i].x*WK,FH[i]+sinf(t*3+i)*0.05f,food[i].y*WK,1.0f);
  ell(0,0,0,0.14,0.11,0.14,0xe53935);
 }
 for(int i=0;i<2;i++)if(xf[i].got==0){
  setM(t*2,xf[i].x*WK,(i==0?0.75f:0.2f)+sinf(t*3+i)*0.05f,xf[i].y*WK,1.0f);
  ell(0,0,0,0.14,0.11,0.14,i==0?0x9ccc65:0xffb74d);
 }
 if(!toyCarried){setM(0,toyx*WK,0.06f,toyy*WK,1.0f);ell(0,0,0,0.25,0.05,0.05,0x43a047);}
 else{setM(atan2f(ddy,ddx),dogx*WK,0,dogy*WK,1.0f);ell(0.75,0.45,0,0.25,0.05,0.05,0x43a047);}
 person(momx,momy,momdir,0xec407a,0x37474f,0x5d4037,1.0f,0,t,true);
 person(dad.x,dad.y,dad.dir,0x3f6ab5,0x37474f,0x9e9e9e,1.0f,dad.st==0?0.3f:0,t,dad.st!=0&&dad.st!=2);
 person(son.x,son.y,son.dir,0xffca28,0x1565c0,0x3e2723,0.75f,0,t,true);
 if(evType==4||evType==5)person(22,60,0,0xff7043,0x263238,0x212121,1.0f,0,t,false);
 drawCat(t);
 drawDog(t,dt);
 sector(momx,momy,momdir,viewRange(sneakNow),0.6f,seen>0?0xff3030:0xffeb3b);
 sector(dad.x,dad.y,dad.dir,dadRange(),0.6f,dad.seen>0?0xff3030:0xffeb3b);
 sector(son.x,son.y,son.dir,sonRange(),0.8f,son.seen>0?0xff3030:0xffeb3b);
 gAlpha=90;gbox(10,320,60,65,2.0f,0x81d4fa);gAlpha=255;
 if(sk[1]>0){
  gAlpha=110;gNoZ=true;
  for(int i=0;i<5;i++)if(!food[i].got&&dist(dogx,dogy,food[i].x,food[i].y)<60+50*sk[1]){
   setM(0,food[i].x*WK,FH[i],food[i].y*WK,1.0f);ell(0,0,0,0.4,0.4,0.4,0xffff00);}
  for(int i=0;i<2;i++)if(xf[i].got==0&&dist(dogx,dogy,xf[i].x,xf[i].y)<60+50*sk[1]){
   setM(0,xf[i].x*WK,i==0?0.75f:0.2f,xf[i].y*WK,1.0f);ell(0,0,0,0.4,0.4,0.4,0xffff00);}
  gAlpha=255;gNoZ=false;
 }
}
