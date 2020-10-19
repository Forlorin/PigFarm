#include "SubWidgetLabel.h"
#include <QDebug>
RecordDisplay::RecordDisplay()
{
    mode =true;
    move(20,20);
    resize(800,600);
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(false);
    ft.setFamily("Microsoft YaHei");
    this->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::Text,QColor(233,233,235));
    this->setPalette(pa);
    setText("Push the button to get the record of buy and sell pigs");

}

void RecordDisplay::bind(SellRecord *SellHead, BuyRecord *BuyHead)
{
    sellHead=SellHead;
    buyhead=BuyHead;
}

void RecordDisplay::displayText()
{
    QString str="";
    if (mode)
    {
        BuyRecord* buyP=buyhead;
        while (buyP!=nullptr) {
            str="Day "+QString::number(buyP->time)+":\n"
                +"  buy "+QString::number(buyP->pigNum)+" pigs\n"
                +"  black pig:"+QString::number(buyP->blackNum)+"\n"
                +"  big spot pig:"+QString::number(buyP->bigSpotNum)+"\n"
                +"  small spot pig:"+QString::number(buyP->smallSpotNum)+"\n"
                +str;
            buyP=buyP->next;
        }
    }
    else
    {
        SellRecord* sellP=sellHead;
        qDebug()<<sellP;
        while (sellP!=nullptr) {
            str="Day "+QString::number(sellP -> time)+":\n"
                +"  Sell "+QString::number(sellP->pigNum)+" pigs,"
                +" get "+QString::number(sellP->money)+"￥\n"
                +str;
            sellP=sellP->next;
        }
    }
    mode= !mode;
    setText(str);
}


DistributionChartLabels::DistributionChartLabels()
{
    mode=true;
    myFarm=nullptr;

    chartTitle.move(450,20);
    chartTitle.resize(400,50);
    QFont ft;
    ft.setPointSize(20);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    chartTitle.setFont(ft);
    chartTitle.setAlignment(Qt::AlignCenter);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(233,233,235));
    chartTitle.setPalette(pa);
    chartTitle.setText("Distribution of Day");
    chartTitle.hide();

    ft.setPointSize(20);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");

    for(int i=0; i<12; i++)
    {
        subjectTitle[i].setAlignment(Qt::AlignCenter);
        subjectTitle[i].setStyleSheet("background-color:rgb(65, 79, 106);");
        subjectTitle[i].move(50+100*i,600);
        subjectTitle[i].resize(90,40);
        subjectTitle[i].setFont(ft);
        subjectTitle[i].setPalette(pa);
        subjectTitle[i].setText(QString::number(i));
        subjectTitle[i].hide();
    }
    subjectTitle[11].setText("∞");
    for(int i=0; i<11; i++)
    {
        chartBar[i].setAlignment(Qt::AlignTop);
        if(i%2)
        {
            chartBar[i].setStyleSheet("background-color:rgb(42, 57, 80);");
        }
        else
        {
            chartBar[i].setStyleSheet("background-color:rgb(61,97,115);");
        }
        chartBar[i].move(150+100*i,100);
        chartBar[i].resize(100,500);
        chartBar[i].setFont(ft);
        chartBar[i].setPalette(pa);
        chartBar[i].hide();
    }
}

void DistributionChartLabels::displayText()
{
    int maxNum=0,length=0;
    double maxHight;
    Pig* p;
    if (mode)
    {

        for(int i=0;i<11;i++)
            dayNum[i]=0;
        int day;
        for(int i=0;i<100;i++)
        {
            p=myFarm[i].head;
            while(p!=nullptr)
            {
                day=p->getDay();
                day=day/36;
                if(day<10)
                {
                    dayNum[day]++;
                }
                else
                {
                    dayNum[10]++;
                }
                p=p->next;
            }
        }
        for(int i=0;i<11;i++)
            if(maxNum<dayNum[i])
                maxNum=dayNum[i];
        if (maxNum==0)
        {
            for(int i=0; i<11;i++)
            {
                chartBar[i].move(150+100*i,600);
                chartBar[i].resize(100,0);

            }
        }
        else
        {
            for(int i=0; i<11; i++)
            {
                maxHight=(0.9+0.1*(maxNum/1000.0))*500.0;
                length=int(maxHight/maxNum*dayNum[i]);
                chartBar[i].move(100+100*i,600-length);
                chartBar[i].resize(100,length);
                if(length>20)
                {
                    chartBar[i].setText(QString::number(dayNum[i]));
                }
                else
                {
                    chartBar[i].setText("");
                }
            }
        }
    }
    else
    {

        for(int i=0;i<11;i++)
            weightNum[i]=0;
        float weight;
        int order;
        for(int i=0;i<100;i++)
        {
            p=myFarm[i].head;
            while(p!=nullptr)
            {
                weight=p->getWeight();
                order=int(weight/7.5);
                if(order<10)
                {
                    weightNum[order]++;
                }
                else
                {
                    weightNum[10]++;
                }
                p=p->next;
            }
        }
        for(int i=0;i<11;i++)
            if(maxNum<weightNum[i])
                maxNum=weightNum[i];
        if (maxNum==0)
        {
            for(int i=0; i<11;i++)
            {
                chartBar[i].move(100+100*i,600);
                chartBar[i].resize(100,0);

            }
        }
        else
        {
            for(int i=0; i<11; i++)
            {
                maxHight=(0.9+0.1*(maxNum/1000.0))*500.0;
                length=int(maxHight/maxNum*weightNum[i]);
                chartBar[i].move(100+100*i,600-length);
                chartBar[i].resize(100,length);
                if(length>20)
                {
                    chartBar[i].setText(QString::number(weightNum[i]));
                }
                else
                {
                    chartBar[i].setText("");
                }
            }
        }
    }
}
/*确定柱状图高度：
 * 计算缩放
 * 最大数值为零时，全部置零。
 * lenth=maxHight/maxNum*num
 *  修正：maxNun<1000
 *      maxHight=(0.9+0.1*(maxNum/1000.0))*500
 * 起点高度：600-length
 */
