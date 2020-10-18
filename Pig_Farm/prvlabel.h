#ifndef PRVLABEL_H
#define PRVLABEL_H
#include <qlabel.h>
#include <pigfarm.h>

enum Mode {detail, overview};
class PrvLabel : public::QLabel
{
    PigFarm* bindFarm;
    int order;
    Mode mode;
public:
    PrvLabel();
    int getOrder(){return order;}
    void changeMode(Mode m);
    void bind(PigFarm* p, int i){bindFarm=p; order=i;}
    void disPlayText();
};

class allPrv :public::QLabel
{
    PigFarm* myFarm;
    Mode mode;
public:
    allPrv();
    int getMode(){return mode;}
    void changeMode(Mode m);
    void bind(PigFarm* p){myFarm=p;}
    void displayText();
};

#endif // PRVLABEL_H
