#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

typedef unsigned char byte;
typedef unsigned int uint;

//APPN Markers
const byte APP0 = 0xE0;
const byte APP1 = 0xE1;
const byte APP2 = 0xE2;
const byte APP3 = 0xE3;
const byte APP4 = 0xE4;
const byte APP5 = 0xE5;
const byte APP6 = 0xE6;
const byte APP7 = 0xE7;
const byte APP8 = 0xE8;
const byte APP9 = 0xE9;
const byte APP10 = 0xEA;
const byte APP11 = 0xEB;
const byte APP12 = 0xEC;
const byte APP13 = 0xED;
const byte APP14 = 0xEE;
const byte APP15 = 0xEF;

// other markers
const byte SOI = 0xD8;
const byte EOI = 0xD9;
const byte SOS = 0xDA;
const byte DOT = 0xDB;
const byte DNL = 0xDC;
const byte DRI = 0xDD;
const byte DHP = 0xDE;
const byte EXP = 0xDF;

struct Header {
    bool valid = true;
};
#endif