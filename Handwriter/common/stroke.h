#ifndef STROKE_H
#define STROKE_H

#include <QQmlEngine>
#include <QtCore>

struct Stroke {
    Q_GADGET
    QML_ANONYMOUS

    Q_PROPERTY(QPointF pos MEMBER pos)
    Q_PROPERTY(QPointF prePos MEMBER prePos)
    Q_PROPERTY(int type MEMBER type)
    Q_PROPERTY(int color MEMBER color)
    Q_PROPERTY(qreal size MEMBER size)
    Q_PROPERTY(qreal preSize MEMBER preSize)

public:
    Stroke() = default;
    Stroke(const QPointF& _pos, const QPointF& _prePos, int _type, int _color,
           qreal _size, qreal _preSize);

    QPointF pos{ 0, 0 };
    QPointF prePos{ 0, 0 };
    int type = -1;
    int color = -1;
    qreal size = -1;
    qreal preSize = -1;
};

#endif  // STROKE_H
