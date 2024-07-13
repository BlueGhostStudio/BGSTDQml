#include "stroke.h"

Stroke::Stroke(const QPointF& _pos, const QPointF& _prePos, int _type,
               int _color, qreal _size, qreal _preSize)
    : pos(_pos),
      prePos(_prePos),
      type(_type),
      color(_color),
      size(_size),
      preSize(_preSize) {}

QColor
Stroke::darkColor(const QColor& color) {
    float h, s, l;
    color.getHslF(&h, &s, &l);

    return QColor::fromHslF(h, s, 1 - l);
}
