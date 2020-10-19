#ifndef MYBTN_H
#define MYBTN_H
#include <qpushbutton.h>
#include "pigfarm.h"
#include "prvlabel.h"

class LargePrv:public ::QPushButton
{
    static int slect;
    PigFarm* bindFarm;
    int order;
    static int start;
public:
    PigFarm* getBindFarm(){return bindFarm;}
    void setSlect(int s){slect=s;}
    int getOrder(){return order;}
    void setStart(int i){start=i;}
    void bind(PigFarm* p, int i){bindFarm=p;order =i;}
    void displayText();
    LargePrv();
    int getStart(){return start;}
};

class CruePigBtn:public::QPushButton
{
    PigFarm* bindFarm;
    bool mode;
public:
    int cruePigFarm();
    void changeMode(int Pig);
    void bind(PigFarm* p);
    CruePigBtn();
};

class RestartBtn:public::QPushButton
{
public:
    RestartBtn();
};

class StatisticBtn:public::QPushButton
{
public:
    StatisticBtn();
};

class ContinueBtn:public::QPushButton
{
public:
    ContinueBtn();
};

#endif
