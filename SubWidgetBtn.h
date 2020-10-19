#ifndef SUBWIDGETBTN_H
#define SUBWIDGETBTN_H
#include "pigfarm.h"
#include <QPushButton>
#include <QString>

class ShowRecordBtn:public::QPushButton
{
    bool mode;
public:
    ShowRecordBtn();
    void changeMode();
};

class ShowDistributionBtn:public::QPushButton
{
    bool mode;//1:day,0:weight
public:
    ShowDistributionBtn();
    void changeMode();
};


#endif // SUBWIDGETBTN_H
