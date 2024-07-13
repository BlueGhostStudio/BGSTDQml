#ifndef HWSTROKESAMPLE_H
#define HWSTROKESAMPLE_H

#include <QQmlEngine>

#include "hwwriter.h"

class HWStrokeSample : public HWWriter {
    Q_OBJECT
    QML_ELEMENT

    // Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)

public:
    HWStrokeSample();
    void componentComplete() override;

    Q_INVOKABLE void updateCircularPreview(qreal radius);
    Q_INVOKABLE void updateSineWavePreview(qreal width, qreal height);

    Q_INVOKABLE void loadFromSetting();
    Q_INVOKABLE void updateFromSetting();

    /*bool recording() const;
    Q_INVOKABLE void record();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void replayRecord();
    Q_INVOKABLE void resetRecord();*/

/*signals:
    void recordingChanged();*/

protected:
    /*bool m_record = false;
    QList<QPair<int, StrokePoint>> m_recordedStrokes;*/

    QVariantList m_strokeSampleFromSetting;
};

#endif  // HWSTROKESAMPLE_H
