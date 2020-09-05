#ifndef PIGFARM_H
#define PIGFARM_H
enum TYPE {Black, BigSpot, SmallSpot};
class Pig
{
    TYPE type;
    float weight;
    int day;
    int perPrice;
    Pig* next;
public:
    Pig(TYPE inType, float inWeight, Pig* &head);
    ~Pig();
}
class PigFarm
{
    static int pigSum;
    static int blackSum;
    static int otherSum;
    int pigNum;
    Pig* head;
public:
    PigFarm();
};

#endif // PIGFARM_H
