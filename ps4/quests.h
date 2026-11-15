#pragma once
#define QSAVE "/data/baksyumpelnoe3.sav"
struct XF{float x,y;int got;};
static XF xf[2];
static int qDone=0,qReact=0,pEvQ=0;
static bool qChased=false,qCaught=false,qFinished=false,qFood0=false;

static void qSave(){
 FILE*f=fopen(QSAVE,"w");if(!f)return;
 fprintf(f,"%d\n",qDone);fclose(f);
}
static void qLoad(){
 FILE*f=fopen(QSAVE,"r");if(!f)return;
 int a;if(fscanf(f,"%d",&a)==1)qDone=a<0?0:a;
 fclose(f);
}
static void qResetDay(){qReact=0;qChased=false;qCaught=false;qFinished=false;qFood0=false;}
static void xfReset(){
 xf[0].x=745;xf[0].y=555;xf[0].got=0;
 xf[1].x=200;xf[1].y=150;xf[1].got=3;
}
static void questInit(){qLoad();xfReset();qResetDay();pEvQ=0;}

static bool qMet(int i){
 switch(i){
  case 0:return qFood0;
  case 1:return son.cd>19.5f;
  case 2:return qReact>=3;
  case 3:return dad.cd>11.5f;
  case 4:return qChased;
  case 5:return xf[0].got==1;
 }
 return false;
}
static void qComplete(){
 qFinished=true;qDone++;
 trust=trust+2>100?100:trust+2;
 gain(20);setFx(3);qSave();
}

static void gameUpdateQ(float dt,const Input&in){
 if(menuOpen){gameUpdate(dt,in);return;}
 int pe=evType,pst=stolen,pdy=day,pD=tD,pS=tS,pT=trust;
 bool g0=in.cross&&food[0].got==0&&dist(dogx,dogy,food[0].x,food[0].y)<30;
 gameUpdate(dt,in);
 if(menuOpen)return;
 bool newDayF=(catCd==15.0f&&evT==22.0f);
 bool dayAdv=(day!=pdy);
 if(g0)qFood0=true;
 if(tD<pD||tS<pS||(!dayAdv&&trust<pT))qCaught=true;
 if(catSt==2)qChased=true;
 if(pe&&!evType&&evLeft>0&&!newDayF){
  qReact++;
  if(pe==4&&xf[1].got==3){xf[1].got=0;setFx(1);}
  else if(pe==5){anx=anx-8<0?0:anx-8;gain(6);}
  else if(pe==6)gain(4);
 }
 if(pe==0&&evType>=1&&evType<=3&&rand()%100<55){
  evType=4+rand()%3;
  if(evType==6){evx=450;evy=250;}else{evx=30;evy=60;}
 }
 if(evType==6)anx+=4*mult()*dt*0.7f;
 if(dayAdv){
  int pi=(pdy-1)%7;
  if(!qFinished){
   if(pi==6&&!qCaught)qComplete();
   else if(pi==0&&qFood0)qComplete();
  }
  qResetDay();
 }
 if(newDayF)xfReset();
 if(!newDayF&&in.cross&&stolen==pst){
  for(int i=0;i<2;i++){
   if(xf[i].got==0&&dist(dogx,dogy,xf[i].x,xf[i].y)<30){
    xf[i].got=1;stolen++;anx-=25;if(anx<0)anx=0;gain(10);setFx(1);break;
   }
  }
 }
 if(!newDayF&&!dayAdv&&!qFinished){
  int idx=(day-1)%7;
  if(idx<6&&qMet(idx))qComplete();
 }
}

static void questDraw(SDL_Renderer*r,const Input&in){
 if(menuOpen)return;
 bool bl=blinkOn();
 fr(r,730,540,30,30,69,90,100);
 bool vb=(evType==6&&bl);
 fr(r,440,240,20,20,vb?244:30,vb?67:136,vb?54:229);
 if(evType==4||evType==5){
  if(bl)fr(r,0,40,10,40,244,67,54);
  disc(r,32,60,9,255,112,67);
 }
 for(int i=0;i<2;i++){
  if(xf[i].got!=0)continue;
  if(sk[1]>0&&dist(dogx,dogy,xf[i].x,xf[i].y)<60+50*sk[1])disc(r,xf[i].x,xf[i].y,16,255,255,0,100);
  disc(r,xf[i].x,xf[i].y,7,i==0?156:255,i==0?204:183,i==0?101:77);
 }
 disc(r,dogx,dogy,10,10,10,10);
 disc(r,dogx+ddx*9,dogy+ddy*9,6,10,10,10);
 disc(r,dogx+ddx*4,dogy+ddy*4,3.5f,245,245,245);

 number(r,20,905,day,5,255,255,255);
 for(int i=0;i<5;i++)px(r,20+i*26,950,20,20,food[i].got?60:229,food[i].got?60:57,food[i].got?60:53);
 for(int i=0;i<sp&&i<12;i++)px(r,20+i*22,980,18,18,255,213,79);
 px(r,20,1004,404,16,0,0,0);
 px(r,22,1006,(int)(400.f*xp/(lvl*40)),12,66,165,245);
 number(r,440,1000,lvl,5,255,255,255);
 px(r,20,1030,404,28,0,0,0);
 px(r,24,1034,(int)(anx<100?anx*4:400),20,anx>70?244:255,anx>70?67:179,anx>70?54:0);
 px(r,440,950,204,14,0,0,0);
 px(r,442,952,tD*2,10,63,106,181);
 px(r,440,975,204,14,0,0,0);
 px(r,442,977,tS*2,10,255,202,40);

 int idx=(day-1)%7;
 number(r,60,40,idx+1,6,255,213,79);
 int R=70,G=70,B=80;
 if(qFinished){R=102;G=187;B=106;}
 else if(idx==6&&qCaught){R=244;G=67;B=54;}
 px(r,110,40,30,30,R,G,B);
 if(idx==2&&!qFinished)for(int i=0;i<3;i++)px(r,60+i*26,86,20,12,i<qReact?102:70,i<qReact?187:70,i<qReact?106:80);
 number(r,60,120,qDone,4,255,255,255);
}

static void gameInitQ(){gameInit();questInit();}
static void gameDrawQ(SDL_Renderer*r,const Input&in){gameDraw(r,in);questDraw(r,in);}
#undef gameUpdate
#undef gameDraw
#undef gameInit
#define gameUpdate gameUpdateQ
#define gameDraw gameDrawQ
#define gameInit gameInitQ
