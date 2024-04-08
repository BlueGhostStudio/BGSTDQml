#include "dataconversion.h"

#include <QVariant>
#include <QtDebug>
#include <QtMath>

Stroke
variant2Stroke(const QVariant& stroke) {
    Stroke result;
    if (stroke.canConvert<QVariantMap>()) {
        QVariantMap strokeMap = stroke.toMap();

        QVariantMap posMap = strokeMap["pos"].toMap();
        result.pos.setX(posMap["x"].toDouble());
        result.pos.setY(posMap["y"].toDouble());

        QVariantMap prePosMap = strokeMap["prePos"].toMap();
        result.prePos.setX(prePosMap["x"].toDouble());
        result.prePos.setY(prePosMap["y"].toDouble());

        result.size = strokeMap["size"].toDouble();
        result.preSize = strokeMap["preSize"].toDouble();
        result.color = strokeMap["color"].toInt();
        result.type = strokeMap["type"].toInt();
    }

    return result;
}

QVariant
stroke2Variant(const Stroke& stroke) {
    QVariantMap vmStroke;

    vmStroke["type"] = stroke.type;
    vmStroke["color"] = stroke.color;

    vmStroke["preSize"] = stroke.preSize;
    vmStroke["size"] = stroke.size;

    QVariantMap vmPrePos;
    vmPrePos["x"] = stroke.prePos.x();
    vmPrePos["y"] = stroke.prePos.y();
    vmStroke["prePos"] = vmPrePos;

    QVariantMap vmPos;
    vmPos["x"] = stroke.pos.x();
    vmPos["y"] = stroke.pos.y();
    vmStroke["pos"] = vmPos;

    return vmStroke;
}

Stroke
byteArray2Stroke(const QByteArray& data) {
    Stroke stroke;

    stroke.type = (data[0] & 0X80) >> 7;
    stroke.color = data[0] & 0X7F;

    stroke.preSize = (double)(((quint8)data[1] << 8) | (quint8)data[2]) / 100;
    stroke.size = (double)(((quint8)data[3] << 8) | (quint8)data[4]) / 100;

    stroke.prePos.setX((double)(((quint8)data[5] << 8) | (quint8)data[6]) / 10);
    stroke.prePos.setY((double)(((quint8)data[7] << 8) | (quint8)data[8]) / 10);

    QVariantMap vmPos;
    stroke.pos.setX((double)(((quint8)data[9] << 8) | (quint8)data[10]) / 10);
    stroke.pos.setY((double)(((quint8)data[11] << 8) | (quint8)data[12]) / 10);

    return stroke;
}

QVariant
byteArray2Variant(const QByteArray& data) {
    return stroke2Variant(byteArray2Stroke(data));

    /*vmStroke["type"] = (data[0] & 0X80) >> 7;
    vmStroke["color"] = data[0] & 0X7F;

    vmStroke["preSize"] =
        (double)(((quint8)data[1] << 8) | (quint8)data[2]) / 100;
    vmStroke["size"] = (double)(((quint8)data[3] << 8) | (quint8)data[4]) / 100;

    QVariantMap vmPrePos;
    vmPrePos["x"] = (double)(((quint8)data[5] << 8) | (quint8)data[6]) / 10;
    vmPrePos["y"] = (double)(((quint8)data[7] << 8) | (quint8)data[8]) / 10;
    vmStroke["prePos"] = vmPrePos;

    QVariantMap vmPos;
    vmPos["x"] = (double)(((quint8)data[9] << 8) | (quint8)data[10]) / 10;
    vmPos["y"] = (double)(((quint8)data[11] << 8) | (quint8)data[12]) / 10;
    vmStroke["pos"] = vmPos;*/
}

QByteArray
stroke2ByteArray(const Stroke& stroke) {
    QByteArray strokeData(13, '\x00');

    strokeData[0] = stroke.type << 7 | stroke.color;

    unsigned int preSize = qFloor(stroke.preSize * 100);
    strokeData[1] = preSize >> 8;
    strokeData[2] = preSize & 0xFF;

    unsigned int size = qFloor(stroke.size * 100);
    strokeData[3] = size >> 8;
    strokeData[4] = size & 0XFF;

    unsigned int prePosX = qFloor(stroke.prePos.x() * 10);
    strokeData[5] = prePosX >> 8;
    strokeData[6] = prePosX & 0XFF;

    unsigned int prePosY = qFloor(stroke.prePos.y() * 10);
    strokeData[7] = prePosY >> 8;
    strokeData[8] = prePosY & 0XFF;

    unsigned int posX = qFloor(stroke.pos.x() * 10);
    strokeData[9] = posX >> 8;
    strokeData[10] = posX & 0XFF;

    unsigned int posY = qFloor(stroke.pos.y() * 10);
    strokeData[11] = posY >> 8;
    strokeData[12] = posY & 0XFF;

    return strokeData;
}

QByteArray
variant2ByteArray(const QVariant& variant) {
    return stroke2ByteArray(variant2Stroke(variant));
}

QList<Stroke>
b2sl(const QByteArray& data) {
    QList<Stroke> strokes;

    for (int i = 0; i < data.length(); i += 13)
        strokes.append(byteArray2Stroke(data.mid(i, 13)));

    return strokes;
}

QVariantList
b2vl(const QByteArray& data) {
    QVariantList vlStrokes;

    for (int i = 0; i < data.size(); i += 13)
        vlStrokes.append(byteArray2Variant(data.mid(i, 13)));

    return vlStrokes;
}

QByteArray
vl2b(const QVariantList& strokes) {
    QByteArray data;

    foreach (const QVariant& strokeVariant, strokes)
        data += variant2ByteArray(strokeVariant);

    return data;
}

QByteArray
sl2b(const QList<Stroke>& strokes) {
    QByteArray data;

    foreach (const Stroke& stroke, strokes)
        data += stroke2ByteArray(stroke);

    return data;
}
