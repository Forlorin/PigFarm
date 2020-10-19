#ifndef PIGFARM_H
#define PIGFARM_H
#include <stdlib.h>
#include <QVector>
enum TYPE {Black, BigSpot, SmallSpot};
class Pig
{
    TYPE type;
    float weight;
    int day;
    bool ifInfected;
public:
    int perPrice;
    Pig* next;
    Pig(TYPE inType, float inWeight, Pig* &head);
    Pig(TYPE inType, float inWeight, Pig* &head, int Day);
    ~Pig();
    void addWeight();
    void infect(){ifInfected=true;}
    void cure(){ifInfected=false;}
    bool isInfect(){return ifInfected;}
    float getWeight(){return weight;}
    int getDay (){return day;}
    int getPrice();
    TYPE getType(){return type;}

};

struct SellRecord
{
    int time;
    int money;
    int pigNum;
    SellRecord *next;
    SellRecord(int Time, int Money, int PigNum)
    {
        time=Time;
        money=Money;
        pigNum=PigNum;
        next=nullptr;
    }
};

struct BuyRecord
{
    int time;
    int pigNum;
    int blackNum;
    int bigSpotNum;
    int smallSpotNum;
    BuyRecord *next;
    BuyRecord(int Time, int PigNum, int BlackNum, int BigSpotNum, int SmallSpotNum)
    {
        time=Time;
        pigNum=PigNum;
        blackNum=BlackNum;
        bigSpotNum=BigSpotNum;
        smallSpotNum=SmallSpotNum;
        next=nullptr;
    }
};

class PigFarm
{
    static int money;
    static int pigSum;
    static int infectedNum;
    static int blackSum;
    static int otherSum;
    static int bigspotSum;
    static int day;

    bool isBlack;
    bool ifInfected;
    int pigNum;

public:
    Pig* head;
    static SellRecord* sellHead;
    static BuyRecord* buyHead;
    PigFarm();
    ~PigFarm();

    bool isbLack(){return isBlack;}
    bool isInfected() {return ifInfected;}
    int getNum(){return pigNum;}
    int getSum(){return pigSum;}
    int getInfectedNum(){return infectedNum;}
    int getBlackSum(){return blackSum;}
    int getOtherSum(){return otherSum;}
    int getMoney(){return money;}
    int getBigspotSum(){return bigspotSum;}
    int getDay(){return day;}
    void setDay(int i){day=i;}
    void setMoney(int i){money=i;}
    void addDay(){day++;}
    Pig *removePig(Pig* outPig);
    void sellPig(int &sellMoney, int &sellNum);
    void addPig(TYPE type,float weight);
    void addPig(TYPE type, float weight, int day);
    void movePig(PigFarm *inFarm);
    bool infect();
    void crue(){ifInfected=false;}
    void addSellRecord(int Time, int Money, int PigNum);
    void cleanSellRecord();
    void addBuyRecord(int Time, int PigNum, int BlackNum, int BigSpotNum, int SmallSpotNum);
    void cleanBuyRecord();
};


#endif // PIGFARM_H
