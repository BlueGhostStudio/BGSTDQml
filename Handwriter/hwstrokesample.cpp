#include "hwstrokesample.h"

#include <QRandomGenerator>
#include <hwsettings.h>

HWStrokeSample::HWStrokeSample() {
    setClip(true);
    setFlag(QQuickItem::ItemObservesViewport, false);

    // setContentVisible(VisibleFullContent);

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
HWStrokeSample::componentComplete() {
    HWWriter::componentComplete();
    m_strokeSampleFromSetting = m_settings->strokeSample();
}

void
HWStrokeSample::updateCircularPreview(qreal radius) {
    clear();
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
            velocity = QRandomGenerator::global()->generateDouble() *
                           m_velocityThreshold +
                       50;

        point = { 1,
                  QPointF(qCos(angle) * radius + center.x(),
                          qSin(angle) * radius + center.y()),
                  velocity };
        penSegment(point);
    }
}

void
HWStrokeSample::updateSineWavePreview(qreal width, qreal height) {
    clear();
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
        velocity =
            (qSin(accumLen * r_p + M_PI / 2) * 0.5 + 0.5) * m_velocityThreshold;
        accumLen += 5;

        StrokePoint point{ 1,
                           { startX + accumLen,
                             qSin(accumLen * hScale) * vScale +
                                 verticalCenter },
                           velocity };

        penSegment(point);
        // emit writeUpdated(stroke, point);
    }

    setImplicitSize(width, height);
}

void
HWStrokeSample::loadFromSetting() {
    QObject::connect(m_settings, &HWSettings::strokeSampleChanged, this, [=]() {
        m_strokeSampleFromSetting = m_settings->strokeSample();
        updateFromSetting();
    });

    updateFromSetting();
}

void
HWStrokeSample::updateFromSetting() {
    if (m_strokeSampleFromSetting.isEmpty()) {
        updateCircularPreview((width() - 6) / 2);
    } else {
        clear();
        replayStrokesWrite(m_strokeSampleFromSetting);
        setImplicitSize(m_range.width(), m_range.height());
    }
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
