#include "mainwidget.h"
#include "pigfarm.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <stdlib.h>
#include <time.h>
#include <qtimer.h>
#include <qfont.h>
#include <qpalette.h>
#include <QVector>

#define interval 100
/******************初始化**********************/
MainWidget::MainWidget(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::MainWidget)
{

    srand((unsigned)time(NULL));

    ui->setupUi(this);
    this->setStyleSheet("background-color:rgb(65, 79, 106);");
    QPalette paW;
    paW.setColor(QPalette::WindowText,QColor(233,233,235));
    this->setPalette(paW);

    continueBtn.setParent(this);
    restartBtn.setParent(this);
    connect(&continueBtn, &QPushButton::clicked,this,[=](){
        initialize();
        continueBtn.hide();
        restartBtn.hide();
    });
    connect(&restartBtn, &QPushButton::clicked,this,[=](){
        continueBtn.hide();
        restartBtn.hide();
        restart();
        initialize();
    });


}
void MainWidget::initialize()
{
    initializeFarm();
        //加载猪圈
    initializeUi();
        //界面

    FarmTimer = new QTimer(this);
    FarmTimer->stop();
    FarmTimer->setInterval(interval);
    connect(FarmTimer,SIGNAL(timeout()),this, SLOT(updatePig()));
    FarmTimer->start();
        //设置计时器
}

/********************界面************************/
void MainWidget::initializeUi()
{
    for (int i = 0; i<100; i++)
    {
        QPushButton* prst = &farmPrv[i];
        prst->setParent(this);

        prst->setFixedSize(80,80);
        prst->move(20+i%10*100, 20+i/10*100);
        prst->setStyleSheet("background-color:rgb(42, 57, 80);");
        //设置按钮外观：大小、位置，颜色

        QFont ft;
        ft.setPointSize(20);
        ft.setBold(true);
        ft.setFamily("Microsoft YaHei");
        prst->setFont(ft);
        QPalette pa;
        pa.setColor(QPalette::ButtonText,QColor(233,233,235));
        prst->setPalette(pa);
        prst->hide();
        connect(prst, &QPushButton::clicked, this,[=]()
        {
            infPrv.bind(&myFarm[i],i);
            infPrv.disPlayText();
            cruePigBtn.bind(&myFarm[i]);
        });
    }   //预览按钮

    Large.setParent(this);
    Large.setFixedSize(120,80);
    Large.move(1040,820);
    Large.setStyleSheet("background-color:rgb(42, 57, 80);");
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    Large.setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    Large.setPalette(pa);
    Large.setText("Overview");
    Large.show();
    connect(&Large, &QPushButton::clicked, this, [=](){changeMode();});
        //转换按钮

    connect(&cruePigBtn,&QPushButton::clicked, this, [=]()
    {
        cruePigBtn.cruePigFarm();
        cruePigBtn.changeMode(illPigNum);
    });
    cruePigBtn.bind(myFarm);
    cruePigBtn.setParent(this);
    cruePigBtn.show();
        //治愈猪圈按钮
    statisticBtn.setParent(this);
    statisticBtn.show();
    connect(&statisticBtn,&QPushButton::clicked,this,[=](){subWidget.show();});
        //统计按钮
        //重新开始按钮

    for(int i=0; i<20;i++)
    {
        LargePrv* prst=&largePrv[i];
        prst->setParent(this);
        prst->move(20+i%5*200,20+i/5*200);
        prst->bind(&myFarm[i],i);
        prst->displayText();
        prst->show();
        connect(prst,&LargePrv::clicked,this,[=]()
        {
            prst->setSlect(prst->getOrder());
            largePrv[infPrv.getOrder()%20].displayText();
            infPrv.bind(prst->getBindFarm(),prst->getOrder());
            cruePigBtn.bind(prst->getBindFarm());
            prst->displayText();
            infPrv.disPlayText();
        });
    }
        //详细信息按钮
    for(int i=0; i<2;i++)
    {
        QPushButton* prst=&page[i];
        prst->setParent(this);
        prst->move(20+ (i?500 :0),820 );
        prst->setFixedSize(120,80);

        prst->setStyleSheet("background-color:rgb(42, 57, 80);");
        QFont ft;
        ft.setPointSize(30);
        ft.setBold(true);
        ft.setFamily("SimHei");
        prst->setFont(ft);
        QPalette pa;
        pa.setColor(QPalette::ButtonText,QColor(233,233,235));
        prst->setPalette(pa);
        prst->setText(i?">":"<");
        prst->show();
        connect(prst, &QPushButton::clicked, this, [=](){switchPage(i);});
    }
        //翻页按钮
    pageN.setParent(this);
    pageN.move(200,800);
    pageN.setFixedSize(260,120);
    pageN.setAlignment(Qt::AlignCenter);
    QFont ft0;
    ft0.setPointSize(20);
    ft0.setBold(true);
    ft0.setFamily("Microsoft YaHei");
    pageN.setFont(ft0);
    QPalette pa0;
    pa0.setColor(QPalette::WindowText,QColor(233,233,235));
    pageN.setPalette(pa0);
    pageN.setText("1 / 5");
    pageN.show();
        //页数显示
    infPrv.setParent(this);
    infPrv.bind(myFarm,0);
    infPrv.disPlayText();
    infPrv.show();
        //每个猪圈信息预览界面
    allInfLabel.setParent(this);
    allInfLabel.bind(myFarm);
    allInfLabel.displayText();
    allInfLabel.hide();
        //所有猪圈统计总览信息界面
    subWidget.setFather(this);
        //子窗口
    //加载界面
}

