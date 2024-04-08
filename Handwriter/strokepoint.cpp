#include "strokepoint.h"

StrokePoint::StrokePoint(int _event, const QPointF& pos, float vel)
    : event(_event), position(pos), velocity(vel) {}

StrokePoint::StrokePoint(int _event, const QEventPoint& point)
    : event(_event), position(point.position()), velocity(point.velocity().length()) {}
