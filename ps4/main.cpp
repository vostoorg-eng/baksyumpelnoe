#include <SDL2/SDL.h>
#include <sstream>
#include "game.h"
std::stringstream debugLogStream;

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
 Uint32 last=SDL_GetTicks(),retry=0;
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
  gameDraw(rd,in);
  SDL_UpdateWindowSurface(window);
 }
 SDL_Quit();
 return 0;
}
