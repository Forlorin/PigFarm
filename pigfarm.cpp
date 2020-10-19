#include "pigfarm.h"
#include <QDebug>
#include <stdlib.h>
#include <time.h>

int PigFarm::pigSum=0;
int PigFarm::blackSum=0;
int PigFarm::otherSum=0;
int PigFarm::bigspotSum=0;
int PigFarm::money=1000;
int PigFarm::day=0;
SellRecord* PigFarm::sellHead=nullptr;
BuyRecord* PigFarm::buyHead=nullptr;
//int PigFarm::infectedPig;

PigFarm::PigFarm()
{
    pigNum=0;
    ifInfected=false;
    head=nullptr;
}

PigFarm::~PigFarm()
{
    if (head!=nullptr)
    {
        Pig* p=head->next;
        while(p!=nullptr)
        {
            delete head;
            head=p;
            p=head->next;
        }
        delete head;
    }
}

void PigFarm::addPig(TYPE type,float weight)
{
    this->isBlack=!type;
    pigSum++;
    if(type==0)
        blackSum++;
    else
    {
        otherSum++;
        if(type==BigSpot)
            bigspotSum++;
    }
    pigNum++;
    head = new Pig(type, weight, head);
}

void PigFarm::addPig(TYPE type,float weight, int day)
{
    this->isBlack=!type;
    pigSum++;
    if(type==0)
        blackSum++;
    else
    {
        otherSum++;
        if(type==BigSpot)
            bigspotSum++;
    }
    pigNum++;
    head = new Pig(type, weight, head, day);
}

//指定一只猪，去除之。并且返回被删除猪的下一头猪的指针
Pig* PigFarm::removePig(Pig* outPig)
{
    Pig* P = head;
    pigNum--;
    pigSum--;
    if(isBlack)
        blackSum--;
    else
    {
        otherSum--;
        if(outPig->getType()==BigSpot)
            bigspotSum--;
    }
    if(outPig == head)
    {
        head=head->next;
        delete P;
        P=head;
        return head;
    }
    else
    {
        while(P->next!=outPig)
            P=P->next;
        P->next = outPig->next;
        delete outPig;
        outPig=head;
        return  P->next;
    }
}

void PigFarm::movePig( PigFarm* inFarm)
{
    if(this->head!=nullptr)
    {
        Pig* p=this->head;
        this->head=this->head->next;
        p->next=inFarm->head;
        inFarm->head=p;
        this->pigNum--;
        inFarm->pigNum++;
        if(this->getNum()==0)
            this->isBlack=(!(this->isBlack));
    }

}

void PigFarm::sellPig(int &sellMoney,int &sellNum)
{
    while( head!=nullptr && (head->getDay()>365 || head->getWeight()>75))
    {
        sellMoney +=head->getPrice();
        sellNum++;
        head=removePig(head);
    }
    Pig* p=head;
    while(p!=nullptr)
    {
        if(p->getDay()>365 || p->getWeight()>75)
        {
            sellMoney +=p->getPrice();
            sellNum++;
            p = removePig(p);
        }
        else
            p=p->next;
    }
    money += sellMoney;
}

bool PigFarm::infect()
{
    if(pigNum)
    {
        ifInfected=true;
        head->infect();
        return true;
    }
    return false;
}

void PigFarm::addSellRecord(int Time, int Money, int PigNum)
{
    if (sellHead==nullptr)
    {
        sellHead=new SellRecord(Time, Money, PigNum);
    }
    else
    {
        SellRecord* p=sellHead;
        while (p->next!=nullptr) {
            p=p->next;
        }
        p->next=new SellRecord(Time, Money, PigNum);
    }
}

void PigFarm::cleanSellRecord()
{
    SellRecord* p;
    while(sellHead!=nullptr && sellHead->time>1800)
    {
        p=sellHead;
        sellHead=sellHead->next;
        delete p;
    }
}

void PigFarm::addBuyRecord(int Time, int PigNum, int BlackNum, int BigSpotNum, int SmallSpotNum)
{
    if (buyHead==nullptr)
    {
        buyHead=new BuyRecord(Time, PigNum, BlackNum, BigSpotNum, SmallSpotNum);
    }
    else
    {
        BuyRecord* p=buyHead;
        while (p->next!=nullptr) {
            p=p->next;
        }
        p->next=new BuyRecord(Time, PigNum, BlackNum, BigSpotNum, SmallSpotNum);
    }
}
void PigFarm::cleanBuyRecord()
{
    BuyRecord* p;
    while(buyHead!=nullptr && buyHead->time>1800)
    {
        p=buyHead;
        buyHead=buyHead->next;
        delete p;
    }
}
Pig::Pig(TYPE inType, float inWeight, Pig* &head)
{
    ifInfected=false;
    next=head;
    head=this;
    //用链表将新猪添加到第一个
    type=inType;
    if(inType==Black)
        perPrice=15;
    else if(inType==BigSpot)
        perPrice=6;
    else
        perPrice=7;
    //设定猪的单价
    weight=inWeight;
    day=0;
}
Pig::Pig(TYPE inType, float inWeight, Pig* &head, int Day)
{
    ifInfected=false;
    next=head;
    head=this;
    //用链表将新猪添加到第一个
    type=inType;
    if(inType==Black)
        perPrice=15;
    else if(inType==BigSpot)
        perPrice=6;
    else
        perPrice=7;
    //设定猪的单价
    weight=inWeight;
    day = Day;
}

Pig::~Pig()
{
    next = nullptr;
}

void Pig::addWeight()
{
    weight+=rand()/float(RAND_MAX)*1.2;
    day++;
}

int Pig::getPrice()
{
    return perPrice*weight;
}


