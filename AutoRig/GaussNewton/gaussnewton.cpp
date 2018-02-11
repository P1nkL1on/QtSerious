#include "gaussnewton.h"

GraphicMotion::GraphicMotion()
{
    hei = 50; widperstep = 10; top = 0; left = 0; steps = 20; clr = Qt::red; values = {}; MAX = 5;
}

GraphicMotion::GraphicMotion(QColor clr0)
{
    hei = 50; widperstep = 10; top = 0; left = 0; steps = 20; clr = clr0; values = {}; MAX = 5;
}

GraphicMotion::GraphicMotion(QColor clr0, int hei0, int width, int top0, int left0, int MAX0)
{
    hei = hei0; widperstep = width;top = top0; left = left0; steps = 20; clr = clr0; values = {}; this->MAX = MAX0;
}

int H (int hei, int VAL, int MAX, int MIN){
    if (MAX == MIN)
    {MAX ++; MIN --;}
    return (int)((VAL - MIN) * hei * 1.0 / (MAX - MIN));
}

void GraphicMotion::DrawOn(QPainter *painter)
{
    if (values.length() <= 0)
        return;
    float minValue = -MAX, maxValue = MAX;//values[0], maxValue = values[0];
    for (int i = 0; i < values.length(); i++){
        if (maxValue > values[i]) maxValue = values[i];
        if (minValue < values[i]) minValue = values[i];
    }
    QPen pen = QPen(Qt::gray);
    painter->setPen(pen);
    int WID = widperstep / values.length();
    for (int i = 0; i < values.length(); i++)
        painter->drawLine(left + WID * i, top, left + WID * i, top + hei);

    pen = QPen(clr);
    painter->setPen(pen);
    for (int i = 0; i < values.length() - 1; i++)
        painter->drawLine(left + WID * i, top + hei - H(hei, values[i], maxValue, minValue),
                          left + WID * (i + 1), top + hei - H(hei, values[i + 1], maxValue, minValue));
}