/******************加载猪圈***********************/
void MainWidget::initializeFarm()
{
    std::ifstream saveFile;
    saveFile.open("PigFarmSave.txt");

    bool isSaved=false;
    saveFile>>isSaved;

    if(isSaved)
    {
        int day, money,pigNum,typeId;
        TYPE type;
        float weight;
        saveFile>>day>>money;
        myFarm->setDay(day);
        myFarm->setMoney(money);
        for(int i=0; i<FN; i++)
        {
            saveFile>>pigNum;
            for(int j=0; j<pigNum; j++)
            {
                saveFile>>typeId>>weight>>day;
                switch(typeId)
                {
                case 0:
                    type=Black;
                    break;
                case 1:
                    type=BigSpot;
                    break;
                case 2:
                    type= SmallSpot;
                    break;
                }
                myFarm[i].addPig(type,weight,day);
            }
        }
        //重新载入猪圈
        std::ifstream recordFile;
        recordFile.open("buyRecord.txt");
        bool isRecorded;
        recordFile>>isRecorded;
        if(isRecorded)
        {
            if(!recordFile.eof())
            {
                int Time=0, PigNum=0, BlackNum=0, BigSpotNum=0, SmallSpotNum=0;
                recordFile>>Time>>PigNum>>BlackNum>>BigSpotNum>>SmallSpotNum;
                myFarm[0].addBuyRecord(Time, PigNum, BlackNum, BigSpotNum, SmallSpotNum);
            }
        }
        recordFile.close();
        isRecorded=false;
        recordFile.open("sellRecord.txt");
        recordFile>>isRecorded;
        if(isRecorded)
        {
            if(!recordFile.eof())
            {
                int Time=0, Money=0, PigNum;
                recordFile>>Time>>Money>>PigNum;
                if(Time!=0)
                    myFarm[0].addSellRecord(Time,Money,PigNum);
            }
            /*日期 金钱
             * 猪圈情况
             *  猪圈猪的数量
             *     每只猪：种类，体重，饲养天数
             */
        }
        recordFile.close();
    }
    else
    {
        int buyNum = rand()%100+800, pigBuyNum=buyNum;
        int buyBlack=0, buyBigSpot=0;
        while (buyNum--)
        {
            buyPig(myFarm,buyBlack, buyBigSpot);
        }
        myFarm->addBuyRecord(myFarm->getDay(),pigBuyNum,buyBlack,buyBigSpot,pigBuyNum-buyBlack-buyBigSpot);
        myFarm->cleanBuyRecord();
    }
    saveFile.close();

    illPigNum=0;
    existDisease=false;
    //加载猪圈
}

