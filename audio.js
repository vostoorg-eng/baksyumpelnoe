let ac=null,master=null,musG=null,sfxG=null,noiseBuf=null,muted=false,nextT=0,stepI=0,stepT=0;
try{muted=localStorage.getItem('bax_mute')==='1'}catch(e){}
function initAudio(){if(ac)return;const AC=window.AudioContext||window.webkitAudioContext;if(!AC)return;
ac=new AC();master=ac.createGain();master.gain.value=muted?0:0.9;master.connect(ac.destination);
const lp=ac.createBiquadFilter();lp.type='lowpass';lp.frequency.value=2200;lp.connect(master);
musG=ac.createGain();musG.gain.value=0.2;musG.connect(lp);
sfxG=ac.createGain();sfxG.gain.value=0.7;sfxG.connect(master);
noiseBuf=ac.createBuffer(1,ac.sampleRate,ac.sampleRate);const d=noiseBuf.getChannelData(0);for(let i=0;i<d.length;i++)d[i]=Math.random()*2-1}
function ok(){return ac&&ac.state==='running'&&!muted}
function tone(f,t,d,type,vol,dest,f2){dest=dest||sfxG;const o=ac.createOscillator(),gn=ac.createGain();o.type=type;o.frequency.setValueAtTime(f,t);if(f2)o.frequency.exponentialRampToValueAtTime(f2,t+d);
gn.gain.setValueAtTime(0.0001,t);gn.gain.exponentialRampToValueAtTime(vol,t+0.02);gn.gain.exponentialRampToValueAtTime(0.0001,t+d);
o.connect(gn);gn.connect(dest);o.start(t);o.stop(t+d+0.05)}
function noiz(t,d,vol,freq,type){const s=ac.createBufferSource();s.buffer=noiseBuf;s.loop=true;const fl=ac.createBiquadFilter();fl.type=type||'bandpass';fl.frequency.value=freq;const gn=ac.createGain();
gn.gain.setValueAtTime(vol,t);gn.gain.exponentialRampToValueAtTime(0.0001,t+d);s.connect(fl);fl.connect(gn);gn.connect(sfxG);s.start(t);s.stop(t+d+0.05)}
const SND={
bark(v){if(!ok())return;const t=ac.currentTime,k=v||1;tone(420,t,0.16,'sawtooth',0.35*k,null,180);noiz(t,0.12,0.5*k,900);tone(380,t+0.22,0.14,'sawtooth',0.28*k,null,170);noiz(t+0.22,0.1,0.4*k,900)},
farBark(){SND.bark(0.4)},
meow(){if(!ok())return;const t=ac.currentTime,o=ac.createOscillator(),gn=ac.createGain();o.type='triangle';o.frequency.setValueAtTime(550,t);o.frequency.linearRampToValueAtTime(900,t+0.18);o.frequency.linearRampToValueAtTime(520,t+0.55);gn.gain.setValueAtTime(0.0001,t);gn.gain.linearRampToValueAtTime(0.3,t+0.08);gn.gain.linearRampToValueAtTime(0.0001,t+0.55);o.connect(gn);gn.connect(sfxG);o.start(t);o.stop(t+0.6)},
hiss(){if(!ok())return;noiz(ac.currentTime,0.35,0.4,5000,'highpass')},
phone(){if(!ok())return;const t=ac.currentTime;for(let i=0;i<4;i++)tone(i%2?1250:1000,t+i*0.18,0.14,'square',0.12)},
knock(){if(!ok())return;const t=ac.currentTime;for(let i=0;i<3;i++){tone(140,t+i*0.2,0.12,'sine',0.7,null,60);noiz(t+i*0.2,0.06,0.4,300,'lowpass')}},
vacuum(){if(!ok())return;const t=ac.currentTime;noiz(t,1.6,0.35,250,'lowpass');tone(95,t,1.6,'sawtooth',0.15)},
pick(){if(!ok())return;const t=ac.currentTime;tone(660,t,0.1,'triangle',0.4);tone(990,t+0.09,0.16,'triangle',0.4)},
level(){if(!ok())return;const t=ac.currentTime;[523,659,784,1047].forEach((f,i)=>tone(f,t+i*0.1,0.25,'triangle',0.4))},
scold(){if(!ok())return;tone(320,ac.currentTime,0.4,'sawtooth',0.3,null,140)},
whine(){if(!ok())return;const t=ac.currentTime;tone(750,t,0.5,'sine',0.3,null,380);tone(700,t+0.55,0.5,'sine',0.25,null,330)},
squeak(){if(!ok())return;tone(1500,ac.currentTime,0.09,'square',0.1,null,2100)},
ding(){if(!ok())return;const t=ac.currentTime;tone(880,t,0.3,'sine',0.3);tone(1320,t+0.08,0.3,'sine',0.2)},
step(){if(!ok())return;noiz(ac.currentTime,0.05,0.12,350,'lowpass')}};
const EVSND={phone:'phone',door:'knock',courier:'knock',guest:'knock',street:'farBark',vacuum:'vacuum'};
const CH=[[57,60,64],[53,57,60],[60,64,67],[55,59,62]],BS=[45,41,48,43],MEL=[69,72,74,76,79];
function mtof(m){return 440*Math.pow(2,(m-69)/12)}
function musicStep(i,t){const c=Math.floor(i/8)%4,k=i%8;
if(k===0||k===4)tone(mtof(BS[c]),t,1.2,'triangle',0.7,musG);
if(k%2===0)tone(mtof(CH[c][(k>>1)%3]+12),t,0.9,'sine',0.35,musG);
if(k%2===1&&Math.random()<0.3)tone(mtof(MEL[Math.floor(Math.random()*5)]),t,0.6,'triangle',0.25,musG);
if(anx>70&&k===0)tone(mtof(BS[c]+1),t,1,'sawtooth',0.15,musG)}
function sched(){if(!ok())return;const now=ac.currentTime;if(nextT<now)nextT=now+0.05;
while(nextT<now+0.25){musicStep(stepI,nextT);nextT+=anx>70?0.3:0.42;stepI++}}
setInterval(sched,100);
function unlock(){try{initAudio();if(ac&&ac.state!=='running')ac.resume()}catch(e){}}
['touchend','pointerup','click','keydown'].forEach(n=>addEventListener(n,unlock,{passive:true}));
const mb2=document.createElement('button');mb2.textContent=muted?'🔇':'🔊';
mb2.style.cssText='top:10px;right:84px;width:40px;height:40px;border-radius:10px;background:#c5e1a5;font-size:18px;z-index:5';
document.body.appendChild(mb2);
mb2.addEventListener('pointerdown',e=>{e.preventDefault();muted=!muted;mb2.textContent=muted?'🔇':'🔊';if(master)master.gain.value=muted?0:0.9;try{localStorage.setItem('bax_mute',muted?'1':'0')}catch(x){}});
const _ua=update;
update=function(dt){
const bk=barkP&&!menuOpen&&!dayDone,pe=ev,pst=stolen,plv=S.lvl,ptr=S.trust+S.tD+S.tS,pcs=cat.st,pp=panic>0,psh=shakeCd,pty=toy.carried,pdd=dayDone;
_ua(dt);
if(!ok())return;
if(bk)SND.bark();
if(ev&&ev!==pe){const fn=SND[EVSND[ev.t]];if(fn)fn()}
if(stolen>pst||(toy.carried&&!pty))SND.pick();
if(S.lvl>plv||(dayDone&&!pdd))SND.level();
const tr=S.trust+S.tD+S.tS;
if(tr<ptr)SND.scold();else if(tr>ptr&&!(dayDone&&!pdd))SND.ding();
if(cat.st==='hunt'&&pcs!=='hunt')SND.meow();
if(cat.st==='flee'&&pcs!=='flee')SND.hiss();
if(panic>0&&!pp)SND.whine();
if(shakeCd>psh+1)SND.squeak();
if(dog.moving&&!sneakNow&&!menuOpen&&!dayDone){stepT-=dt;if(stepT<=0){stepT=0.32;SND.step()}}};
document.addEventListener('visibilitychange',()=>{if(!ac)return;if(document.hidden)ac.suspend();else ac.resume()});