void DistributionChartLabels::setParent(QWidget* father)
{
    chartTitle.setParent(father);
    for(int i=0; i<12; i++)
    {
        subjectTitle[i].setParent(father);
    }

    for(int i=0; i<11; i++)
    {
        chartBar[i].setParent(father);
    }
}

void DistributionChartLabels::hide()
{
    chartTitle.hide();
    for(int i=0; i<12; i++)
    {
        subjectTitle[i].hide();
    }

    for(int i=0; i<11; i++)
    {
        chartBar[i].hide();
    }
}

void DistributionChartLabels::show()
{
    chartTitle.show();
    for(int i=0; i<12; i++)
    {
        subjectTitle[i].show();
    }

    for(int i=0; i<11; i++)
    {
        chartBar[i].show();
    }
}

void DistributionChartLabels::bindFarm(PigFarm *theFarm)
{
    myFarm=theFarm;
}

void DistributionChartLabels::changeMode()
{
    mode=mode?0:1;
    if(mode)
    {
        chartTitle.setText("Distribution of Day");
        for(int i=0; i<11; i++)
        {
            subjectTitle[i].setText(QString::number(i*36));
        }
    }
    else
    {
        chartTitle.setText("Distribution of Weight");
        for(int i=0; i<11; i++)
        {
            subjectTitle[i].setText(QString::number(i*7.5));
        }
    }
}


TypeChartLabels::TypeChartLabels()
{
    myFarm=nullptr;

    chartTitle.move(450,20);
    chartTitle.resize(400,50);
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    chartTitle.setFont(ft);
    chartTitle.setAlignment(Qt::AlignCenter);

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(233,233,235));
    chartTitle.setPalette(pa);
    chartTitle.setText("Distribution of Pig Types");
    chartTitle.hide();

    typeBar[0].setStyleSheet("background-color:rgb(42,57,80);");
    typeBar[1].setStyleSheet("background-color:rgb(233,233,235);");
    typeBar[2].setStyleSheet("background-color:rgb(61,97,115);");
    for(int i=0; i<3; i++)
    {
        subjectTitle[i].setPalette(pa);
        subjectTitle[i].setFont(ft);
        subjectTitle[i].setStyleSheet("background-color:rgb(65, 79, 106);");
        typeBar[i].hide();
        subjectTitle[i].resize(100,80);
        subjectTitle[i].hide();
    }
}

void TypeChartLabels::displayText()
{
    int pigSum=myFarm->getSum();
    pigNum[0]=myFarm->getBlackSum();
    pigNum[1]=myFarm->getBigspotSum();
    pigNum[2]=pigSum-pigNum[0]-pigNum[1];

    int length;
    int start=20;
    for(int i=0; i<3; i++)
    {
        length=int((pigNum[i]*1000.0)/pigSum);
        typeBar[i].resize(length,100);
        typeBar[i].move(start,300);
        subjectTitle[i].move(start,450);
        start+=length;

    }
     subjectTitle[0].setText("Black:\n"+QString::number(pigNum[0]));
     subjectTitle[1].setText("BigSpot:\n"+QString::number(pigNum[1]));
     subjectTitle[2].setText("SmallSpot:\n"+QString::number(pigNum[2]));
}

void TypeChartLabels::setParent(QWidget* father)
{
    chartTitle.setParent(father);
    for(int i=0; i<3; i++)
    {
        subjectTitle[i].setParent(father);
    }

    for(int i=0; i<3; i++)
    {
        typeBar[i].setParent(father);
    }
}

void TypeChartLabels::bindFarm(PigFarm *theFarm)
{
    myFarm=theFarm;
}

void TypeChartLabels::hide()
{
    chartTitle.hide();
    for(int i=0; i<3; i++)
    {
        subjectTitle[i].hide();
    }

    for(int i=0; i<3; i++)
    {
        typeBar[i].hide();
    }
}

void TypeChartLabels::show()
{
    chartTitle.show();
    for(int i=0; i<3; i++)
    {
        subjectTitle[i].show();
    }

    for(int i=0; i<3; i++)
    {
        typeBar[i].show();
    }
}