/****************保存数据**********************/
MainWidget::~MainWidget()
{
    /*猪圈数据
     * 日期 金钱 是否有猪瘟 病猪只数
     * 猪圈情况
     *  猪圈猪的数量
     *     每只猪：种类，体重，饲养天数
     *购买数据
     * 时间 卖出猪的数量 黑猪的数量 大花的数量 小花的数量
     */
    PigFarm* p= &myFarm[0];
    Pig* pig=nullptr;
    std::ofstream saveFile;
    saveFile.open("PigFarmSave.txt");
    saveFile<<true<<'\n';
        //文件开头判断是否有存档
    saveFile<<p->getDay()<<' '<<p->getMoney()<<'\n';
    for(int i=0;i<FN;i++)
    {
        p=&myFarm[i];
        saveFile<<p->getNum()<<'\n';
        pig=p->head;
        while (pig!=nullptr)
        {
            saveFile<<pig->getType()<<' '
                    <<pig->getWeight()<<' '
                    <<pig->getDay()<<' '
                    <<'\n';
            pig=pig->next;
        }
    }
    saveFile.close();

    BuyRecord* buyP=p->buyHead;
    saveFile.open("buyRecord.txt");
    saveFile<<true<<'\n';
    while(buyP!=nullptr)
    {
        saveFile<<buyP->time<<' '
                <<buyP->pigNum<<' '
                <<buyP->blackNum<<' '
                <<buyP->bigSpotNum<<' '
                <<buyP->smallSpotNum<<'\n';
        buyP=buyP->next;
    }
    saveFile.close();

    SellRecord* sellP=p->sellHead;
    saveFile.open("sellRecord.txt");
    saveFile<<true<<'\n';
    while(sellP!=nullptr)
    {
        saveFile<<sellP->time<<' '
                <<sellP->money<<' '
                <<sellP->pigNum<<'\n';
        sellP=sellP->next;
    }
    saveFile.close();
    subWidget.hide();
    delete ui;

}


/**************购买、卖出猪与分配猪圈**************/
int distract(PigFarm *myFarm, bool isBlack);    //分配猪

int getLess(PigFarm *myFarm, bool isBlack, int rid = -1); //找到最少的猪圈

bool MainWidget::buyPig(PigFarm *myFarm,int &BlackNum, int &BigSpotNum)
{
    /*购买：
     *  有空猪圈，直接放空猪圈
     *  没空猪圈，放最少的猪圈
     *  放不下了
     *      别的猪圈能腾出空间
     *          腾出空间，放猪
     *      别的猪圈不能腾出空间
     *          返回0，不能买猪
     */
    float weight;
    TYPE type;
    bool isBlack = 0;
    weight = rand() / double(RAND_MAX) * 30 + 20;
    switch (rand()%3)
    {
    case 0:
        type = Black;
        isBlack = 1;
        BlackNum++;
        break;
    case 1:
        type = SmallSpot;
        BigSpotNum++;
        break;
    case 2:
        type = BigSpot;
        break;
    }
    // 生成一只猪
    int less = 0;
    for (int i = 0; i < FN; i++)
    {
        if (myFarm[i].getNum() == 0)
        {
            myFarm[i].addPig(type, weight);
            return true;
        }
    }
    less = getLess(myFarm, isBlack);
    if (myFarm[less].getNum() < 10)
    {
        myFarm[less].addPig(type, weight);
        return true;
    }
    else
    {
        less = distract(myFarm, myFarm[less].isbLack());
        myFarm[less].addPig(type, weight);
        return true;
    }
    return false;
}


int distract(PigFarm *myFarm, bool isBlack)
{
    /*找到最少猪数量的那个猪圈
     *其中猪和别的合并
     *返回空出来的猪圈
     * 参数：isblack：移入的猪是否是黑色
     *
     *腾出猪圈
     *找到要腾出的猪圈类型中的的猪最少的猪圈
     *将其分配到别的猪圈
     */
    isBlack=(!isBlack);
    int out = getLess(myFarm, isBlack);
    PigFarm * outP = &myFarm[out];
    while(outP->getNum()!=0)
    {
        outP->movePig( &myFarm[getLess(myFarm, isBlack, out)] );
    }
    return out;
}


int getLess(PigFarm *myFarm, bool isBlack, int rid)
{
    int less = 0;
    if (rid!=-1)
    {
        for (int i = 1; i < FN; i++)
        {
            if (i==rid)
                continue;
            if (myFarm[i].isbLack() == isBlack && myFarm[i].getNum() < myFarm[less].getNum())
                less = i;
        }
    }
    else
        for (int i = 1; i < FN; i++)
            if (myFarm[i].isbLack() == isBlack && myFarm[i].getNum() < myFarm[less].getNum())
                less = i;
    return less;
}



