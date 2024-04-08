#ifndef HWWRITER_H
#define HWWRITER_H

#include <hwcanvas.h>
#include <hwguideline.h>
#include <strokepoint.h>

#include <QQmlEngine>
#include <QQuickItem>

class HWWriter : public HWCanvas {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(HWGuideLine* guideLine READ guideLine CONSTANT)

    Q_PROPERTY(QJSValue strokeOptions READ strokeOptions WRITE setStrokeOptions
                   NOTIFY strokeOptionsChanged)
    Q_PROPERTY(StrokeType strokeType READ strokeType WRITE setStrokeType NOTIFY
                   strokeTypeChanged)
    Q_PROPERTY(StrokeColor strokeColor READ strokeColor WRITE setStrokeColor
                   NOTIFY strokeColorChanged)
    Q_PROPERTY(qreal strokeSize READ strokeSize WRITE setStrokeSize NOTIFY
                   strokeSizeChanged)
    Q_PROPERTY(qreal velocityThreshold READ velocityThreshold WRITE
                   setVelocityThreshold NOTIFY velocityThresholdChanged)
    Q_PROPERTY(qreal scaledVelocityThreshold READ scaledVelocityThreshold NOTIFY
                   scaledVelocityThresholdChanged)
    Q_PROPERTY(qreal maxVelocity READ maxVelocity NOTIFY maxVelocityChanged)
    Q_PROPERTY(
        qreal combinedScale READ combinedScale NOTIFY combinedScaleChanged)
    Q_PROPERTY(qreal minPenSize READ minPenSize WRITE setMinPenSize NOTIFY
                   minPenSizeChanged)

    Q_PROPERTY(qreal fadeoutLimit READ fadeoutLimit WRITE setFadeoutLimit NOTIFY
                   fadeoutLimitChanged)
    Q_PROPERTY(qreal fadeinLimit READ fadeinLimit WRITE setFadeinLimit NOTIFY
                   fadeinLimitChanged)

    Q_PROPERTY(
        bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)

    Q_PROPERTY(QJSValue strokeSizeCalc READ strokeSizeCalc WRITE
                   setStrokeSizeCalc NOTIFY strokeSizeCalcChanged)

    Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)

    Q_PROPERTY(bool writing READ writing NOTIFY writingChanged)

public:
    HWWriter(QQuickItem* parent = nullptr);

    enum StrokeType { Ballpoint, Pen, Paint };
    Q_ENUM(StrokeType)

    enum StrokeColor { Black, Blue, Red, Yellow, Green, Cyan, Purple };
    Q_ENUM(StrokeColor)

    /*enum PressureSupport { None, Pressure, TouchSize };
    Q_ENUM(PressureSupport)*/

    void classBegin() override;
    void componentComplete() override;

    Q_INVOKABLE void loadSettings();
    Q_INVOKABLE bool loadSettings(int preset);
    Q_INVOKABLE void loadSettings(const QJSValue& strokeOptions);

    HWGuideLine* guideLine() const;

    void setStrokeOptions(const QJSValue& options);
    QJSValue strokeOptions() const;

    void setStrokeType(StrokeType type);
    StrokeType strokeType() const;

    void setStrokeColor(StrokeColor color);
    StrokeColor strokeColor() const;

    void setStrokeSize(qreal size);
    qreal strokeSize() const;

    /*void setPressureSupport(PressureSupport type);
    PressureSupport pressureSupport() const;

    void setMinPressure(qreal pressure);
    qreal minPressure() const;

    void setMaxPressure(qreal pressure);
    qreal maxPressure() const;

    void setMinEllipseDiameters(qreal ELLD);
    qreal minEllipseDiameters() const;

    void setMaxEllipseDiameters(qreal ELLD);
    qreal maxEllipseDiameters() const;*/

    /*qreal minPE() const;
    qreal maxPE() const;*/

    void setVelocityThreshold(qreal vel);
    qreal velocityThreshold() const;

    qreal scaledVelocityThreshold() const;

    qreal maxVelocity() const;
    qreal combinedScale() const;

    void setMinPenSize(qreal size);
    qreal minPenSize() const;

    void setFadeoutLimit(qreal fol);
    qreal fadeoutLimit() const;

    void setFadeinLimit(qreal fil);
    qreal fadeinLimit() const;

    void setReadOnly(bool ro);
    bool readOnly() const;

    bool writing() const;

    void setStrokeSizeCalc(const QJSValue& callback);
    QJSValue strokeSizeCalc() const;

    Q_INVOKABLE qreal fadeLimitCalc(qreal preSize, qreal size) const;

    bool recording() const;
    Q_INVOKABLE void record();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void replayRecord();
    Q_INVOKABLE void resetRecord();
    // Q_INVOKABLE void replayStrokesWrite(const QJSValue& strokes);

    Q_INVOKABLE void penDown(const StrokePoint& point, bool signal = true);
    Q_INVOKABLE Stroke penSegment(const StrokePoint& point, bool signal = true);

