#ifndef SUBWIDGETLABEL_H
#define SUBWIDGETLABEL_H
#include <QLabel>
#include <QString>
#include <QTextEdit>
#include <pigfarm.h>
#include <QWidget>

enum SubMode{record, distribution,type};

class RecordDisplay:public::QTextEdit
{
    bool mode;//true为购买数据，false为销售数据
    SellRecord* sellHead;
    BuyRecord* buyhead;
public:
    RecordDisplay();
    void bind(SellRecord *SellHead, BuyRecord* BuyHead);
    void displayText();
};

class DistributionChartLabels
{
    bool mode;//1:day,0:weight
    int dayNum[11];
    int weightNum[11];
    PigFarm* myFarm;
    QLabel chartTitle;
    QLabel subjectTitle[12];
    QLabel chartBar[11];

public:
    DistributionChartLabels();
    void setParent(QWidget* father);
    void bindFarm(PigFarm* theFarm);
    void hide();
    void show();
    void displayText();
    void changeMode();
};

class TypeChartLabels
{
    int pigNum[3];
    PigFarm* myFarm;
    QLabel chartTitle;
    QLabel subjectTitle[3];
    QLabel typeBar[3];
public:
    TypeChartLabels();
    void setParent(QWidget* father);
    void bindFarm(PigFarm* theFarm);
    void hide();
    void show();
    void displayText();
};
#endif // SUBWIDGETLABEL_H