/********************槽函数**************************/
//翻页
void MainWidget:: switchPage(bool flag)
{
    static int pageNum=1;
    int n,start=(pageNum-1)*20;
    if(flag&&pageNum<5)
    {
        pageN.setText(QString::number(++pageNum)+" / 5");
        largePrv->setStart(start+=20);
        for(int i=0; i<20; i++)
        {
            n= start+i;
            largePrv[i].bind(&myFarm[n],n);
            largePrv[i].displayText();
        }
    }
    else if(!flag&&pageNum>1)
    {
        largePrv->setStart(start-=20);
        for(int i=0; i<20; i++)
        {
            n= start+i;
            largePrv[i].bind(&myFarm[n],n);
            largePrv[i].displayText();
        }
        pageN.setText(QString::number(--pageNum)+" / 5");
    }
    infPrv.bind(&myFarm[start],start);
    infPrv.disPlayText();
    largePrv[0].displayText();
    cruePigBtn.bind(&myFarm[start]);
}
//转换显示格式
void MainWidget::changeMode()
{
    if (allInfLabel.getMode()==overview)
    {
        infPrv.changeMode(detail);
        infPrv.disPlayText();
        allInfLabel.changeMode(detail);
        allInfLabel.displayText();
        Large.setText("Overview");

        int start=infPrv.getOrder();
        if      (0<=start&&start<20)
            start=0;
        else if (20<=start&&start<40)
            start=20;
        else if (40<=start&&start<60)
            start=40;
        else if (60<=start&&start<80)
            start=60;
        else if (80<=start&&start<100)
            start=80;
        largePrv->setStart(start);
        for(int i=0; i<20;i++)
        {
            LargePrv* prst=&largePrv[i];
            prst->show();
            prst->bind(&myFarm[i+start],start+i);
            prst->displayText();
        }
        for(int i=0;i<FN;i++)
            farmPrv[i].hide();
        page[0].show();
        page[1].show();
        pageN.show();
    }//转换到详细信息
    else
    {
        infPrv.changeMode(overview);
        infPrv.disPlayText();
        allInfLabel.changeMode(overview);

        allInfLabel.displayText();
        Large.setText("Detail");
        QPalette pa;
        QPalette pal;
        pal.setColor(QPalette::ButtonText,QColor(240,74,77));
        for(int i=0;i<FN;i++)
        {
            QPushButton* prst = &farmPrv[i];
            QString numText="";
            int num=myFarm[i].getNum();
            if(num<10)
                numText+=char('0'+num);
            else
                numText="10";
            prst->setText(numText);
            if(num)
            {
                if(myFarm[i].isbLack())
                {
                    prst->setStyleSheet("background-color:rgb(42, 57, 80);");
                    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
                    prst->setPalette(pa);
                }
                else
                {
                    prst->setStyleSheet("background-color:rgb(233, 233, 235);");
                    pa.setColor(QPalette::ButtonText,QColor(42,57,80));
                    prst->setPalette(pa);
                }

                if(myFarm[i].isInfected())
                {
                    prst->setPalette(pal);
                }
            }
            else
            {
                prst->setStyleSheet("background-color:rgb(65, 79, 106);");
                pa.setColor(QPalette::ButtonText,QColor(233,233,235));
                prst->setPalette(pa);
            }
            prst->show();
        }
        for(int i=0; i<20;i++)
            largePrv[i].hide();
        page[0].hide();
        page[1].hide();
        pageN.hide();
    }//转换到概览
}
//每秒更新猪圈状态
void MainWidget::updatePig()
{

    static int fatality = 3;
    static int diseaseExistDays=0;

    if(!existDisease && rand()%1000<1)
    {
        int viruseBed=rand()%FN;
        existDisease=myFarm[viruseBed].infect();
        illPigNum=18;
        cruePigBtn.changeMode(illPigNum);
    }

    if(existDisease)
    {
        diseaseExistDays++;
        for(int i=0; i<FN;i++)
        {
            Pig* p= myFarm[i].head;
            while(p!=nullptr)
            {
                if(p->isInfect()&&rand()%100<fatality)
                {
                    p=myFarm[i].removePig(p);
                }
                else
                {
                    p=p->next;
                }
            }
            if (myFarm[i].getNum()==0)
                myFarm[i].crue();

            if (illPigNum==0)
            {
                QString str="The plague ended in ";
                str+=QString::number(diseaseExistDays)+" days.\nYou have ";
                str+=QString::number(myFarm->getSum())+" pigs left";
                existDisease=false;
                cruePigBtn.changeMode(illPigNum);
                QMessageBox box;
                box.setParent(this);
                box.setStyleSheet("background-color:rgb(42, 57, 80);");
                box.setStyleSheet("QLabel{color : rgb(255,255,255);}");
                QFont ft;
                ft.setPointSize(20);
                ft.setFamily("SimHei");
                box.setFont(ft);
                QPalette pa;
                pa.setColor(QPalette::ButtonText,QColor(233,233,235));
                pa.setColor(QPalette::Text,QColor(233,233,235));
                box.setPalette(pa);
                box.setText(str);
                box.exec();
                diseaseExistDays=0;
                break;
            }//疫病结束
        }
        //因疾病病死的猪
        for(int i=0;i<FN;i++)
        {
            if (myFarm[i].isInfected())
            {
                if(i>0 && myFarm[i-1].isInfected()==0 && rand()%100<15)
                {
                    myFarm[i-1].infect();

                }
                if(i<FN-1 && myFarm[i+1].isInfected()==0 && rand()%100<15)
                {
                    myFarm[i+1].infect();

                }
                //疾病在猪圈间的扩散
                //相邻猪圈的猪每天被传染的几率是15%，不相邻的猪圈的猪不传染。
                //每个猪圈，判断周围是否有猪圈，并且感染，猪圈里的猪死光就无法感染

                Pig* p=myFarm[i].head;
                while (p!=nullptr)
                {
                    if(rand()%2)
                    {
                        p->infect();
                    }
                    p=p->next;
                }
                //疾病在猪之间的扩散
                //同一个猪圈的猪每天被传染几率是50%，
            }
            for(int i = 0; i<20; i++)
            {
                largePrv[i].displayText();
            }
        }
        Pig* pig;
        illPigNum=0;
        for(int i=0; i<FN; i++)
        {
            pig=myFarm[i].head;
            while (pig!=nullptr)
            {
                if(pig->isInfect())
                    illPigNum++;
                pig=pig->next;
            }
        }
    }
    //疫病
    myFarm->addDay();
    for(int i=0;i<FN;i++)
    {
        Pig* p = myFarm[i].head;
        while(p!=nullptr)
        {
            p->addWeight();
            p=p->next;
        }
    }
    //每个猪每天的增重，日期

    if(existDisease==0 && (myFarm->getDay()%90==0))
    {
        PigFarm* headFarm=&myFarm[0];
        int sellMoney=0, sellNum=0;
        for(int i=0;i<FN;i++)
        {
            myFarm[i].sellPig(sellMoney, sellNum);
        }
        if(sellNum!=0)
        {
            headFarm->addSellRecord(headFarm->getDay(),sellMoney,sellNum);
        }
        headFarm->cleanSellRecord();

        int buyNum = rand()%100+230;
        int buyBlack, buyBigSpot;
        for(int i=0;i<buyNum;i++)
        {
            buyPig(myFarm,buyBlack, buyBigSpot);
        }

        headFarm->addBuyRecord(headFarm->getDay(),buyNum,buyBlack,buyBigSpot,buyNum-buyBlack-buyBigSpot);
        headFarm->cleanBuyRecord();
        for(int i = 0; i<20; i++)
        {
            largePrv[i].displayText();
        }
    }

    //每隔三个月出一批猪


    QPalette pa;
    if(allInfLabel.getMode()==overview)
    {
        allInfLabel.displayText();
            //全局显示
        QPalette pal;
        pal.setColor(QPalette::ButtonText,QColor(240,74,77));
        for (int i=0;i<FN; i++)
        {
            QPushButton* prst = &farmPrv[i];
            QString numText="";
            int num=myFarm[i].getNum();
            if(num<10)
                numText+=char('0'+num);
            else
                numText="10";
            prst->setText(numText);

            if(myFarm[i].getNum())
            {
                if(myFarm[i].isbLack())
                {
                    prst->setStyleSheet("background-color:rgb(42, 57, 80);");
                    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
                    prst->setPalette(pa);
                }
                else
                {
                    prst->setStyleSheet("background-color:rgb(233, 233, 235);");
                    pa.setColor(QPalette::ButtonText,QColor(42,57,80));
                    prst->setPalette(pa);
                }
                if(myFarm[i].isInfected())
                {
                    prst->setPalette(pal);
                }
            }
            else
            {
                prst->setStyleSheet("background-color:rgb(65, 79, 106);");
                pa.setColor(QPalette::ButtonText,QColor(233,233,235));
                prst->setPalette(pa);
            }
            //按钮显示
        }
    }

    infPrv.disPlayText();
    //侧边显示
    //设置按钮和侧边显示
}
//重新开始
void MainWidget::restart()
{
    std::ifstream saveFile;
    std::ofstream writeFile;
    saveFile.open("PigFarmSave.txt");
    bool isSaved=false;
    saveFile>>isSaved;

    if(isSaved)
    {
        saveFile.close();
        writeFile.open("PigFarmSave.txt");
        writeFile<<false<<'\n';
        writeFile.close();
    }
    else
        saveFile.close();
    //重新载入猪圈

    saveFile.open("buyRecord.txt");
    bool isRecorded;
    saveFile>>isRecorded;
    if(isRecorded)
    {
        saveFile.close();
        writeFile.open("buyRecord.txt");
        writeFile<<false<<'\n';
        writeFile.close();
    }
    else
        saveFile.close();
    //重新载入购买记录

    isRecorded=false;
    saveFile.open("sellRecord.txt");
    saveFile>>isRecorded;
    if(isRecorded)
    {
        saveFile.close();
        writeFile.open("sellRecord.txt");
        writeFile<<false<<'\n';
        writeFile.close();
    }
    else
        saveFile.close();
    //重新载入销售记录
}
/*********************子窗口*************************/
/* 子窗口三个模式
 *  查询记录模式
 *  统计数量模式
 *  直方图模式
 *
 */
