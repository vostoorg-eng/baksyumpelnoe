(function(){
// Ковры
box(40,120,180,50,0.03,0xa1574f,0.02);
box(520,120,200,150,0.03,0x5c7a8c,0.02);
box(330,400,110,60,0.03,0xc0a060,0.02);
box(130,440,140,60,0.03,0x7e57a0,0.02);
box(120,300,70,40,0.03,0xeeeeee,0.02);
// Картины на северной стене
box(420,10,60,1,0.7,0x7e57c2,1.2);
box(425,10,50,1,0.5,0xffcc80,1.3);
box(500,10,60,1,0.7,0xffa726,1.2);
box(505,10,50,1,0.5,0x66bb6a,1.3);
// Торшер у дивана
box(550,36,8,8,1.3,0x424242);
const sh=sph(0.3,1,0.8,1,0xfff59d);sh.position.set(554*K,1.5,40*K);scene.add(sh);
SOLID.push([548,34,12,12]);
// Цветок в прихожей
box(240,20,18,18,0.45,0x6d4c41);
const fl=sph(0.28,1,1.2,1,0x388e3c);fl.position.set(249*K,0.75,29*K);scene.add(fl);
SOLID.push([238,18,22,22]);
// Свет по комнатам
function pl(c,x,z,i){const l=new T.PointLight(c,i,14);l.position.set(x*K,2,z*K);scene.add(l)}
pl(0xffd9a0,550,150,0.45);
pl(0xfff0c0,550,480,0.4);
pl(0xaab4ff,150,500,0.35);
pl(0xffe0b0,150,100,0.3);
// Время суток по дням: утро, день, вечер, ночь
const TOD=[[0xffe9c4,0x3a3a4a,0.6],[0xfff6e8,0x40485a,0.7],[0xffa060,0x2a2030,0.5],[0x8090ff,0x101018,0.3]];
const _rd=resetDay;
resetDay=function(){_rd();const t=TOD[(S.day-1)%4];
sun.color.set(t[0]);sun.intensity=t[2];scene.background.set(t[1])};
resetDay();
})();
