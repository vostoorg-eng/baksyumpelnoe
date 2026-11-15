const WALLS=[[0,0,800,10],[0,590,800,10],[0,0,10,600],[790,0,10,600],[295,0,10,80],[295,150,10,300],[295,520,10,80],[0,195,100,10],[180,195,120,10],[0,395,100,10],[180,395,120,10],[300,345,200,10],[600,345,200,10]];
const FURN=[[470,440,120,70],[400,30,140,50],[740,380,40,60],[20,420,50,30],[680,14,70,16],[10,320,60,65],[20,510,100,70]];
const SOLID=WALLS.concat(FURN);
const PATH=[[680,250],[550,250],[550,390],[680,390],[680,560],[680,390],[550,390],[550,250]];
const SK=[['stealth','Скрытность','Мать замечает с меньшего расстояния'],['nose','Нюх','Еда подсвечивается сквозь стены'],['endur','Выдержка','Тревожность растёт медленнее']];
const EVS={phone:{t:'phone',x:150,y:100,n:'Звонит телефон! Подбеги и ГАВ'},door:{t:'door',x:30,y:60,n:'Стучат в дверь! Подбеги и ГАВ'},street:{t:'street',x:620,y:40,n:'Лай собаки на улице! К окну и ГАВ'}};
let S={lvl:1,xp:0,sp:0,stealth:0,nose:0,endur:0,trust:50,day:1};
try{const l=JSON.parse(localStorage.getItem('bax1'));if(l)S=Object.assign(S,l)}catch(e){}
function save(){try{localStorage.setItem('bax1',JSON.stringify(S))}catch(e){}}
let dog,food,mom,toy,cat,ev,evT,anx,seen,panic,msg='',msgT=0,grabP=false,barkP=false,menuOpen=false,shakeCd=0,stolen=0,dayDone=false,sneakNow=false,js=null;
function resetDay(){dog={x:60,y:100,dx:0,dy:1,moving:false};
food=[{x:540,y:425,got:0},{x:640,y:485,got:0},{x:150,y:470,got:0},{x:430,y:70,got:0},{x:350,y:500,got:0}];
toy={x:200,y:300,carried:false};mom={x:680,y:250,i:1,dir:0};
cat={x:700,y:150,st:'idle',t:3,tx:700,ty:150,cd:15};
ev=null;evT=8;anx=10;seen=0;panic=0;stolen=0;dayDone=false;
msg='День '+S.day+'. Цель: стащить 3 вкусняшки. Следи за котом!';msgT=4}
function say(t){msg=t;msgT=2.5}
function gain(n){S.xp+=n;while(S.xp>=S.lvl*40){S.xp-=S.lvl*40;S.lvl++;S.sp++;say('Уровень '+S.lvl+'! Очко навыка (МЕНЮ)')}save()}
function hit(x,y){const r=10;for(const [rx,ry,rw,rh] of SOLID){const cx=Math.max(rx,Math.min(x,rx+rw)),cy=Math.max(ry,Math.min(y,ry+rh));if((x-cx)**2+(y-cy)**2<r*r)return true}return false}
function inWall(x,y){for(const [rx,ry,rw,rh] of WALLS)if(x>rx&&x<rx+rw&&y>ry&&y<ry+rh)return true;return false}
function mult(){return 1-0.12*S.endur}
function viewRange(sneak){return (sneak?110:180)*(1-0.1*S.stealth)*(S.trust>=60?0.85:1)}
function dist(a,b){return Math.hypot(a.x-b.x,a.y-b.y)}
document.addEventListener('touchstart',e=>{if(menuOpen)return;if(e.target.tagName==='BUTTON')return;e.preventDefault();for(const t of e.changedTouches)if(t.clientX<innerWidth/2&&!js)js={id:t.identifier,x0:t.clientX,y0:t.clientY,x:t.clientX,y:t.clientY}},{passive:false});
document.addEventListener('touchmove',e=>{if(menuOpen)return;e.preventDefault();for(const t of e.changedTouches)if(js&&t.identifier===js.id){js.x=t.clientX;js.y=t.clientY}},{passive:false});
function endT(e){for(const t of e.changedTouches)if(js&&t.identifier===js.id)js=null}
document.addEventListener('touchend',endT);document.addEventListener('touchcancel',endT);
document.getElementById('grab').addEventListener('pointerdown',e=>{e.preventDefault();grabP=true});
document.getElementById('bark').addEventListener('pointerdown',e=>{e.preventDefault();barkP=true});
document.getElementById('mb').addEventListener('pointerdown',e=>{e.preventDefault();toggleMenu()});
const keys={};addEventListener('keydown',e=>{keys[e.key]=1;if(e.key==='e')grabP=true;if(e.key==='q')barkP=true;if(e.key==='m')toggleMenu()});
addEventListener('keyup',e=>keys[e.key]=0);
function toggleMenu(){menuOpen=!menuOpen;js=null;const m=document.getElementById('menu');m.style.display=menuOpen?'block':'none';if(menuOpen)renderMenu()}
function renderMenu(){const m=document.getElementById('menu');
let h='<h2>Баксюмпельное</h2><p>Уровень '+S.lvl+' · опыт '+S.xp+'/'+S.lvl*40+'<br>День '+S.day+' · Доверие Матери: '+S.trust+'/100<br>Очки навыков: <b>'+S.sp+'</b></p>';
for(const [k,n,d] of SK)h+='<button onclick="up(\''+k+'\')">'+n+' ('+S[k]+'/5) — '+d+'</button>';
h+='<button style="background:#90caf9" onclick="toggleMenu()">Закрыть</button><button style="background:#ef9a9a" onclick="if(confirm(\'Стереть прогресс?\')){localStorage.removeItem(\'bax1\');location.reload()}">Сбросить прогресс</button>';
m.innerHTML=h}
function up(k){if(S.sp>0&&S[k]<5){S[k]++;S.sp--;save();renderMenu()}}
function moveTo(o,tx,ty,sp,dt){const dx=tx-o.x,dy=ty-o.y,d=Math.hypot(dx,dy);if(d>3){o.x+=dx/d*sp*dt;o.y+=dy/d*sp*dt}return d}
function updateCat(dt){
cat.cd-=dt;
if(cat.st==='idle'){cat.t-=dt;
if(cat.t<=0){cat.tx=320+Math.random()*460;cat.ty=20+Math.random()*560;cat.t=3+Math.random()*3}
moveTo(cat,cat.tx,cat.ty,40,dt);
if(cat.cd<=0&&food.some(f=>!f.got)){cat.st='hunt';say('Кот крадётся к еде!')}}
else if(cat.st==='hunt'){let best=null,bd=1e9;for(const f of food)if(!f.got){const d=dist(cat,f);if(d<bd){bd=d;best=f}}
if(!best){cat.st='idle';cat.cd=20}
else{moveTo(cat,best.x,best.y,85,dt);
if(bd<10){best.got=2;anx+=8*mult();cat.st='idle';cat.cd=25;say('Кот украл вкусняшку!')}}}
else if(cat.st==='flee'){cat.t-=dt;let dx=cat.x-dog.x,dy=cat.y-dog.y;const d=Math.hypot(dx,dy)||1;
cat.x=Math.max(20,Math.min(780,cat.x+dx/d*130*dt));cat.y=Math.max(20,Math.min(580,cat.y+dy/d*130*dt));
if(cat.t<=0){cat.st='idle';cat.cd=12}}}
function seeing(range){const dx=dog.x-mom.x,dy=dog.y-mom.y,d=Math.hypot(dx,dy);if(d>range)return false;
let a=Math.atan2(dy,dx)-mom.dir;a=Math.atan2(Math.sin(a),Math.cos(a));if(Math.abs(a)>0.6)return false;
for(let s=0;s<d;s+=8)if(inWall(mom.x+dx*s/d,mom.y+dy*s/d))return false;return true}
function update(dt){
if(menuOpen){grabP=barkP=false;return}
if(dayDone){if(grabP)resetDay();grabP=barkP=false;return}
let jx=0,jy=0;
if(js){jx=(js.x-js.x0)/50;jy=(js.y-js.y0)/50;const m=Math.hypot(jx,jy);if(m>1){jx/=m;jy/=m}}
if(keys.ArrowLeft)jx-=1;if(keys.ArrowRight)jx+=1;if(keys.ArrowUp)jy-=1;if(keys.ArrowDown)jy+=1;
const mag=Math.min(1,Math.hypot(jx,jy));dog.moving=mag>0.1;
const sneak=mag>0.1&&mag<0.55;sneakNow=sneak;
if(panic>0)panic-=dt;
if(dog.moving){dog.dx=jx/mag;dog.dy=jy/mag;let sp=sneak?55+8*S.stealth:130;if(panic>0)sp*=0.5;sp*=dt;
const nx=dog.x+dog.dx*sp,ny=dog.y+dog.dy*sp;if(!hit(nx,dog.y))dog.x=nx;if(!hit(dog.x,ny))dog.y=ny}
if(toy.carried){toy.x=dog.x;toy.y=dog.y;anx-=3*dt}
const tg=PATH[mom.i],mx=tg[0]-mom.x,my=tg[1]-mom.y,md=Math.hypot(mx,my);
if(md<3)mom.i=(mom.i+1)%PATH.length;else{mom.dir=Math.atan2(my,mx);mom.x+=mx/md*55*dt;mom.y+=my/md*55*dt}
if(seeing(viewRange(sneak))){seen+=dt;if(seen>0.6){seen=0;anx+=15*mult();S.trust=Math.max(0,S.trust-5);dog.x=60;dog.y=100;say('Мать: «Баксик, фу!» Доверие -5');save()}}else seen=Math.max(0,seen-dt);
updateCat(dt);
evT-=dt;
if(!ev&&evT<=0){const k=['phone','door','street'][Math.floor(Math.random()*3)];ev=Object.assign({left:14},EVS[k]);say(ev.n)}
if(ev){ev.left-=dt;anx+=4*mult()*dt;if(ev.left<=0){anx+=10*mult();ev=null;evT=12+Math.random()*6;say('Ты не отреагировал...')}}
anx+=1.2*mult()*dt;
if(barkP){let used=false;
if(ev&&dist(dog,ev)<90){ev=null;evT=12+Math.random()*6;anx-=14;gain(8);say('Гав! Молодец! Стресс -14, +8 опыта');used=true}
if(cat.st!=='flee'&&dist(dog,cat)<110){cat.st='flee';cat.t=5;gain(4);say('Кот убежал от твоего лая!');used=true}
if(!used)say('Гав!')}
if(shakeCd>0)shakeCd-=dt;
if(grabP){let done=false;
for(const f of food)if(!f.got&&dist(dog,f)<30){f.got=1;stolen++;anx-=25;gain(10);say('Вкусняшка! +10 опыта');done=true;break}
if(!done){if(!toy.carried&&dist(dog,toy)<30){toy.carried=true;say('Канат в зубах. ВЗЯТЬ — потрепать');done=true}
else if(toy.carried&&shakeCd<=0){anx-=10;shakeCd=2;gain(2);say('Трепать канат! Стресс -10');done=true}}}
grabP=barkP=false;
if(anx>=100){anx=60;panic=6;say('Паника! Пёс дрожит и еле идёт')}
anx=Math.max(0,anx);
if(stolen>=3){dayDone=true;gain(25);S.day++;S.trust=Math.min(100,S.trust+3);save();msg='День пройден! +25 опыта. Нажми ВЗЯТЬ';msgT=999}
else if(food.every(f=>f.got)){resetDay();say('Кот и ты всё разобрали. Новый день, тот же номер')}
if(msgT>0&&msgT<990)msgT-=dt}
resetDay();
