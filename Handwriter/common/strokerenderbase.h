#ifndef STROKERENDERBASE_H
#define STROKERENDERBASE_H

#include <QtCore>

#include "stroke.h"

class StrokeRenderBase {
public:
    void drawStroke(const Stroke& stroke);

protected:
    virtual void render(qreal radius, const QPointF& pos) = 0;
    virtual void render(qreal prePosRadius, qreal posRadius,
                        const QPointF& prePos, const QPointF& pos,
                        qreal prePosStartAngle, qreal prePosEndAngle,
                        qreal posStartAngle, qreal posEndAngle) = 0;
};

#endif  // STROKERENDERBASE_H
