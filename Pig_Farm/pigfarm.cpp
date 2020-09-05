#include "pigfarm.h"

PigFarm::PigFarm()
{

}

Pig::Pig(TYPE inType, float inWeight, Pig* &head)
{
    next=head;
    head=this;
    //用链表将新猪添加到第一个
    type=inType;
    if(inType=0)
        perPrice=15;
    else if(inType=1)
        perPrice=6;
    else
        perPrice=7;
    //设定猪的单价
    weight=inWeight;
    day=0;
}

Pig::~Pig()
{
    next=nullptr;
}
