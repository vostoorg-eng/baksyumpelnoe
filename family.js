const DP=[[550,200],[550,330],[550,390],[705,415]],SEAT=[490,58];
const SP=[[180,470],[330,485],[420,450],[420,550],[330,490],[200,550]];
const SITDIR=Math.atan2(22-58,715-490);
let dad,son;
if(S.tD===undefined)S.tD=50;
if(S.tS===undefined)S.tS=50;
const _reset=resetDay,_upd=update,_sync=sync,_rm=renderMenu;
function dadRange(){return(sneakNow?80:150)*(1-0.1*S.stealth)*(S.tD>=60?0.85:1)}
function sonRange(){return(sneakNow?70:130)*(1-0.1*S.stealth)*(S.tS>=60?0.85:1)}
function stepTo(w,tx,ty,sp,dt){const dx=tx-w.x,dy=ty-w.y,d=Math.hypot(dx,dy);if(d<4)return true;w.dir=Math.atan2(dy,dx);w.x+=dx/d*sp*dt;w.y+=dy/d*sp*dt;return false}
function famSees(w,range,half){const dx=dog.x-w.x,dy=dog.y-w.y,d=Math.hypot(dx,dy);if(d>range)return false;
let a=Math.atan2(dy,dx)-w.dir;a=Math.atan2(Math.sin(a),Math.cos(a));if(Math.abs(a)>half)return false;
for(let s=0;s<d;s+=8)if(inWall(w.x+dx*s/d,w.y+dy*s/d))return false;return true}
function scold(w,k,line,a){w.seen=0;anx+=a*mult();S[k]=Math.max(0,S[k]-4);dog.x=60;dog.y=100;say(line);save()}
resetDay=function(){_reset();
dad={x:SEAT[0],y:SEAT[1],dir:SITDIR,st:'sit',t:20+Math.random()*10,i:0,seen:0,fawn:0,cd:0};
son={x:180,y:470,dir:0,i:1,seen:0,cd:0,pause:0};
msg='День '+S.day+'. Мать, Отец, Сын и кот дома. Осторожно!';msgT=4};
update=function(dt){_upd(dt);if(menuOpen||dayDone)return;famUpdate(dt)};
function famUpdate(dt){
dad.cd-=dt;son.cd-=dt;
if(dad.st==='sit'){dad.dir=SITDIR;dad.t-=dt;if(dad.t<=0){dad.st='go';dad.i=0;say('Отец встал с дивана')}}
else if(dad.st==='go'){if(stepTo(dad,DP[dad.i][0],DP[dad.i][1],50,dt)){dad.i++;if(dad.i>=DP.length){dad.st='fridge';dad.t=4}}}
else if(dad.st==='fridge'){dad.dir=0;dad.t-=dt;if(dad.t<=0){dad.st='back';dad.i=DP.length-2}}
else if(dad.st==='back'){const p=dad.i>=0?DP[dad.i]:SEAT;if(stepTo(dad,p[0],p[1],50,dt)){if(dad.i<0){dad.st='sit';dad.t=20+Math.random()*15}else dad.i--}}
if(son.pause>0)son.pause-=dt;else{const p=SP[son.i];if(stepTo(son,p[0],p[1],65,dt)){if(son.i===3)son.pause=2;son.i=(son.i+1)%SP.length}}
const dd=dist(dog,dad),sd=dist(dog,son);
if(dad.st==='sit'&&dd<50){dad.seen=0;dad.fawn+=dt;
if(dad.fawn>=3&&dad.cd<=0){dad.fawn=0;dad.cd=12;S.tD=Math.min(100,S.tD+3);anx-=10;gain(4);let t='Отец почесал тебя за ухом. Доверие +3';
if(S.tD>=60){anx-=10;gain(4);t+=' и дал колбаски'}say(t);save()}}
else{dad.fawn=Math.max(0,dad.fawn-dt);
if(famSees(dad,dadRange(),0.6)){dad.seen+=dt;if(dad.seen>0.6)scold(dad,'tD','Отец: «Ну-ка, Баксик, брысь!» Доверие -4',12)}else dad.seen=Math.max(0,dad.seen-dt)}
if(toy.carried&&sd<90){son.seen=0;
if(sd<60&&son.cd<=0){son.cd=20;S.tS=Math.min(100,S.tS+3);anx-=15;gain(6);say('Сын играет с тобой в канат! Стресс -15');save()}}
else if(famSees(son,sonRange(),0.8)){son.seen+=dt;if(son.seen>0.6)scold(son,'tS','Сын: «Ты что тут делаешь?!» Доверие -4',8)}
else son.seen=Math.max(0,son.seen-dt);
if(anx>=100){anx=60;panic=6;say('Паника! Пёс дрожит и еле идёт')}
anx=Math.max(0,anx)}
renderMenu=function(){_rm();document.getElementById('menu').insertAdjacentHTML('beforeend','<p>Доверие: Отец '+S.tD+', Сын '+S.tS+'</p>')};
function person(shirt,pants,hair){const g=new T.Group();
add(g,cyl(0.22,0.28,1.0,shirt),0,0.9,0);
for(const s of [-1,1])add(g,cyl(0.09,0.09,0.4,pants),0,0.2,s*0.1);
add(g,sph(0.2,1,1,1,0xffcc99),0,1.62,0);
add(g,sph(0.21,1,0.8,1,hair),-0.03,1.7,0);
add(g,sph(0.04,1,1,1,0xe0a070),0.2,1.6,0);
scene.add(g);return g}
function cone(half){const g=new T.Group();
const m=new T.Mesh(new T.CircleGeometry(1,14,-half,half*2),new T.MeshBasicMaterial({color:0xffeb3b,transparent:true,opacity:0.3,side:T.DoubleSide,depthWrite:false}));
m.rotation.x=-Math.PI/2;g.add(m);scene.add(g);return [g,m]}
const dadG=person(0x3f6ab5,0x37474f,0x9e9e9e),sonG=person(0xffca28,0x1565c0,0x3e2723);
sonG.scale.set(0.75,0.75,0.75);
const [dcG,dcM]=cone(0.6),[scG,scM]=cone(0.8);
sync=function(dt,t){_sync(dt,t);
dadG.position.set(dad.x*K,dad.st==='sit'?0.3:Math.abs(Math.sin(t*6))*0.03,dad.y*K);dadG.rotation.y=-dad.dir;
dcG.position.set(dad.x*K,0.07,dad.y*K);dcG.rotation.y=-dad.dir;
const dr=dadRange()*K;dcM.scale.set(dr,dr,1);dcM.material.color.set(dad.seen>0?0xff2222:0xffeb3b);
sonG.position.set(son.x*K,Math.abs(Math.sin(t*9))*0.04,son.y*K);sonG.rotation.y=-son.dir;
scG.position.set(son.x*K,0.08,son.y*K);scG.rotation.y=-son.dir;
const sr=sonRange()*K;scM.scale.set(sr,sr,1);scM.material.color.set(son.seen>0?0xff2222:0xffeb3b);
hudSt.textContent+='  Отец '+S.tD+' Сын '+S.tS};
resetDay();
