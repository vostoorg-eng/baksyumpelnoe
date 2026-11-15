const PACE=0.7;
const _rc=resetDay;
resetDay=function(){_rc();evT=22};
const _uc=update;
update=function(dt){
const bx=dog.x,by=dog.y,pe=ev;
_uc(dt*PACE);
if(!menuOpen&&!dayDone){
const mx=dog.x-bx,my=dog.y-by;
if(Math.abs(mx)+Math.abs(my)<15){
const k=1/PACE-1,nx=dog.x+mx*k,ny=dog.y+my*k;
if(!hit(nx,dog.y))dog.x=nx;
if(!hit(dog.x,ny))dog.y=ny}}
if(pe&&!ev)evT+=8};
resetDay();
