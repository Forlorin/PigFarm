#include "myBtn.h"
#include <qstring.h>
#include <qdebug.h>

int LargePrv::start =0;
int LargePrv::slect =0;

LargePrv::LargePrv()
{
    bindFarm=nullptr;
    order =0;
    this->setFixedSize(160,160);
    this->setStyleSheet("border- image: url(:/new/prefix1/pic/wn.png)");
    QFont ft;
    ft.setPointSize(25);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    this->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    this->setPalette(pa);

}

void LargePrv::displayText()
{

    QString display=QString::number(order)+"\n \n";   
    this->setText(display);
    QString path;
    path+=bindFarm->isbLack()?"b":"w";
    path+=bindFarm->isInfected()?"l":"n";
    if(slect==order) path+="s";
    path="border-image: url(:/new/prefix1/pic/"+path+".png)";
    this->setStyleSheet(path);
}

CruePigBtn::CruePigBtn()
{
    bindFarm=nullptr;
    mode=false;
    setFixedSize(120,80);
    move(1040,920);
    setStyleSheet("background-color:rgb(42, 57, 80);");
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Healthy");
}

void CruePigBtn::bind(PigFarm* p)
{
    bindFarm=p;
}

void CruePigBtn::changeMode(int illPig)
{
    if(illPig==0)
    {
        mode=true;
    }
    else
    {
        mode=false;
    }
    if(mode)
    {
        QPalette pa;
        pa.setColor(QPalette::ButtonText,QColor(233,233,235));
        setPalette(pa);
        setText("Healthy");
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::ButtonText,QColor(240,74,77));
        setPalette(pa);
        setText("Cure");
    }
}

int CruePigBtn::cruePigFarm()
{
    Pig* p=bindFarm->head;
    int count = 0;
    while (p!=nullptr)
    {
        p -> cure();
        p=p->next;
        count++;
    }
    bindFarm->crue();
    return count;
}


StatisticBtn::StatisticBtn()
{
    setFixedSize(120,80);
    move(1180,920);
    setStyleSheet("background-color:rgb(42, 57, 80);");
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Statistic");
}

RestartBtn::RestartBtn()
{
    setFixedSize(160,100);
    move(720,300);
    setStyleSheet("background-color:rgb(42, 57, 80);");
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Restart");
}


ContinueBtn::ContinueBtn()
{
    setFixedSize(160,100);
    move(720,500);
    setStyleSheet("background-color:rgb(42, 57, 80);");
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Continue");
}

