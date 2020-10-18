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

namespace Ui
{
    class MainWidget;
}

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

};

#endif // MAINWIDGET_H
