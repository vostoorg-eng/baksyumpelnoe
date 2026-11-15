Object.assign(EVS,{
courier:{t:'courier',x:30,y:60,n:'Курьер у двери! Подбеги и ГАВ'},
guest:{t:'guest',x:30,y:60,n:'Пришёл гость! Встреть его: ГАВ у двери'},
vacuum:{t:'vacuum',x:450,y:250,n:'Гудит пылесос! Подбеги и ГАВ'}});
let lastEv=null;
const _r3=resetDay;
resetDay=function(){_r3();food.push({x:745,y:555,got:0},{x:200,y:150,got:3});lastEv=null};
QS.push({n:'Достань еду из мусорки',c:()=>!!food[5]&&food[5].got===1});
box(730,540,30,30,0.6,0x455a64);
const vac=box(440,240,20,20,0.45,0x1e88e5);
const visG=person(0xff7043,0x263238,0x212121);visG.position.set(0.55,0,1.5);visG.visible=false;
function mkFood(c){const g=new T.Group();g.add(sph(0.14,1,0.8,1,c));
const h=new T.Mesh(new T.SphereGeometry(0.4,8,6),new T.MeshBasicMaterial({color:0xffff00,transparent:true,opacity:0.3,depthTest:false}));
h.renderOrder=10;h.name='halo';g.add(h);scene.add(g);return g}
foodM.push(mkFood(0x9ccc65),mkFood(0xffb74d));
const _u3=update;
update=function(dt){const pe=ev;_u3(dt);
if(menuOpen||dayDone)return;
if(ev&&ev!==lastEv){lastEv=ev;
if(['phone','door','street'].includes(ev.t)&&Math.random()<0.55){
const ks=['courier','guest','vacuum'];
ev=Object.assign({left:14},EVS[ks[Math.floor(Math.random()*3)]]);lastEv=ev;say(ev.n)}}
if(ev&&ev.t==='vacuum')anx+=4*mult()*dt;
if(pe&&!ev&&pe.left>0){
if(pe.t==='courier'){if(food[6]&&food[6].got===3){food[6].got=0;say('Курьер оставил пакет в прихожей! Там вкусняшки')}}
else if(pe.t==='guest'){anx=Math.max(0,anx-8);gain(6);say('Гость погладил тебя и дал печеньку! Стресс -8')}
else if(pe.t==='vacuum'){gain(4);say('Пылесос выключили. Тишина!')}}};
const _s3=sync;
sync=function(dt,t){_s3(dt,t);
for(let i=5;i<7;i++){const f=food[i],m=foodM[i];if(!f)continue;
m.visible=f.got===0;if(f.got!==0)continue;
m.position.set(f.x*K,(i===5?0.75:0.2)+Math.sin(t*3+i)*0.05,f.y*K);m.rotation.y=t*2;
m.getObjectByName('halo').visible=S.nose>0&&dist(dog,f)<60+50*S.nose}
const vis=ev&&(ev.t==='courier'||ev.t==='guest');visG.visible=!!vis;
if(vis&&blink())doorM.material.color.set(0xf44336);
vac.material.color.set(ev&&ev.t==='vacuum'&&blink()?0xf44336:0x1e88e5)};
resetDay();