signals:
    void clicked(const QPointF& pos);

    void writeStart(const QPointF& pos, const StrokePoint& point);
    void writeUpdated(const Stroke& stroke, const StrokePoint& point);
    void writeEnd();
    void writeIgnore();

    void strokeOptionsChanged();

    void strokeTypeChanged();
    void strokeColorChanged();
    void strokeSizeChanged();

    /*void pressureSupportChanged();

    void minPressureChanged();
    void maxPressureChanged();

    void minEllipseDiametersChanged();
    void maxEllipseDiametersChanged();*/

    void velocityThresholdChanged();
    void maxVelocityChanged();
    void combinedScaleChanged();

    void minPenSizeChanged();
    void readOnlyChanged();
    void fadeoutLimitChanged();
    void fadeinLimitChanged();

    void strokeSizeCalcChanged();

    void recordingChanged();

    /*void recordChanged();
    void recording(const QPointF& prePos, const QPointF& pos, qreal ELLD,
                   qreal pressre);*/

    void writingChanged();

    void scaledVelocityThresholdChanged();

private slots:
    void cumulativeScale();

private:
    // qreal getPE(const t_strokePoint& point) const;

    QVariant toStrokeVariant(int type, int color, const QPointF& pos,
                             const QPointF& prePos, qreal size,
                             qreal preSize) const;
    qreal calcStrokeSize(qreal preSize, const StrokePoint& point);
    qreal defaultStrokeSizeCalc(qreal preSize,
                                /*qreal PE,*/ qreal velocity) const;

protected:
    void replayStrokesWrite(const QList<StrokePoint>& strokePoints);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void touchEvent(QTouchEvent* event) override;

protected:
    quint64 m_pressTimeStamp = 0;
    HWGuideLine* m_guideLine = nullptr;
    QPointF m_prePos;
    qreal m_preSize;

    int m_strokeType;
    int m_strokeColor;
    qreal m_strokeSize;

    /*PressureSupport m_pressureSupport;

    qreal m_minPressure;
    qreal m_maxPressure;

    qreal m_minEllipseDiameters;
    qreal m_maxEllipseDiameters;*/

    qreal m_velocityThreshold;
    qreal m_maxVelocity = 0;
    qreal m_combinedScale = 1.00;

    qreal m_minPenSize;
    // qreal m_maxPenSizeRatio = 2;

    qreal m_fadeoutLimit;
    qreal m_fadeinLimit;

    bool m_keepTouchIgnore = false;

    bool m_writing = false;

    bool m_readOnly = false;

    QJSValue m_strokeSizeCalcFun;

    bool m_record = false;
    // QList<QPair<int, StrokePoint>> m_recordedStrokes;
    QList<StrokePoint> m_recordedStrokes;
};

#endif  // HWWRITER_H
