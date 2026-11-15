#pragma once
static const float SC=1.8f,OX=240.0f;
static SDL_Surface *gScreen=NULL,*gBg=NULL;
static bool gNoBg=false;
static const unsigned short DG[10]={0x7B6F,0x2C97,0x73E7,0x73CF,0x5BC9,0x79CF,0x79EF,0x7249,0x7BEF,0x7BCF};

static void px(SDL_Renderer*r,int x,int y,int w,int h,int R,int G,int B,int A=255){
 SDL_SetRenderDrawColor(r,R,G,B,A);SDL_Rect q={x,y,w,h};SDL_RenderFillRect(r,&q);
}
static void fr(SDL_Renderer*r,float x,float y,float w,float h,int R,int G,int B,int A=255){
 px(r,(int)(OX+x*SC),(int)(y*SC),(int)(w*SC+1),(int)(h*SC+1),R,G,B,A);
}
static void disc(SDL_Renderer*r,float cx,float cy,float rad,int R,int G,int B,int A=255){
 float X=OX+cx*SC,Y=cy*SC,rr=rad*SC;
 SDL_SetRenderDrawColor(r,R,G,B,A);
 for(int dy=-(int)rr;dy<=(int)rr;dy++){
  float h=sqrtf(rr*rr-(float)dy*dy);
  SDL_Rect q={(int)(X-h),(int)(Y+dy),(int)(2*h)+1,1};SDL_RenderFillRect(r,&q);
 }
}
static void digit(SDL_Renderer*r,int x,int y,int d,int s,int R,int G,int B){
 for(int row=0;row<5;row++)for(int col=0;col<3;col++)
  if((DG[d]>>(14-(row*3+col)))&1)px(r,x+col*s,y+row*s,s,s,R,G,B);
}
static void number(SDL_Renderer*r,int x,int y,int n,int s,int R,int G,int B){
 if(n<0)n=0;
 char buf[12];snprintf(buf,sizeof(buf),"%d",n);
 for(int i=0;buf[i];i++)digit(r,x+i*4*s,y,buf[i]-'0',s,R,G,B);
}
static bool blinkOn(){return (SDL_GetTicks()/200)%2==1;}

static void drawStatic(SDL_Renderer*r){
 px(r,0,0,1920,1080,30,30,34);
 for(int i=0;i<5;i++)fr(r,ROOMS[i].x,ROOMS[i].y,ROOMS[i].w,ROOMS[i].h,ROOMS[i].r,ROOMS[i].g,ROOMS[i].b);
 for(int i=0;i<13;i++)fr(r,WALLS[i].x,WALLS[i].y,WALLS[i].w,WALLS[i].h,78,52,46);
 for(int i=0;i<7;i++)fr(r,FURN[i].x,FURN[i].y,FURN[i].w,FURN[i].h,121,85,72);
}

static void drawMenu(SDL_Renderer*r){
 static const int C[3][3]={{102,187,106},{255,213,79},{66,165,245}};
 px(r,0,0,1920,1080,0,0,0,200);
 px(r,560,200,800,680,40,44,60);
 number(r,600,230,lvl,10,255,255,255);
 px(r,600,330,720,24,0,0,0);
 px(r,602,332,(int)(716.f*xp/(lvl*40)),20,66,165,245);
 for(int i=0;i<sp&&i<12;i++)px(r,600+i*40,380,30,30,255,213,79);
 for(int i=0;i<3;i++){
  int y=450+i*110;
  if(i==menuSel)px(r,590,y-10,740,100,255,255,255);
  px(r,596,y-4,728,88,50,54,72);
  px(r,620,y+10,60,60,C[i][0],C[i][1],C[i][2]);
  for(int j=0;j<5;j++){
   bool on=j<sk[i];
   px(r,720+j*100,y+14,80,52,on?C[i][0]:70,on?C[i][1]:70,on?C[i][2]:80);
  }
 }
 number(r,600,790,trust,6,255,255,255);
 number(r,900,790,day,6,255,213,79);
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

 bool bp=evType==1&&blinkOn(),bd=evType==2&&blinkOn(),bw=evType==3&&blinkOn();
 fr(r,140,90,20,20,bp?244:51,bp?67:51,bp?54:51);
 fr(r,0,40,10,40,bd?244:93,bd?67:64,bd?54:55);
 fr(r,590,0,60,10,bw?244:129,bw?67:212,bw?54:250);

 for(int i=0;i<5;i++)if(!food[i].got){
  if(sk[1]>0&&dist(dogx,dogy,food[i].x,food[i].y)<60+50*sk[1])disc(r,food[i].x,food[i].y,16,255,255,0,100);
  disc(r,food[i].x,food[i].y,7,229,57,53);
 }
 if(!toyCarried)fr(r,toyx-10,toyy-3,20,6,67,160,71);

 float range=viewRange(sneakNow)*SC;
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

 disc(r,catx-6,caty-9,4,142,154,166);
 disc(r,catx+6,caty-9,4,142,154,166);
 disc(r,catx,caty,9,142,154,166);
 if(catSt==1)disc(r,catx,caty-20,3,255,255,255);

 float sh=panic>0?sinf(SDL_GetTicks()/30.f)*2:0;
 disc(r,dogx+sh,dogy,10,10,10,10);
 disc(r,dogx+sh+ddx*9,dogy+ddy*9,6,10,10,10);
 disc(r,dogx+sh+ddx*4,dogy+ddy*4,3.5f,245,245,245);
 if(toyCarried)fr(r,dogx+ddx*16-10,dogy+ddy*16-3,20,6,67,160,71);

 number(r,20,905,day,5,255,255,255);
 for(int i=0;i<5;i++)px(r,20+i*26,950,20,20,food[i].got?60:229,food[i].got?60:57,food[i].got?60:53);
 for(int i=0;i<sp&&i<12;i++)px(r,20+i*22,980,18,18,255,213,79);
 px(r,20,1004,404,16,0,0,0);
 px(r,22,1006,(int)(400.f*xp/(lvl*40)),12,66,165,245);
 number(r,440,1000,lvl,5,255,255,255);
 px(r,20,1030,404,28,0,0,0);
 px(r,24,1034,(int)(anx<100?anx*4:400),20,anx>70?244:255,anx>70?67:179,anx>70?54:0);
 if(!in.pad)px(r,1500,20,40,40,255,0,0);

 if(fxT>0){
  int c=fx;
  int R2=c==1?100:(c==2?240:255),G2=c==1?220:(c==2?60:215),B2=c==1?100:(c==2?60:0);
  px(r,0,0,1920,14,R2,G2,B2,200);px(r,0,1066,1920,14,R2,G2,B2,200);
  px(r,0,0,14,1080,R2,G2,B2,200);px(r,1906,0,14,1080,R2,G2,B2,200);
 }
 if(menuOpen)drawMenu(r);
}
