const T=THREE,K=1/40;
const renderer=new T.WebGLRenderer({canvas:document.getElementById('gl'),antialias:false});
const scene=new T.Scene();scene.background=new T.Color(0x20202a);
const camera=new T.PerspectiveCamera(55,innerWidth/innerHeight,0.1,80);
function resize(){renderer.setSize(Math.floor(innerWidth/3),Math.floor(innerHeight/3),false);camera.aspect=innerWidth/innerHeight;camera.updateProjectionMatrix()}
addEventListener('resize',resize);resize();
scene.add(new T.AmbientLight(0xffffff,0.75));
const sun=new T.DirectionalLight(0xfff2dd,0.55);sun.position.set(6,12,4);scene.add(sun);
function mat(c){return new T.MeshLambertMaterial({color:c,flatShading:true})}
function tex(a,b){const cv=document.createElement('canvas');cv.width=cv.height=8;const x=cv.getContext('2d');
for(let i=0;i<8;i++)for(let j=0;j<8;j++){x.fillStyle=((i>>1)+(j>>1))%2?a:b;x.fillRect(i,j,1,1)}
const t=new T.CanvasTexture(cv);t.magFilter=T.NearestFilter;t.minFilter=T.NearestFilter;t.wrapS=t.wrapT=T.RepeatWrapping;return t}
function box(x,y,w,h,hh,col,y0){const m=new T.Mesh(new T.BoxGeometry(w*K,hh,h*K),mat(col));m.position.set((x+w/2)*K,(y0||0)+hh/2,(y+h/2)*K);scene.add(m);return m}
function sph(r,sx,sy,sz,c){const m=new T.Mesh(new T.SphereGeometry(r,8,6),mat(c));m.scale.set(sx,sy,sz);return m}
function cyl(r1,r2,h,c){return new T.Mesh(new T.CylinderGeometry(r1,r2,h,6),mat(c))}
function add(p,m,x,y,z){m.position.set(x,y,z);p.add(m);return m}
const ROOMS=[[0,0,300,200,'#8d6e63','#7d5e53'],[0,200,300,200,'#80cbc4','#70bbb4'],[0,400,300,200,'#9fa8da','#8f98ca'],[300,0,500,350,'#bcaaa4','#ac9a94'],[300,350,500,250,'#ffe0b2','#efd0a2']];
for(const [x,y,w,h,a,b] of ROOMS){const t=tex(a,b);t.repeat.set(w/40,h/40);
const f=new T.Mesh(new T.PlaneGeometry(w*K,h*K),new T.MeshLambertMaterial({map:t}));f.rotation.x=-Math.PI/2;f.position.set((x+w/2)*K,0,(y+h/2)*K);scene.add(f)}
for(const r of WALLS)box(r[0],r[1],r[2],r[3],2.2,0x9c8472);
box(470,440,120,70,0.12,0x8d5a3a,0.78);
for(const [lx,ly] of [[472,442],[580,442],[472,500],[580,500]])box(lx,ly,8,8,0.78,0x6d4a2a);
box(400,30,140,50,0.5,0x5c6bc0);box(400,30,140,12,1.0,0x3f51b5);
box(740,380,40,60,2.0,0xeceff1);
box(20,420,50,30,0.8,0x8d6e63);
box(680,14,70,16,0.5,0x5d4037);box(690,18,50,6,0.7,0x111111,0.5);
box(10,320,60,65,0.1,0xffffff);
const shw=box(10,320,60,65,2.0,0x81d4fa);shw.material.transparent=true;shw.material.opacity=0.35;shw.material.depthWrite=false;
box(20,510,100,70,0.4,0x7986cb);box(24,514,24,62,0.15,0xffffff,0.4);
const phoneM=box(140,90,20,20,0.12,0x333333,0);
const doorM=box(10,40,4,40,1.9,0x5d4037);
const windowM=box(590,10,60,3,0.9,0x81d4fa,1.0);
const BK=0x141414,WH=0xf5f5f5;
const dogG=new T.Group(),dogM=new T.Group();dogG.add(dogM);scene.add(dogG);
add(dogM,sph(0.3,1.5,0.85,0.8,BK),0,0.5,0);
add(dogM,sph(0.2,1,1.05,0.95,WH),0.32,0.45,0);
add(dogM,sph(0.2,1,0.95,1,BK),0.58,0.72,0);
add(dogM,sph(0.12,1.4,0.7,0.8,0x2a2a2a),0.76,0.66,0);
for(const s of [-1,1]){add(dogM,sph(0.035,1,1,1,WH),0.7,0.78,s*0.1);
const e=add(dogM,new T.Mesh(new T.ConeGeometry(0.07,0.16,4),mat(BK)),0.52,0.92,s*0.14);e.rotation.x=s*0.4}
const legs=[];
for(const [x,z,c] of [[0.3,0.14,WH],[0.3,-0.14,WH],[-0.28,0.14,BK],[-0.28,-0.14,BK]]){
const p=new T.Group();p.position.set(x,0.38,z);dogM.add(p);const l=cyl(0.065,0.05,0.38,c);l.position.y=-0.19;p.add(l);legs.push(p)}
const tailP=new T.Group();tailP.position.set(-0.42,0.6,0);dogM.add(tailP);
const tl=cyl(0.04,0.02,0.4,BK);tl.rotation.z=1.0;tl.position.set(-0.17,0.11,0);tailP.add(tl);
const GR=0x8e9aa6;const catG=new T.Group();scene.add(catG);
add(catG,sph(0.18,1.4,0.9,0.8,GR),0,0.25,0);
add(catG,sph(0.13,1,1,1.1,GR),0.28,0.36,0);
for(const s of [-1,1]){add(catG,new T.Mesh(new T.ConeGeometry(0.05,0.1,4),mat(GR)),0.28,0.5,s*0.08);add(catG,sph(0.025,1,1,1,0xffb300),0.39,0.39,s*0.06)}
const ct=cyl(0.03,0.02,0.4,GR);ct.rotation.z=1.2;add(catG,ct,-0.35,0.4,0);
for(const [x,z] of [[0.15,0.08],[0.15,-0.08],[-0.15,0.08],[-0.15,-0.08]])add(catG,cyl(0.04,0.04,0.15,GR),x,0.08,z);
const momG=new T.Group();scene.add(momG);
add(momG,cyl(0.22,0.28,1.0,0xec407a),0,0.9,0);
for(const s of [-1,1])add(momG,cyl(0.09,0.09,0.4,0x37474f),0,0.2,s*0.1);
add(momG,sph(0.2,1,1,1,0xffcc99),0,1.62,0);
add(momG,sph(0.21,1,0.8,1,0x5d4037),-0.03,1.7,0);
add(momG,sph(0.04,1,1,1,0xe0a070),0.2,1.6,0);
const coneG=new T.Group();coneG.position.y=0.06;scene.add(coneG);
const coneM=new T.Mesh(new T.CircleGeometry(1,14,-0.6,1.2),new T.MeshBasicMaterial({color:0xffeb3b,transparent:true,opacity:0.3,side:T.DoubleSide,depthWrite:false}));
coneM.rotation.x=-Math.PI/2;coneG.add(coneM);
const foodM=[],foodH=[0.2,0.2,0.2,0.65,0.2];
for(let i=0;i<5;i++){const g=new T.Group();g.add(sph(0.14,1,0.8,1,0xe53935));
const halo=new T.Mesh(new T.SphereGeometry(0.4,8,6),new T.MeshBasicMaterial({color:0xffff00,transparent:true,opacity:0.3,depthTest:false}));halo.renderOrder=10;halo.name='halo';g.add(halo);
scene.add(g);foodM.push(g)}
const toyM=cyl(0.05,0.05,0.5,0x43a047);toyM.rotation.z=Math.PI/2;
scene.add(toyM);