//子窗口初始化


SubWidget::SubWidget()
{
    resize(1300,800);
    mode =distribution;
    this->setStyleSheet("background-color:rgb(65, 79, 106);");
    QPalette paW;
    paW.setColor(QPalette::WindowText,QColor(233,233,235));
    this->setPalette(paW);

    showRecord.setParent(this);
    connect(&showRecord,&QPushButton::clicked,this,[=]()
    {
        sellHead= fatherWidget->getSellHead();
        buyHead=fatherWidget->getBuyHead();
        recordDisplay.bind(sellHead,buyHead);
        recordDisplay.displayText();
        showRecord.changeMode();
        subTimer->stop();
        recordDisplay.show();
        distributionChart.hide();
        typeChart.hide();
        mode=record;
    });
    //展示记录按键
    recordDisplay.setParent(this);
    //展示记录文本框
    distributionChart.setParent(this);

    //分布图表
    showDistribution.setParent(this);
    connect(&showDistribution,&QPushButton::clicked,this,[=]()
    {
        distributionChart.bindFarm(fatherWidget->getFarmHead());
        showDistribution.changeMode();
        subTimer->start();
        mode=distribution;
        recordDisplay.hide();
        typeChart.hide();
        distributionChart.show();
        distributionChart.changeMode();
        distributionChart.displayText();
    });
    //展示分布按键

    showType.setParent(this);
    showType.move(470,700);
    showType.resize(130,80);
    QFont ft;
    ft.setPointSize(13);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    showType.setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    showType.setPalette(pa);
    showType.setText("Type\nDistribution");
    connect(&showType,&QPushButton::clicked,this,[=]()
    {
        typeChart.bindFarm(fatherWidget->getFarmHead());
        subTimer->start();
        mode=type;
        recordDisplay.hide();
        distributionChart.hide();
        typeChart.show();
    });
    //展示种类按钮
    typeChart.setParent(this);
    //种类图表
    subTimer= new QTimer;
    subTimer->setParent(this);
    subTimer->setInterval(interval);
    subTimer->stop();
    connect(subTimer,&QTimer::timeout,this,&SubWidget::subUpdate);
    //计时器
}

//获取父窗口指针和数据
void SubWidget::setFather(MainWidget *father)
{
    fatherWidget=father;
    sellHead= father->getSellHead();
    buyHead=father->getBuyHead();
    recordDisplay.bind(sellHead,buyHead);
}

void SubWidget::subUpdate()
{
    if(mode==distribution)
    {
        distributionChart.displayText();
    }
    if(mode==type)
    {
        typeChart.displayText();
    }
}
