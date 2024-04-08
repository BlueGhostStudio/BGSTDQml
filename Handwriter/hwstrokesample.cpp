#include "hwstrokesample.h"

#include <QRandomGenerator>

HWStrokeSample::HWStrokeSample() {
    setClip(true);

    QObject::connect(this, &HWStrokeSample::writeStart, this,
                     [=](const QPointF&, const StrokePoint& point) {
                         if (m_record) {
                             m_recordedStrokes.append(point);
                         }
                     });
    QObject::connect(this, &HWStrokeSample::writeUpdated, this,
                     [=](const Stroke&, const StrokePoint& point) {
                         if (m_record) {
                             m_recordedStrokes.append(point);
                         }
                     });
}

void
HWStrokeSample::updateCircularPreview(qreal radius) {
    qreal angle = 0;
    const QPointF center(width() / 2, height() / 2);
    float velocity = -1;

    const QPointF startPos{ center.x() + radius, center.y() };
    StrokePoint point{ 0, startPos, velocity };
    penDown(point);
    // emit writeStart(startPos, point);

    qreal maxAngle = 2 * M_PI;

    while (angle < maxAngle) {
        qreal len = QRandomGenerator::global()->generateDouble() * 5 + 5;
        qreal halfLen = len / 2;
        angle += qAsin(halfLen / radius);

        if (QRandomGenerator::global()->generateDouble() > 0.5)
            velocity = QRandomGenerator::global()->generateDouble() * 750 + 50;

        point = { 1,
                  QPointF(qCos(angle) * radius + center.x(),
                          qSin(angle) * radius + center.y()),
                  velocity };
        penSegment(point);
    }
}

void
HWStrokeSample::updateSineWavePreview(qreal width, qreal height) {
    float velocity = -1;
    const qreal verticalCenter = this->height() / 2;
    qreal accumLen = 0;

    const qreal hScale = 2 * M_PI / width;
    const qreal vScale = height / 2;
    const qreal startX = (this->width() - width) / 2;

    const qreal r_p = 2 * M_PI / width;

    const QPointF startPos{ startX, verticalCenter };
    StrokePoint point{ 0, startPos, velocity };
    penDown(point);
    // emit writeStart(startPos, point);

    while (accumLen < width) {
        velocity = (qSin(accumLen * r_p + M_PI / 2) * 0.5 + 0.5) * 400;
        accumLen += 5;

        StrokePoint point{ 1,
                           { startX + accumLen,
                             qSin(accumLen * hScale) * vScale +
                                 verticalCenter },
                           velocity };

        Stroke stroke = penSegment(point);
        // emit writeUpdated(stroke, point);
    }
    /*auto createPolygon = [&](qreal len,
                             QList<QPair<int, StrokePoint>>& strokePoints) {
        velocity = (qSin(accumLen * r_p + M_PI / 2) * 0.5 + 0.5) * 400;
        accumLen += len;

        strokePoints.append(
            { 1,
              { QPointF(startX + accumLen,
                        qSin(accumLen * hScale) * vScale + verticalCenter),
                velocity } });
    };

    auto f = [&](QList<QPair<int, StrokePoint>>& strokePoints) {
        strokePoints.append({ 0, { QPointF(startX, verticalCenter), -1 } });

        while (accumLen < width) {
            createPolygon(5, strokePoints);
        }
    };

    if (m_record) {
        f(m_recordedStrokes);
        replayRecord();
    } else {
        QList<QPair<int, StrokePoint>> strokePoints;
        f(strokePoints);
        replayStrokesWrite(strokePoints);
    }*/
}

/*bool
HWStrokeSample::recording() const {
    return m_record;
}

void
HWStrokeSample::record() {
    m_recordedStrokes.clear();
    m_record = true;
    emit recordingChanged();
}

void
HWStrokeSample::stopRecord() {
    m_record = false;
    emit recordingChanged();
}

void
HWStrokeSample::replayRecord() {
    // stopRecord();
    clear();

    replayStrokesWrite(m_recordedStrokes);
}

void
HWStrokeSample::resetRecord() {
    clear();
    record();
}*/
