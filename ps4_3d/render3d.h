#pragma once
#include <cmath>
#include <cstring>
#include <cstdint>
#define RW 640
#define RH 360
#define WK 0.025f
static uint32_t fbuf[RW*RH];
static float zbuf[RW*RH];
static float cEx,cEy,cEz,cFx,cFy,cFz,cRx,cRz,cUx,cUy,cUz;
static const float FOC=345.0f;
static float gCa=1,gSa=0,gPx=0,gPy=0,gPz=0,gSc=1,gYS=1;
static int gAlpha=255;
static bool gNoZ=false;

static void camLook(float ex,float ey,float ez,float tx,float ty,float tz){
 cEx=ex;cEy=ey;cEz=ez;
 float fx=tx-ex,fy=ty-ey,fz=tz-ez,l=sqrtf(fx*fx+fy*fy+fz*fz);
 fx/=l;fy/=l;fz/=l;cFx=fx;cFy=fy;cFz=fz;
 float rx=-fz,rz=fx,rl=sqrtf(rx*rx+rz*rz);
 rx/=rl;rz/=rl;cRx=rx;cRz=rz;
 cUx=0*fz-rz*fy;cUy=rz*fx-rx*fz;cUz=rx*fy-0*fx;
}

static void rast(float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz,uint32_t col){
 float area=(cx-ax)*(by-ay)-(cy-ay)*(bx-ax);
 if(fabsf(area)<1e-5f)return;
 float minx=fminf(ax,fminf(bx,cx)),maxx=fmaxf(ax,fmaxf(bx,cx));
 float miny=fminf(ay,fminf(by,cy)),maxy=fmaxf(ay,fmaxf(by,cy));
 int x0=(int)fmaxf(0.f,floorf(minx)),x1=(int)fminf((float)(RW-1),ceilf(maxx));
 int y0=(int)fmaxf(0.f,floorf(miny)),y1=(int)fminf((float)(RH-1),ceilf(maxy));
 if(x0>x1||y0>y1)return;
 float ia=1.f/az,ib=1.f/bz,ic=1.f/cz,inv=1.f/area;
 unsigned sr=(col>>16)&255,sg=(col>>8)&255,sb=col&255;
 for(int y=y0;y<=y1;y++){
  float py=y+0.5f;
  for(int x=x0;x<=x1;x++){
   float px=x+0.5f;
   float w0=((px-bx)*(cy-by)-(py-by)*(cx-bx))*inv;
   float w1=((px-cx)*(ay-cy)-(py-cy)*(ax-cx))*inv;
   float w2=((px-ax)*(by-ay)-(py-ay)*(bx-ax))*inv;
   if(w0<0||w1<0||w2<0)continue;
   float iz=w0*ia+w1*ib+w2*ic;
   int idx=y*RW+x;
   if(!gNoZ&&iz<=zbuf[idx])continue;
   if(gAlpha>=255){zbuf[idx]=iz;fbuf[idx]=col;}
   else{
    uint32_t d=fbuf[idx];
    unsigned dr=(d>>16)&255,dg=(d>>8)&255,db=d&255,a=gAlpha;
    fbuf[idx]=(((sr*a+dr*(255-a))/255)<<16)|(((sg*a+dg*(255-a))/255)<<8)|((sb*a+db*(255-a))/255);
   }
  }
 }
}

static void tri3(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2,uint32_t col){
 float ux=x1-x0,uy=y1-y0,uz=z1-z0,vx=x2-x0,vy=y2-y0,vz=z2-z0;
 float nx=uy*vz-uz*vy,ny=uz*vx-ux*vz,nz=ux*vy-uy*vx;
 float nl=sqrtf(nx*nx+ny*ny+nz*nz);
 if(nl<1e-9f)return;
 nx/=nl;ny/=nl;nz/=nl;
 float mx=(x0+x1+x2)/3-cEx,my=(y0+y1+y2)/3-cEy,mz=(z0+z1+z2)/3-cEz;
 if(nx*mx+ny*my+nz*mz>0){nx=-nx;ny=-ny;nz=-nz;}
 float d=nx*0.42f+ny*0.80f+nz*0.33f;
 if(d<0)d=0;
 float k=0.55f+0.45f*d;
 unsigned r=(unsigned)(((col>>16)&255)*k),g=(unsigned)(((col>>8)&255)*k),b=(unsigned)((col&255)*k);
 if(r>255)r=255;
 if(g>255)g=255;
 if(b>255)b=255;
 uint32_t sc=(r<<16)|(g<<8)|b;
 float w[9]={x0,y0,z0,x1,y1,z1,x2,y2,z2};
 float px[3],py[3],pz[3];
 for(int i=0;i<3;i++){
  float dx=w[i*3]-cEx,dy=w[i*3+1]-cEy,dz=w[i*3+2]-cEz;
  px[i]=dx*cRx+dz*cRz;
  py[i]=dx*cUx+dy*cUy+dz*cUz;
  pz[i]=dx*cFx+dy*cFy+dz*cFz;
 }
 const float NEAR=0.15f;
 float qx[4],qy[4],qz[4];int n=0;
 for(int i=0;i<3;i++){
  int j=(i+1)%3;
  bool ai=pz[i]>=NEAR,bi=pz[j]>=NEAR;
  if(ai){qx[n]=px[i];qy[n]=py[i];qz[n]=pz[i];n++;}
  if(ai!=bi){
   float t=(NEAR-pz[i])/(pz[j]-pz[i]);
   qx[n]=px[i]+(px[j]-px[i])*t;qy[n]=py[i]+(py[j]-py[i])*t;qz[n]=NEAR;n++;
  }
 }
 if(n<3)return;
 float sx[4],sy[4];
 for(int i=0;i<n;i++){sx[i]=RW*0.5f+qx[i]/qz[i]*FOC;sy[i]=RH*0.5f-qy[i]/qz[i]*FOC;}
 for(int i=1;i+1<n;i++)rast(sx[0],sy[0],qz[0],sx[i],sy[i],qz[i],sx[i+1],sy[i+1],qz[i+1],sc);
}

