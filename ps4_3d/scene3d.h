#include <SDL2/SDL.h>
#include <sstream>
#include "logic.h"
#include "render.h"
#include "family.h"
#include "quests.h"
#include "render3d.h"
#include "scene3d.h"
std::stringstream debugLogStream;

static SDL_Surface* lowSurf=NULL;
static int fpsV=0,msR=0,msP=0;

static void present3D(){
 SDL_PixelFormat* f=gScreen->format;
 int sc=gScreen->w/RW;
 if(f->BitsPerPixel==32&&sc>=1&&RH*sc<=gScreen->h){
  Uint32 rs=f->Rshift,gs=f->Gshift,bs=f->Bshift,am=f->Amask;
  Uint32* dst=(Uint32*)gScreen->pixels;
  int pitch=gScreen->pitch/4;
  for(int y=0;y<RH;y++){
   Uint32* row=dst+(y*sc)*pitch;
   const uint32_t* src=fbuf+y*RW;
   for(int x=0;x<RW;x++){
    uint32_t c=src[x];
    Uint32 p=(((c>>16)&255)<<rs)|(((c>>8)&255)<<gs)|((c&255)<<bs)|am;
    for(int k=0;k<sc;k++)row[x*sc+k]=p;
   }
   for(int k=1;k<sc;k++)memcpy(row+k*pitch,row,gScreen->w*4);
  }
  return;
 }
 if(!lowSurf)lowSurf=SDL_CreateRGBSurfaceFrom(fbuf,RW,RH,32,RW*4,0x00FF0000,0x0000FF00,0x000000FF,0);
 if(lowSurf)SDL_BlitScaled(lowSurf,NULL,gScreen,NULL);
}

static void hud3D(SDL_Renderer*r,const Input&in){
 number(r,80,865,day,5,255,255,255);
 for(int i=0;i<5;i++)px(r,80+i*26,910,20,20,food[i].got?60:229,food[i].got?60:57,food[i].got?60:53);
 for(int i=0;i<sp&&i<12;i++)px(r,80+i*22,940,18,18,255,213,79);
 px(r,80,964,404,16,0,0,0);
 px(r,82,966,(int)(400.f*xp/(lvl*40)),12,66,165,245);
 number(r,500,960,lvl,5,255,255,255);
 px(r,80,990,404,28,0,0,0);
 px(r,84,994,(int)(anx<100?anx*4:400),20,anx>70?244:255,anx>70?67:179,anx>70?54:0);
 px(r,500,910,204,14,0,0,0);
 px(r,502,912,tD*2,10,63,106,181);
 px(r,500,935,204,14,0,0,0);
 px(r,502,937,tS*2,10,255,202,40);
 int idx=(day-1)%7;
 number(r,100,80,idx+1,6,255,213,79);
 int cr=70,cg=70,cb=80;
 if(qFinished){cr=102;cg=187;cb=106;}
 else if(idx==6&&qCaught){cr=244;cg=67;cb=54;}
 px(r,150,80,30,30,cr,cg,cb);
 if(idx==2&&!qFinished)for(int i=0;i<3;i++)px(r,100+i*26,126,20,12,i<qReact?102:70,i<qReact?187:70,i<qReact?106:80);
 number(r,100,160,qDone,4,255,255,255);
 number(r,1650,60,fpsV,4,255,255,255);
 number(r,1650,100,msR,4,102,187,106);
 number(r,1650,140,msP,4,66,165,245);
 if(!in.pad)px(r,1500,60,40,40,255,0,0);
 if(fxT>0){
  int c=fx;
  int R2=c==1?100:(c==2?240:255),G2=c==1?220:(c==2?60:215),B2=c==1?100:(c==2?60:0);
  px(r,0,0,1920,14,R2,G2,B2,200);px(r,0,1066,1920,14,R2,G2,B2,200);
  px(r,0,0,14,1080,R2,G2,B2,200);px(r,1906,0,14,1080,R2,G2,B2,200);
 }
 if(menuOpen)drawMenu(r);
}

int main(int argc,char* args[]){
 if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)!=0)return 1;
 SDL_Window* window=SDL_CreateWindow("main",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1920,1080,0);
 if(!window)return 1;
 SDL_Surface* surf=SDL_GetWindowSurface(window);
 gScreen=surf;
 SDL_Renderer* rd=SDL_CreateSoftwareRenderer(surf);
 if(!rd)return 1;
 SDL_SetRenderDrawBlendMode(rd,SDL_BLENDMODE_BLEND);
 SDL_Joystick* pad=NULL;
 unsigned prev=0;
 Uint32 last=SDL_GetTicks(),retry=0,fpsT=0,sumR=0,sumP=0;
 int fpsN=0;
 gameInit();
 for(;;){
  Uint32 now=SDL_GetTicks();
  float dt=(now-last)/1000.f;last=now;if(dt>0.05f)dt=0.05f;
  SDL_Event e;while(SDL_PollEvent(&e)){}
  SDL_JoystickUpdate();
  if(!pad&&now>retry){retry=now+1000;if(SDL_NumJoysticks()>0)pad=SDL_JoystickOpen(0);}
  Input in={};
  in.pad=pad!=NULL;
  if(pad){
   for(int i=0;i<4;i++)in.axis[i]=SDL_JoystickGetAxis(pad,i)/32768.0f;
   for(int i=0;i<20;i++)if(SDL_JoystickGetButton(pad,i))in.btn|=(1u<<i);
   in.jx=in.axis[0];in.jy=in.axis[1];
   if(fabsf(in.jx)<0.2f)in.jx=0;
   if(fabsf(in.jy)<0.2f)in.jy=0;
   if(in.btn&(1u<<15))in.jx=-1;
   if(in.btn&(1u<<16))in.jx=1;
   if(in.btn&(1u<<13))in.jy=-1;
   if(in.btn&(1u<<14))in.jy=1;
   unsigned pressed=in.btn&~prev;
   in.cross=(pressed&(1u<<0))!=0;
   in.circle=(pressed&(1u<<1))!=0;
   in.square=(pressed&(1u<<2))!=0;
   in.options=(pressed&(1u<<9))!=0;
   in.up=(pressed&(1u<<13))!=0;
   in.down=(pressed&(1u<<14))!=0;
   prev=in.btn;
  }
  gameUpdate(dt,in);
  Uint32 ta=SDL_GetTicks();
  renderWorld3D(now/1000.f,dt);
  Uint32 tb=SDL_GetTicks();
  present3D();
  Uint32 tc=SDL_GetTicks();
  hud3D(rd,in);
  sumR+=tb-ta;sumP+=tc-tb;fpsN++;
  if(now-fpsT>=1000){fpsV=fpsN;msR=sumR/fpsN;msP=sumP/fpsN;fpsN=0;sumR=0;sumP=0;fpsT=now;}
  SDL_UpdateWindowSurface(window);
 }
 SDL_Quit();
 return 0;
}
