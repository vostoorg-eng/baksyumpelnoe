#pragma once
static const float DP[4][2]={{550,200},{550,330},{550,390},{705,415}};
static const float SEATX=490,SEATY=58;
static const float SPT[6][2]={{180,470},{330,485},{420,450},{420,550},{330,490},{200,550}};
struct Walker{float x,y,dir,seen,cd,t,fawn,pause;int st,i;};
static Walker dad,son;
static int tD=50,tS=50;
#define FAMSAVE "/data/baksyumpelnoe2.sav"

static void famSave(){
 FILE*f=fopen(FAMSAVE,"w");if(!f)return;
 fprintf(f,"%d %d\n",tD,tS);fclose(f);
}
static void famLoad(){
 FILE*f=fopen(FAMSAVE,"r");if(!f)return;
 int a,b;
 if(fscanf(f,"%d %d",&a,&b)==2){tD=a<0?0:(a>100?100:a);tS=b<0?0:(b>100?100:b);}
 fclose(f);
}
static float sitDir(){return atan2f(22.f-SEATY,715.f-SEATX);}
static void famReset(){
 dad.x=SEATX;dad.y=SEATY;dad.dir=sitDir();dad.seen=0;dad.cd=0;dad.t=20+rand()%10;dad.fawn=0;dad.pause=0;dad.st=0;dad.i=0;
 son.x=180;son.y=470;son.dir=0;son.seen=0;son.cd=0;son.t=0;son.fawn=0;son.pause=0;son.st=0;son.i=1;
}
static void famInit(){famLoad();famReset();}
static float dadRange(){return (sneakNow?80.f:150.f)*(1.0f-0.1f*sk[0])*(tD>=60?0.85f:1.0f);}
static float sonRange(){return (sneakNow?70.f:130.f)*(1.0f-0.1f*sk[0])*(tS>=60?0.85f:1.0f);}
static bool stepTo(Walker&w,float tx,float ty,float s,float dt){
 float dx=tx-w.x,dy=ty-w.y,d=sqrtf(dx*dx+dy*dy);
 if(d<4)return true;
 w.dir=atan2f(dy,dx);w.x+=dx/d*s*dt;w.y+=dy/d*s*dt;
 return false;
}
static bool famSees(const Walker&w,float range,float half){
 float dx=dogx-w.x,dy=dogy-w.y,d=sqrtf(dx*dx+dy*dy);
 if(d>range)return false;
 float a=atan2f(dy,dx)-w.dir;a=atan2f(sinf(a),cosf(a));
 if(fabsf(a)>half)return false;
 for(float s=0;s<d;s+=8)if(inWall(w.x+dx*s/d,w.y+dy*s/d))return false;
 return true;
}
static void scold(Walker&w,int*t,float a){
 w.seen=0;anx+=a*mult();*t=*t>4?*t-4:0;
 dogx=60;dogy=100;setFx(2);famSave();
}

static void famUpdate(float dt){
 if(menuOpen)return;
 if(catCd==15.0f&&evT==22.0f)famReset();
 float w=dt*0.7f;
 dad.cd-=w;son.cd-=w;
 if(dad.st==0){
  dad.dir=sitDir();dad.t-=w;
  if(dad.t<=0){dad.st=1;dad.i=0;}
 }else if(dad.st==1){
  if(stepTo(dad,DP[dad.i][0],DP[dad.i][1],50,w)){dad.i++;if(dad.i>=4){dad.st=2;dad.t=4;}}
 }else if(dad.st==2){
  dad.dir=0;dad.t-=w;
  if(dad.t<=0){dad.st=3;dad.i=2;}
 }else{
  if(dad.i>=0){if(stepTo(dad,DP[dad.i][0],DP[dad.i][1],50,w))dad.i--;}
  else{if(stepTo(dad,SEATX,SEATY,50,w)){dad.st=0;dad.t=20+rand()%15;}}
 }
 if(son.pause>0)son.pause-=w;
 else{
  if(stepTo(son,SPT[son.i][0],SPT[son.i][1],65,w)){
   if(son.i==3)son.pause=2;
   son.i=(son.i+1)%6;
  }
 }
 float dd=dist(dogx,dogy,dad.x,dad.y),sd=dist(dogx,dogy,son.x,son.y);
 if(dad.st==0&&dd<50){
  dad.seen=0;dad.fawn+=w;
  if(dad.fawn>=3&&dad.cd<=0){
   dad.fawn=0;dad.cd=12;tD=tD+3>100?100:tD+3;anx-=10;gain(4);
   if(tD>=60){anx-=10;gain(4);}
   setFx(1);famSave();
  }
 }else{
  dad.fawn-=w;if(dad.fawn<0)dad.fawn=0;
  if(famSees(dad,dadRange(),0.6f)){dad.seen+=w;if(dad.seen>0.6f)scold(dad,&tD,12);}
  else{dad.seen-=w;if(dad.seen<0)dad.seen=0;}
 }
 if(toyCarried&&sd<90){
  son.seen=0;
  if(sd<60&&son.cd<=0){
   son.cd=20;tS=tS+3>100?100:tS+3;anx-=15;gain(6);setFx(1);famSave();
  }
 }else if(famSees(son,sonRange(),0.8f)){
  son.seen+=w;if(son.seen>0.6f)scold(son,&tS,8);
 }else{son.seen-=w;if(son.seen<0)son.seen=0;}
 if(anx<0)anx=0;
 if(anx>=100){anx=60;panic=6;}
}

static void famCone(SDL_Renderer*r,float x,float y,float dir,float range,float half,bool red){
 float rg=range*SC,mx=OX+x*SC,my=y*SC;
 float dx=cosf(dir),dy=sinf(dir),ck=cosf(half);
 SDL_SetRenderDrawColor(r,255,red?40:235,red?40:59,80);
 int R=(int)rg;
 for(int by=-R;by<=R;by+=3){
  int x0=1<<30,x1=-(1<<30);
  for(int bx=-R;bx<=R;bx+=6){
   float d=sqrtf((float)(bx*bx+by*by));
   if(d>rg||d<1)continue;
   if(bx*dx+by*dy>=d*ck){if(bx<x0)x0=bx;if(bx>x1)x1=bx;}
  }
  if(x0<=x1){SDL_Rect q={(int)mx+x0,(int)my+by,x1-x0+6,3};SDL_RenderFillRect(r,&q);}
 }
}

static void famDraw(SDL_Renderer*r){
 if(menuOpen)return;
 famCone(r,dad.x,dad.y,dad.dir,dadRange(),0.6f,dad.seen>0);
 famCone(r,son.x,son.y,son.dir,sonRange(),0.8f,son.seen>0);
 disc(r,dad.x,dad.y,12,63,106,181);
 disc(r,son.x,son.y,9,255,202,40);
 px(r,440,950,204,14,0,0,0);
 px(r,442,952,tD*2,10,63,106,181);
 px(r,440,975,204,14,0,0,0);
 px(r,442,977,tS*2,10,255,202,40);
}

static void gameUpdateF(float dt,const Input&in){gameUpdate(dt,in);famUpdate(dt);}
static void gameDrawF(SDL_Renderer*r,const Input&in){gameDraw(r,in);famDraw(r);}
static void gameInitF(){gameInit();famInit();}
#define gameUpdate gameUpdateF
#define gameDraw gameDrawF
#define gameInit gameInitF
