#include "SubWidgetBtn.h"
#include <QDebug>
ShowRecordBtn::ShowRecordBtn()
{
    mode=true;
    move(100,700);
    resize(130,80);
    QFont ft;
    ft.setPointSize(13);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Buy Record");
}

void ShowRecordBtn::changeMode()
{
    if (mode)
        setText("Sell Record");
    else
        setText("Buy Record");
    mode=mode?false:true;
}


ShowDistributionBtn::ShowDistributionBtn()
{
    move(250,700);
    resize(130,80);
    QFont ft;
    ft.setPointSize(13);
    ft.setBold(true);
    ft.setFamily("Microsoft YaHei");
    setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::ButtonText,QColor(233,233,235));
    setPalette(pa);
    setText("Weight\nDistribution");
}

void ShowDistributionBtn::changeMode()
{
    if (mode)
        setText("Weight\nDistribution");
    else
        setText("Day\nDistribution");
    mode=mode?false:true;
}
