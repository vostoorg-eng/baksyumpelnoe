while(dogM.children.length)dogM.remove(dogM.children[0]);
legs.length=0;
while(tailP.children.length)tailP.remove(tailP.children[0]);
dogM.add(tailP);
tailP.position.set(-0.56,0.66,0);
const MZ=0x3a3a3a;
add(dogM,sph(0.3,1.5,0.85,0.82,BK),0,0.52,0);
add(dogM,sph(0.3,0.95,0.95,0.8,BK),-0.3,0.55,0);
add(dogM,sph(0.3,1.05,1.12,0.95,BK),0.22,0.58,0);
add(dogM,sph(0.22,1,1.1,1,WH),0.38,0.44,0);
for(const s of [-1,1])add(dogM,sph(0.14,1,1.1,1,BK),0.3,0.58,s*0.2);
const neck=cyl(0.17,0.23,0.36,BK);neck.rotation.z=-0.85;add(dogM,neck,0.485,0.72,0);
const col=cyl(0.22,0.235,0.07,0xd32f2f);col.rotation.z=-0.85;add(dogM,col,0.425,0.667,0);
add(dogM,sph(0.035,1,1,0.5,0xffd600),0.63,0.52,0);
add(dogM,sph(0.2,1.05,0.95,1.12,BK),0.66,0.86,0);
for(const s of [-1,1]){
add(dogM,sph(0.12,1.1,0.9,1,BK),0.7,0.8,s*0.13);
add(dogM,sph(0.03,1,1,1,0xd08a1e),0.79,0.93,s*0.12);
const ear=new T.Mesh(new T.ConeGeometry(0.08,0.15,4),mat(BK));
ear.rotation.set(s*0.5,0,0.5);add(dogM,ear,0.6,1.04,s*0.16)}
add(dogM,new T.Mesh(new T.BoxGeometry(0.22,0.14,0.2),mat(MZ)),0.84,0.8,0);
add(dogM,sph(0.04,1.2,0.8,1.3,0x050505),0.955,0.84,0);
const tongue=add(dogM,new T.Mesh(new T.BoxGeometry(0.1,0.02,0.07),mat(0xf06292)),0.88,0.7,0);
for(const [x,z,c,r,l] of [[0.34,0.17,WH,0.085,0.42],[0.34,-0.17,WH,0.085,0.42],[-0.3,0.18,BK,0.08,0.44],[-0.3,-0.18,BK,0.08,0.44]]){
const p=new T.Group();p.position.set(x,0.43,z);dogM.add(p);
const lg=cyl(r,r*0.75,l,c);lg.position.y=-l/2;p.add(lg);
const paw=sph(0.075,1.35,0.55,1.05,c);paw.position.set(0.04,-l+0.02,0);p.add(paw);legs.push(p)}
const tl=cyl(0.05,0.015,0.5,BK);tl.rotation.z=1.0;tl.position.set(-0.21,0.13,0);tailP.add(tl);
const dogShadow=new T.Mesh(new T.CircleGeometry(0.5,10),new T.MeshBasicMaterial({color:0x000000,transparent:true,opacity:0.25,depthWrite:false}));
dogShadow.rotation.x=-Math.PI/2;dogShadow.position.y=0.02;dogShadow.scale.set(1.6,1,1);dogG.add(dogShadow);
const _sd=sync;sync=function(dt,t){_sd(dt,t);tongue.visible=anx<35&&panic<=0};
