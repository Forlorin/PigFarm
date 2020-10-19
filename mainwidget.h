#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#define FN 100 //the number of farm

#include <QWidget>
#include <pigfarm.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QString>
#include <iostream>
#include <fstream>
#include <iostream>
#include "prvlabel.h"
#include "myBtn.h"
#include <QMessageBox>
#include "SubWidgetLabel.h"
#include "SubWidgetBtn.h"

namespace Ui
{
    class MainWidget;
}

class MainWidget;

class SubWidget : public QWidget
{
    Q_OBJECT
private:
    QTimer* subTimer;
    MainWidget* fatherWidget;
    RecordDisplay recordDisplay;
    SellRecord* sellHead;
    BuyRecord* buyHead;
    ShowRecordBtn showRecord;
    ShowDistributionBtn showDistribution;
    DistributionChartLabels distributionChart;
    QPushButton showType;
    TypeChartLabels typeChart;
    SubMode mode;

public:
    void setFather(MainWidget *father);
    SubWidget();
private:
    void subUpdate();
};

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    Ui::MainWidget *ui;

    PigFarm myFarm[FN];
    QTimer* FarmTimer;
    QPushButton farmPrv[FN];
    LargePrv largePrv[20];
    QPushButton Large;
    QPushButton page[2];
    QLabel pageN;
    PrvLabel infPrv;
    allPrv allInfLabel;
    CruePigBtn cruePigBtn;
    RestartBtn restartBtn;
    ContinueBtn continueBtn;
    StatisticBtn statisticBtn;
    SubWidget subWidget;
    int illPigNum;
    bool existDisease;

private slots:
    void updatePig();
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    bool buyPig(PigFarm* myFarm, int &BlackNum, int &BigSpotNum);
    void changeMode();
    void switchPage(bool flag);
    void initialize();
    void initializeUi();
    void initializeFarm();
    PigFarm* getFarmHead(){return &myFarm[0];}
    SellRecord* getSellHead(){return myFarm->sellHead;}
    BuyRecord* getBuyHead(){return myFarm->buyHead;}
    void restart();
};


#endif // MAINWIDGET_H
