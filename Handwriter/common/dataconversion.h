#ifndef DATACONVERSION_H
#define DATACONVERSION_H

#include <QObject>
#include <QPointF>
#include "stroke.h"

/*struct t_stroke {
    QPointF pos{ 0, 0 };
    QPointF prePos{ 0, 0 };
    int type = -1;
    int color = -1;
    qreal size = -1;
    qreal preSize = -1;
};*/

Stroke variant2Stroke(const QVariant& stroke);
QVariant stroke2Variant(const Stroke& stroke);

Stroke byteArray2Stroke(const QByteArray& data);
QVariant byteArray2Variant(const QByteArray& data);

QByteArray stroke2ByteArray(const Stroke& stroke);
QByteArray variant2ByteArray(const QVariant& variant);

QList<Stroke> b2sl(const QByteArray& data);
QVariantList b2vl(const QByteArray& data);

QByteArray vl2b(const QVariantList& strokes);
QByteArray sl2b(const QList<Stroke>& strokes);

#endif  // DATACONVERSION_H
