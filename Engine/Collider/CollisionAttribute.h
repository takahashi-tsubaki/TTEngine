#pragma once

//当たり判定属性
//地面
const unsigned short COLLISION_ATTR_LANDSPHERE = 0b1 << 0;
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 1;
//自機
const unsigned short COLLISION_ATTR_PLAYERS = 0b1 << 2;
//自機の弾
const unsigned short COLLISION_ATTR_PLAYERBULLETS = 0b1 << 3;
//敵
const unsigned short COLLISION_ATTR_ENEMYS = 0b1 << 4;
//敵の弾
const unsigned short COLLISION_ATTR_ENEMYBULLETS = 0b1 << 5;
//敵のレーザー攻撃
const unsigned short COLLISION_ATTR_ENEMYLASERS = 0b1 << 6;
