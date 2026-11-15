if(S.qDone===undefined)S.qDone=0;
const q={react:0,chased:false,done:false,t0:0};
function trustSum(){return S.trust+S.tD+S.tS}
const QS=[
{n:'Стащи колбасу со стола',c:()=>food[0].got===1},
{n:'Поиграй с Сыном в канат',c:()=>son.cd>19.5},
{n:'Отреагируй на 3 события',c:()=>q.react>=3,p:()=>q.react+'/3'},
{n:'Подлижись к Отцу',c:()=>dad.cd>11.5},
{n:'Прогони кота лаем',c:()=>q.chased},
{n:'Стащи 3 вкусняшки и не попадись',c:()=>stolen>=3&&trustSum()>=q.t0,f:()=>trustSum()<q.t0}
];
function curQ(){return QS[(S.day-1)%QS.length]}
const _g=gain;
gain=function(n){if(n===8)q.react++;_g(n)};
const _reset2=resetDay;
resetDay=function(){_reset2();q.react=0;q.chased=false;q.done=false;q.t0=trustSum();
msg='День '+S.day+'. Задание: '+curQ().n;msgT=5};
function checkQ(){
if(cat.st==='flee')q.chased=true;
const Q=curQ();
if(!q.done&&Q.c()){q.done=true;S.qDone++;S.trust=Math.min(100,S.trust+2);gain(20);
say('Задание выполнено! +20 опыта, доверие +2');
if(dayDone){msg='День пройден! Задание выполнено! Нажми ВЗЯТЬ';msgT=999}save()}}
const _upd2=update;
update=function(dt){_upd2(dt);if(menuOpen)return;checkQ()};
const qEl=document.createElement('div');
qEl.style.cssText='position:fixed;left:10px;top:64px;max-width:60vw;color:#ffe082;font-size:13px;text-shadow:0 0 3px #000,0 0 3px #000;pointer-events:none';
document.body.appendChild(qEl);
const _sync2=sync;
sync=function(dt,t){_sync2(dt,t);const Q=curQ();
qEl.textContent='★ '+Q.n+(q.done?' ✔':(Q.f&&Q.f()?' ✖ провалено':(Q.p?' ('+Q.p()+')':'')))};
const _rm2=renderMenu;
renderMenu=function(){_rm2();document.getElementById('menu').insertAdjacentHTML('beforeend','<p>Задание дня: '+curQ().n+'<br>Выполнено заданий: '+S.qDone+'</p>')};
resetDay();
