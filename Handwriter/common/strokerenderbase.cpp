#include "strokerenderbase.h"

void
StrokeRenderBase::drawStroke(const Stroke& stroke) {
    qreal dx = stroke.pos.x() - stroke.prePos.x();
    qreal dy = stroke.pos.y() - stroke.prePos.y();
    qreal dist = qPow(dx * dx + dy * dy, 0.5);

    qreal tr = qAtan2(-dy, dx);  // 目标角度, Target Radians
    qreal r1 = stroke.size / 2;  // 末尾圆弧半径
    qreal r0 = stroke.type == 0 ? r1 : stroke.preSize / 2;  // 开始圆弧半径

    qreal minR = qMin(r0, r1);
    qreal maxR = qMax(r0, r1);

    if (dist + minR <= maxR) {
        //        int sampling = 16;

        QPointF cp;
        if (r0 == maxR)
            cp = stroke.prePos;
        else
            cp = stroke.pos;

        render(maxR, cp);
    } else {
        // int sampling = 16;
        qreal sr = (dist > 0) ? qAcos((r0 - r1) / dist) : 0;

        qreal sa0 = sr + tr;
        qreal ea0 = 2 * M_PI - sr + tr;
        // qreal ur0 = qAbs(ea0 - sa0) / sampling;

        qreal sa1 = ea0;
        qreal ea1 = sa0;

        QPointF cp0 = stroke.prePos;

        QPointF cp1 = stroke.pos;

        render(r0, r1, cp0, cp1, sa0, ea0, sa1, ea1);
    }
}
