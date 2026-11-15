const hudSt=document.getElementById('st'),fill=document.getElementById('fill'),msgEl=document.getElementById('msg');
let dA=Math.PI/2,cA=0,pc={x:cat.x,y:cat.y};
function turn(a,b,k){let d=b-a;d=Math.atan2(Math.sin(d),Math.cos(d));return a+d*k}
function blink(){return Math.floor(performance.now()/200)%2===1}
function sync(dt,t){
dA=turn(dA,Math.atan2(dog.dy,dog.dx),Math.min(1,dt*12));
dogG.position.set(dog.x*K+(panic>0?Math.sin(t*60)*0.03:0),0,dog.y*K);dogG.rotation.y=-dA;
const mv=dog.moving&&!menuOpen;
dogM.position.y=mv?Math.abs(Math.sin(t*10))*0.04:0;
dogM.scale.y=sneakNow&&mv?0.85:1;
for(let i=0;i<4;i++)legs[i].rotation.z=mv?Math.sin(t*10+(i%2?Math.PI:0)+(i>1?Math.PI/2:0))*0.6:0;
tailP.rotation.y=Math.sin(t*(anx<50?14:6))*(anx<50?0.7:0.25);
if(toy.carried){toyM.position.set(dog.x*K+Math.cos(dA)*0.7,0.45,dog.y*K+Math.sin(dA)*0.7);toyM.rotation.y=-dA+Math.PI/2;toyM.rotation.z=Math.PI/2}
else{toyM.position.set(toy.x*K,0.06,toy.y*K);toyM.rotation.y=0}
momG.position.set(mom.x*K,Math.abs(Math.sin(t*6))*0.03,mom.y*K);momG.rotation.y=-mom.dir;
coneG.position.x=mom.x*K;coneG.position.z=mom.y*K;coneG.rotation.y=-mom.dir;
const r=viewRange(sneakNow)*K;coneM.scale.set(r,r,1);coneM.material.color.set(seen>0?0xff2222:0xffeb3b);
const cdx=cat.x-pc.x,cdy=cat.y-pc.y;if(cdx*cdx+cdy*cdy>0.01)cA=Math.atan2(cdy,cdx);pc.x=cat.x;pc.y=cat.y;
catG.position.set(cat.x*K,0,cat.y*K);catG.rotation.y=-cA;
for(let i=0;i<5;i++){const f=food[i],m=foodM[i];m.visible=!f.got;if(f.got)continue;
m.position.set(f.x*K,foodH[i]+Math.sin(t*3+i)*0.05,f.y*K);m.rotation.y=t*2;
m.getObjectByName('halo').visible=S.nose>0&&dist(dog,f)<60+50*S.nose}
phoneM.material.color.set(ev&&ev.t==='phone'&&blink()?0xf44336:0x333333);
doorM.material.color.set(ev&&ev.t==='door'&&blink()?0xf44336:0x5d4037);
windowM.material.color.set(ev&&ev.t==='street'&&blink()?0xf44336:0x81d4fa);
const dx=dogG.position.x,dz=dogG.position.z;
const k=Math.min(1,dt*5);
camera.position.x+=(dx-camera.position.x)*k;camera.position.y+=(7.5-camera.position.y)*k;camera.position.z+=(dz+5.5-camera.position.z)*k;
camera.lookAt(camera.position.x,0.5,dz-1.2);
hudSt.textContent='Ур.'+S.lvl+'  Опыт '+S.xp+'/'+S.lvl*40+'  День '+S.day+'  Еда '+stolen+'/3  Доверие '+S.trust;
fill.style.width=Math.min(100,anx)+'%';fill.style.background=anx>70?'#f44336':'#ffb300';
if(msgT>0){msgEl.style.display='block';if(msgEl.textContent!==msg)msgEl.textContent=msg}else msgEl.style.display='none'}
camera.position.set(dog.x*K,7.5,dog.y*K+5.5);
let last=performance.now();
function loop(now){const dt=Math.min(0.05,(now-last)/1000);last=now;update(dt);sync(dt,now/1000);renderer.render(scene,camera);requestAnimationFrame(loop)}
requestAnimationFrame(loop);
