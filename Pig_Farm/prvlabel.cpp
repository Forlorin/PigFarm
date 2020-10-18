#include "prvlabel.h"
#include <QString>
#include <qdebug.h>

PrvLabel::PrvLabel()
{
    bindFarm=nullptr;
    mode=detail;

    this->setStyleSheet("background-color:rgb(42, 57, 80);");
    this->setAlignment(Qt::AlignTop);

    this->move(1040,20);
    this->setFixedSize(500,780);

    QFont ft;
    ft.setPointSize(12);
    ft.setBold(false);
    ft.setFamily("Microsoft YaHei");
    this->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(233,233,235));
    this->setPalette(pa);
}
void PrvLabel::changeMode(Mode m)
{
    mode=m;
    if(m==detail)
    {
        this->move(1040,20);
        this->setFixedSize(500,780);
    }
    else
    {
        this->move(1040,440);
        this->setFixedSize(500,360);
    }
    disPlayText();
}
void PrvLabel::disPlayText()
{
    QString display="Pigpen No.";
    display+=QString::number(order,10);

    if (mode==overview)
    {
        if(bindFarm->isInfected()&&bindFarm->getNum()==0)
        {
            QFont ft;
            ft.setPointSize(20);
            ft.setBold(true);
            ft.setFamily("Microsoft YaHei");
            this->setFont(ft);

            QPalette pa;
            pa.setColor(QPalette::WindowText,QColor(240,74,77));
            this->setPalette(pa);
            display+="\n\nNO .\n PIG .\n  LIVES .";

        }
        else
        {
            QFont ft;
            ft.setPointSize(12);
            ft.setBold(false);
            ft.setFamily("Microsoft YaHei");
            this->setFont(ft);
            QPalette pa;
            pa.setColor(QPalette::WindowText,QColor(233,233,235));
            this->setPalette(pa);
            if(bindFarm->getNum())
            {
                display+=", "+QString::number(bindFarm->getNum(),10)+" pigs inside.";
                display+="\n\n";
                display+="There are:\n";
                if(bindFarm->isbLack())
                {
                    display+="Black pig: "+QString::number(bindFarm->getNum(),10)+"\n";
                }
                else
                {
                    int bigN=0, smallN=0;
                    for (Pig* p= bindFarm->head; p!=nullptr; p=p->next)
                    {
                        switch (p->getType())
                        {
                        case BigSpot:
                            bigN++;
                            break;
                        default:
                            smallN++;
                            break;
                        }
                    }
                    display+="Big Spot pig: "+QString::number(bigN,10)+"\n";
                    display+="Small Spot pig: "+QString::number(smallN,10)+"\n";
                }
            }
            else
            {
                display+="\n\nThis pigpen have no pig. ";
            }
        }
    }
    else if(mode==detail)
    {
        if(bindFarm->isInfected())
        {
            QFont ft;
            ft.setPointSize(12);
            ft.setBold(false);
            ft.setFamily("Microsoft YaHei");
            this->setFont(ft);

            QPalette pa;
            pa.setColor(QPalette::WindowText,QColor(240,74,77));
            this->setPalette(pa);
        }
        else
        {
            QFont ft;
            ft.setPointSize(12);
            ft.setBold(false);
            ft.setFamily("Microsoft YaHei");
            this->setFont(ft);

            QPalette pa;
            pa.setColor(QPalette::WindowText,QColor(233,233,235));
            this->setPalette(pa);
        }
        display+=", "+QString::number(bindFarm->getNum(),10)+" pigs inside.";
        display+="\n\n";
        Pig* prst = bindFarm->head;
        int i = 0;
        while(prst!=nullptr)
        {
            display+="No."+QString::number(i,10)+"，";
            switch(prst->getType())
            {
                case Black:display+="black pig:\n";break;
                case BigSpot:display+="big spot pig:\n";break;
                case SmallSpot:display+="small spot pig:\n";
            }
            display+=QString::number(prst->getDay(),10)+" day, ";
            display+=QString("%1 Kg\n").arg(prst->getWeight());
            i++;
            prst=prst->next;
        }
    }

    setText(display);
}

allPrv::allPrv()
{
    myFarm=nullptr;
    mode=detail;

    this->move(1040,20);
    this->setFixedSize(500,400);

    this->setStyleSheet("background-color:rgb(42, 57, 80);");
    this->setAlignment(Qt::AlignTop);


    QFont ft;
    ft.setPointSize(11);
    ft.setBold(false);
    ft.setFamily("Microsoft YaHei");
    this->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(233,233,235));
    this->setPalette(pa);
}

void allPrv::changeMode(Mode m)
{
    mode = m;
    if (m==overview)
    {
        this->show();
        this->move(1040,20);
        this->setFixedSize(500,400);
    }
    else if (m==detail)
    {
        this->hide();
    }
}
void allPrv::displayText()
{
    QString display="Your farm now have ";
    if(mode==overview)
    {
        display +=QString::number(myFarm->getMoney(),10)+"￥, \nand ";
        display +=QString::number(myFarm->getBlackSum()+myFarm->getOtherSum(),10)+" pigs.\n\n";
        display +="There are \n";
        display +=QString::number(myFarm->getBlackSum(),10)+" black pigs,\n";
        display +=QString::number(myFarm->getBigspotSum(),10)+" big spot pigs,\n";
        display +=QString::number(myFarm->getOtherSum()-myFarm->getBigspotSum(),10)+" small spot pigs \nin your farm.\n\n";
        display +="Now, it is the day "+QString::number(myFarm->getDay(),10);
    }
    else if (mode == detail)
    {
        display +=QString::number(myFarm->getMoney(),10)+"￥, and ";
        display +=QString::number(myFarm->getBlackSum()+myFarm->getOtherSum(),10)+" pigs.\n";
        display +="There are ";
        display +=QString::number(myFarm->getBlackSum(),10)+" black pigs,\n";
        display +=QString::number(myFarm->getBigspotSum(),10)+" big spot pigs,\n";
        display +=QString::number(myFarm->getOtherSum()-myFarm->getBigspotSum(),10)+" small spot pigs in your farm.\n";
    }

    setText(display);
}
