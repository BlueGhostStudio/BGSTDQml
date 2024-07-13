#ifndef HWSETTINGS_H
#define HWSETTINGS_H

#include <QQmlEngine>
#include <QSettings>

#include "hwwriter.h"

class HWSettings : public QSettings {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(qreal isTouchScreen READ isTouchScreen CONSTANT)

    Q_PROPERTY(qreal dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(qreal dpMm READ dpMm WRITE setDpMm NOTIFY dpMmChanged)

    Q_PROPERTY(bool byDevicePixelRatio READ byDevicePixelRatio WRITE
                   setByDevicePixelRatio NOTIFY byDevicePixelRatioChanged)

    /*Q_PROPERTY(QVariant strokeSizes READ strokeSizes WRITE setStrokeSizes
    NOTIFY strokeSizesChanged) Q_PROPERTY(qreal defaultStrokeSize READ
    defaultStrokeSize NOTIFY defaultStrokeSizeChanged) Q_PROPERTY(int
    defaultStrokeSizeRule READ defaultStrokeSizeRule NOTIFY
                   defaultStrokeSizeRuleChanged)*/

    Q_PROPERTY(
        QVariant palette READ palette WRITE setPalette NOTIFY paletteChanged)

    Q_PROPERTY(qreal strokeSize READ strokeSize WRITE setStrokeSize NOTIFY
                   strokeSizeChanged)
    Q_PROPERTY(qreal minPenSize READ minPenSize WRITE setMinPenSize NOTIFY
                   minPenSizeChanged)

    /*Q_PROPERTY(int pressureSupport READ pressureSupport WRITE
    setPressureSupport NOTIFY pressureSupportChanged)

    Q_PROPERTY(qreal minPressure READ minPressure WRITE setMinPressure NOTIFY
                   minPressureChanged)
    Q_PROPERTY(qreal maxPressure READ maxPressure WRITE setMaxPressure NOTIFY
                   maxPressureChanged)

    Q_PROPERTY(qreal minEllipseDiameters READ minEllipseDiameters WRITE
                   setMinEllipseDiameters NOTIFY minEllipseDiametersChanged)
    Q_PROPERTY(qreal maxEllipseDiameters READ maxEllipseDiameters WRITE
                   setMaxEllipseDiameters NOTIFY maxEllipseDiametersChanged)*/

    Q_PROPERTY(qreal velocityThreshold READ velocityThreshold WRITE
                   setVelocityThreshold NOTIFY velocityThresholdChanged)

    Q_PROPERTY(qreal fadeoutLimit READ fadeoutLimit WRITE setFadeoutLimit NOTIFY
                   fadeoutLimitChanged)
    Q_PROPERTY(qreal fadeinLimit READ fadeinLimit WRITE setFadeinLimit NOTIFY
                   fadeinLimitChanged)

    Q_PROPERTY(qreal zoomRatio READ zoomRatio WRITE setZoomRatio NOTIFY
                   zoomRatioChanged)
    Q_PROPERTY(int autoScrollInterval READ autoScrollInterval WRITE
                   setAutoScrollInterval NOTIFY autoScrollIntervalChanged)
    Q_PROPERTY(int autoScrollLineHeight READ autoScrollLineHeight WRITE
                   setAutoScrollLineHeight NOTIFY autoScrollLineHeightChanged)
    Q_PROPERTY(int autoScrollNewlineLeft READ autoScrollNewlineLeft WRITE
                   setAutoScrollNewlineLeft NOTIFY autoScrollNewlineLeftChanged)
    Q_PROPERTY(int autoScrollToRight READ autoScrollToRight WRITE
                   setAutoScrollToRight NOTIFY autoScrollToRightChanged)
    Q_PROPERTY(qreal autoScrollTriggerPos READ autoScrollTriggerPos WRITE
                   setAutoScrollTriggerPos NOTIFY autoScrollTriggerPosChanged)

    Q_PROPERTY(QVariantList penPresets READ penPresets WRITE setPenPresets NOTIFY
                   penPresetsChanged)

    Q_PROPERTY(int inkConsumption READ inkConsumption WRITE setInkConsumption
                   NOTIFY inkConsumptionChanged)
    Q_PROPERTY(qreal remainingInk READ remainingInk NOTIFY remainingInkChanged)
    Q_PROPERTY(int maxInkAmount READ maxInkAmount WRITE setMaxInkAmount NOTIFY
                   maxInkAmountChanged)

    Q_PROPERTY(QVariantList userSavedStrokes READ userSavedStrokes WRITE
                   setUserSavedStrokes NOTIFY userSavedStrokesChanged)

    Q_PROPERTY(QVariantList strokeSample READ strokeSample WRITE setStrokeSample
                   NOTIFY strokeSampleChanged)

public:
    explicit HWSettings(QObject* parent = nullptr);

    bool isTouchScreen() const;

    void setDpi(qreal dpi);
    qreal dpi() const;

    qreal dpMm() const;
    void setDpMm(qreal dots);

    bool byDevicePixelRatio() const;
    void setByDevicePixelRatio(bool bdpr);

    /*void setStrokeSizes(const QVariant& sizes);
    QVariant strokeSizes() const;

    qreal defaultStrokeSize() const;

    int defaultStrokeSizeRule() const;*/

    void setPalette(const QVariant& p);
    QVariant palette() const;

    // Q_INVOKABLE void restore2DefaultStrokeSizeRule();

    void setStrokeSize(qreal size);
    qreal strokeSize() const;

    void setMinPenSize(qreal size);
    qreal minPenSize() const;

    /*void setPressureSupport(int pressure);
    int pressureSupport() const;

    void setMinPressure(qreal pressure);
    qreal minPressure() const;

    void setMaxPressure(qreal pressure);
    qreal maxPressure() const;

    void setMinEllipseDiameters(qreal ELLD);
    qreal minEllipseDiameters() const;

    void setMaxEllipseDiameters(qreal ELLD);
    qreal maxEllipseDiameters() const;*/

    void setVelocityThreshold(qreal vel);
    qreal velocityThreshold() const;

    void setFadeoutLimit(qreal fol);
    qreal fadeoutLimit() const;

    void setFadeinLimit(qreal fil);
    qreal fadeinLimit() const;
    Q_INVOKABLE void restore2DefaultPen();

    void setZoomRatio(qreal ratio);
    qreal zoomRatio() const;

    void setAutoScrollInterval(int asi);
    int autoScrollInterval() const;

    void setAutoScrollLineHeight(int lh);
    int autoScrollLineHeight() const;

    void setAutoScrollNewlineLeft(int nl);
    int autoScrollNewlineLeft() const;

    void setAutoScrollToRight(int tl);
    int autoScrollToRight() const;

    void setAutoScrollTriggerPos(qreal tp);
    qreal autoScrollTriggerPos() const;
    Q_INVOKABLE void restore2DefaultZoomAutoScroll();

    QVariantList penPresets() const;
    void setPenPresets(const QVariantList& cp);
    Q_INVOKABLE void restore2DefaultPenPresets();

    int inkConsumption() const;
    void setInkConsumption(int c);

    qreal remainingInk() const;

    int maxInkAmount() const;
    void setMaxInkAmount(int mia);
    Q_INVOKABLE void consumeInk();

    Q_INVOKABLE void restore2Default();

    QVariantList userSavedStrokes() const;
    void setUserSavedStrokes(const QVariantList& userSavedStrokes);
    Q_INVOKABLE int saveUserStroke(const QJSValue& options);
    Q_INVOKABLE void updateUserStroke(int index, const QJSValue& options);
    Q_INVOKABLE void removeUserStroke(int index);

    QVariantList strokeSample();
    void setStrokeSample(const QVariantList& strokeSample);

signals:
    void dpiChanged();
    void dpMmChanged();

    void byDevicePixelRatioChanged();

    // void strokeSizesChanged();
    void defaultStrokeSizeChanged();
    void defaultStrokeSizeRuleChanged();
    void paletteChanged();

    void strokeSizeChanged();
    void minPenSizeChanged();
    // void maxPenSizeRatioChanged();

    /*void pressureSupportChanged();

    void minPressureChanged();
    void maxPressureChanged();

    void minEllipseDiametersChanged();
    void maxEllipseDiametersChanged();*/

    void velocityThresholdChanged();

    void fadeoutLimitChanged();
    void fadeinLimitChanged();
    void zoomRatioChanged();
    void autoScrollIntervalChanged();
    void autoScrollLineHeightChanged();
    void autoScrollNewlineLeftChanged();
    void autoScrollToRightChanged();
    void autoScrollTriggerPosChanged();

    void penPresetsChanged();

    void inkConsumptionChanged();
    void remainingInkChanged();
    void maxInkAmountChanged();
    void inkEmptied();

    void userSavedStrokesChanged();

    void strokeSampleChanged();

private:
    bool m_isTouchScreen = false;
    qreal m_dpi = -1;

    /*const QVariant m_defaultStrokeSizes = QVariantList{
        QVariantMap{ { "label", tr("ExtraLight") }, { "value", 0.15 } },
        QVariantMap{ { "label", tr("Light") }, { "value", 0.65 } },
        QVariantMap{
            { "label", tr("Normal") }, { "value", 1.15 }, { "default", true } },
        QVariantMap{ { "label", tr("Bold") }, { "value", 1.65 } },
        QVariantMap{ { "label", tr("ExtraBold") }, { "value", 2.15 } }
    };
    const int m_defaultDefaultStrokeSize = 2;*/

    const QVariant m_defaultPenPresets =
        QVariantList{ QVariantMap{ { "label", tr("Ballpoint pen") },
                                   { "strokeType", HWWriter::Ballpoint },
                                   { "strokeSize", 1 } },
                      QVariantMap{ { "label", tr("Ballpoint pen M") },
                                   { "strokeType", HWWriter::Ballpoint },
                                   { "strokeSize", 3 } },
                      QVariantMap{ { "label", tr("Fountain Pen F") },
                                   { "strokeType", HWWriter::Pen },
                                   { "strokeSize", 2 /*0.5*/ },
                                   /*{ "minPressure", 0.25 },
                                   { "maxPressure", 0.32 },*/
                                   { "minPenSize", 1 /*0.25*/ },
                                   { "fadeoutLimit", 0.8 },
                                   { "fadeinLimit", 1.2 } },
                      QVariantMap{ { "label", tr("Fountain Pen M") },
                                   { "strokeType", HWWriter::Pen },
                                   { "strokeSize", 5 /*1.2*/ },
                                   /*{ "minPressure", 0.23 },
                                   { "maxPressure", 0.32 },*/
                                   { "minPenSize", 1 /*0.01*/ },
                                   { "fadeoutLimit", 0.8 },
                                   { "fadeinLimit", 1.2 } },
                      QVariantMap{ { "label", tr("Fountain Pen L") },
                                   { "strokeType", HWWriter::Pen },
                                   { "strokeSize", 8 /*1.55*/ },
                                   /*{ "minPressure", 0.23 },
                                   { "maxPressure", 0.32 },*/
                                   { "minPenSize", 1 /*0.01*/ },
                                   { "fadeoutLimit", 0.6 },
                                   { "fadeinLimit", 1.4 },
                                   { "velocityThreshold", 800 } } };
    const QVariant m_defaultPalette = QVariantList{
        QColor::fromString("#000000"), QColor::fromString("#064980"),
        QColor::fromString("#6a1012"), QColor::fromString("#a07001"),
        QColor::fromString("#053c14"), QColor::fromString("#237066"),
        QColor::fromString("#641d59")
    };
    int m_inkConsumption = 0;

    int m_maxInkAmount = 64;

    QVariantList m_strokeSample;
};

#endif  // HWSETTINGS_H