static void boxW(float x0,float y0,float z0,float x1,float y1,float z1,uint32_t c){
 float X[2]={x0,x1},Y[2]={y0,y1},Z[2]={z0,z1};
 static const int F[6][4][3]={
  {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},{{0,0,1},{1,0,1},{1,1,1},{0,1,1}},
  {{0,0,0},{0,0,1},{0,1,1},{0,1,0}},{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
  {{0,0,0},{1,0,0},{1,0,1},{0,0,1}},{{0,1,0},{1,1,0},{1,1,1},{0,1,1}}};
 for(int f=0;f<6;f++){
  float v[4][3];
  for(int i=0;i<4;i++){v[i][0]=X[F[f][i][0]];v[i][1]=Y[F[f][i][1]];v[i][2]=Z[F[f][i][2]];}
  tri3(v[0][0],v[0][1],v[0][2],v[1][0],v[1][1],v[1][2],v[2][0],v[2][1],v[2][2],c);
  tri3(v[0][0],v[0][1],v[0][2],v[2][0],v[2][1],v[2][2],v[3][0],v[3][1],v[3][2],c);
 }
}
static void gbox(float x,float y,float w,float h,float hh,uint32_t c,float y0=0){
 boxW(x*WK,y0,y*WK,(x+w)*WK,y0+hh,(y+h)*WK,c);
}
static void setM(float ang,float px,float py,float pz,float sc){
 gCa=cosf(ang);gSa=sinf(ang);gPx=px;gPy=py;gPz=pz;gSc=sc;
}
#define SU 8
#define SV 5
static float uSx[(SV+1)*SU],uSy[(SV+1)*SU],uSz[(SV+1)*SU];
static bool uInit=false;
static void ell(float cx,float cy,float cz,float rx,float ry,float rz,uint32_t col){
 if(!uInit){
  for(int i=0;i<=SV;i++){float ph=3.14159265f*i/SV;
   for(int j=0;j<SU;j++){float th=6.2831853f*j/SU;int k=i*SU+j;
    uSx[k]=sinf(ph)*cosf(th);uSy[k]=cosf(ph);uSz[k]=sinf(ph)*sinf(th);}}
  uInit=true;
 }
 float vx[(SV+1)*SU],vy[(SV+1)*SU],vz[(SV+1)*SU];
 for(int k=0;k<(SV+1)*SU;k++){
  float lx=(cx+rx*uSx[k])*gSc,ly=(cy+ry*uSy[k])*gSc*gYS,lz=(cz+rz*uSz[k])*gSc;
  vx[k]=gPx+lx*gCa-lz*gSa;vy[k]=gPy+ly;vz[k]=gPz+lx*gSa+lz*gCa;
 }
 for(int i=0;i<SV;i++)for(int j=0;j<SU;j++){
  int a=i*SU+j,b=i*SU+(j+1)%SU,c=(i+1)*SU+j,d=(i+1)*SU+(j+1)%SU;
  tri3(vx[a],vy[a],vz[a],vx[b],vy[b],vz[b],vx[c],vy[c],vz[c],col);
  tri3(vx[b],vy[b],vz[b],vx[d],vy[d],vz[d],vx[c],vy[c],vz[c],col);
 }
}

static void sector(float x,float y,float dir,float range,float half,uint32_t col){
 const int N=14;
 float cx=x*WK,cz=y*WK,r=range*WK,yy=0.07f;
 gAlpha=95;
 for(int i=0;i<N;i++){
  float a0=dir-half+2*half*i/N,a1=dir-half+2*half*(i+1)/N;
  tri3(cx,yy,cz,cx+cosf(a0)*r,yy,cz+sinf(a0)*r,cx+cosf(a1)*r,yy,cz+sinf(a1)*r,col);
 }
 gAlpha=255;
}
