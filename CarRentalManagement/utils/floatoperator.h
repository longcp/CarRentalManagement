#ifndef FLOATOPERATOR_H
#define FLOATOPERATOR_H

#include <QObject>

class FloatOperator : public QObject
{
    Q_OBJECT
public:
    explicit FloatOperator(QObject *parent = nullptr);

    float static add2(float a, float b);
    float static minus(float subtractor, float beSubtractor);
    float static multiply2(float a, float b);
    float static divide2(float divisor, float beDivisor);

signals:

public slots:
};

#endif // FLOATOPERATOR_H
