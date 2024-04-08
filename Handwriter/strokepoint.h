#ifndef STROKEPOINT_H
#define STROKEPOINT_H

#include <QEventPoint>
#include <QQmlEngine>

struct StrokePoint {
    Q_GADGET
    QML_ANONYMOUS

    Q_PROPERTY(int event MEMBER event)
    Q_PROPERTY(QPointF position MEMBER position)
    Q_PROPERTY(float velocity MEMBER velocity)
    // Q_PROPERTY(qreal ellipseDiameters MEMBER ellipseDiameters)
    // Q_PROPERTY(qreal pressure MEMBER pressure)

public:
    StrokePoint() = default;
    StrokePoint(int _event, const QPointF& pos, float vel);
    StrokePoint(int _event, const QEventPoint& point);

    int event;
    QPointF position;
    float velocity;

    // qreal m_ellipseDiameters;
    // qreal m_pressure;
};

#endif  // STROKEPOINT_H
