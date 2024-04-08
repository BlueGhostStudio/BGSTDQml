#ifndef HWSETTINGS_H
#define HWSETTINGS_H

#include <QObject>
//#include <QMap>
//#include <QPair>
//#include <QList>
#include <QSettings>
#include <QVariant>

#include "hwwriter.h"

class HWSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(qreal dpi READ dpi WRITE setDpi NOTIFY dpiChanged)

    Q_PROPERTY(QVariant strokeSizes READ strokeSizes WRITE setStrokeSizes NOTIFY
                   strokeSizesChanged)
    Q_PROPERTY(qreal defaultStrokeSize READ defaultStrokeSize NOTIFY
                   defaultStrokeSizeChanged)
    Q_PROPERTY(int defaultStrokeSizeRule READ defaultStrokeSizeRule NOTIFY
                   defaultStrokeSizeRuleChanged)

    Q_PROPERTY(
        QVariant palette READ palette WRITE setPalette NOTIFY paletteChanged)

    Q_PROPERTY(qreal minPenSize READ minPenSize WRITE setMinPenSize NOTIFY
                   minPenSizeChanged)
    /*Q_PROPERTY(qreal maxPenSizeRatio READ maxPenSizeRatio WRITE
                   setMaxPenSizeRatio NOTIFY maxPenSizeRatioChanged)*/

    Q_PROPERTY(int pressureSupport READ pressureSupport WRITE setPressureSupport
                   NOTIFY pressureSupportChanged)

    Q_PROPERTY(qreal minPressure READ minPressure WRITE setMinPressure NOTIFY
                   minPressureChanged)
    Q_PROPERTY(qreal maxPressure READ maxPressure WRITE setMaxPressure NOTIFY
                   maxPressureChanged)

    Q_PROPERTY(qreal minEllipseDiameters READ minEllipseDiameters WRITE
                   setMinEllipseDiameters NOTIFY minEllipseDiametersChanged)
    Q_PROPERTY(qreal maxEllipseDiameters READ maxEllipseDiameters WRITE
                   setMaxEllipseDiameters NOTIFY maxEllipseDiametersChanged)
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

    Q_PROPERTY(QVariant customizedPens READ customizedPens WRITE
                   setCustomizedPens NOTIFY customizedPensChanged)

    Q_PROPERTY(int inkConsumption READ inkConsumption WRITE setInkConsumption
                   NOTIFY inkConsumptionChanged)
    Q_PROPERTY(qreal remainingInk READ remainingInk NOTIFY remainingInkChanged)
    Q_PROPERTY(int maxInkAmount READ maxInkAmount WRITE setMaxInkAmount NOTIFY
                   maxInkAmountChanged)
    //    Q_PROPERTY(int maxInkAmount READ maxInkAmount)

public:
    explicit HWSettings(QObject* parent = nullptr);
    ~HWSettings();

    void setDpi(qreal dpi);
    qreal dpi() const;

    void setStrokeSizes(const QVariant& sizes);
    QVariant strokeSizes() const;

    qreal defaultStrokeSize() const;

    int defaultStrokeSizeRule() const;

    void setPalette(const QVariant& p);
    QVariant palette() const;

    Q_INVOKABLE void restore2DefaultStrokeSizeRule();

    void setMinPenSize(qreal size);
    qreal minPenSize() const;

    /*void setMaxPenSizeRatio(qreal ratio);
    qreal maxPenSizeRatio() const;*/

    void setPressureSupport(int pressure);
    int pressureSupport() const;

    void setMinPressure(qreal pressure);
    qreal minPressure() const;

    void setMaxPressure(qreal pressure);
    qreal maxPressure() const;

    void setMinEllipseDiameters(qreal ELLD);
    qreal minEllipseDiameters() const;

    void setMaxEllipseDiameters(qreal ELLD);
    qreal maxEllipseDiameters() const;

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

    QVariant customizedPens() const;
    void setCustomizedPens(const QVariant& cp);
    Q_INVOKABLE void restore2DefaultCustomizedPens();

    int inkConsumption() const;
    void setInkConsumption(int c);

    qreal remainingInk() const;

    int maxInkAmount() const;
    void setMaxInkAmount(int mia);
    Q_INVOKABLE void consumeInk();

    Q_INVOKABLE void restore2Default();

signals:
    void dpiChanged();
    void strokeSizesChanged();
    void defaultStrokeSizeChanged();
    void defaultStrokeSizeRuleChanged();
    void paletteChanged();
    void minPenSizeChanged();
    // void maxPenSizeRatioChanged();

    void pressureSupportChanged();

    void minPressureChanged();
    void maxPressureChanged();

    void minEllipseDiametersChanged();
    void maxEllipseDiametersChanged();
    void velocityThresholdChanged();

    void fadeoutLimitChanged();
    void fadeinLimitChanged();
    void zoomRatioChanged();
    void autoScrollIntervalChanged();
    void autoScrollLineHeightChanged();
    void autoScrollNewlineLeftChanged();
    void autoScrollToRightChanged();
    void autoScrollTriggerPosChanged();
    void customizedPensChanged();
    void inkConsumptionChanged();
    void remainingInkChanged();
    void maxInkAmountChanged();
    void inkEmptied();

private:
    const QVariant m_defaultStrokeSizes = QVariantList(
        { QVariantMap({ { "label", tr("ExtraLight") }, { "value", 0.15 } }),
          QVariantMap({ { "label", tr("Light") }, { "value", 0.65 } }),
          QVariantMap({ { "label", tr("Normal") },
                        { "value", 1.15 },
                        { "default", true } }),
          QVariantMap({ { "label", tr("Bold") }, { "value", 1.65 } }),
          QVariantMap({ { "label", tr("ExtraBold") }, { "value", 2.15 } }) });
    const int m_defaultDefaultStrokeSize = 2;
    QSettings m_settings;

    const QVariant m_defaultCustomizedPens =
        QVariantList({ QVariantMap({ { "label", tr("Ballpoint pen") },
                                     { "type", HWWriter::Ballpoint },
                                     { "size", 1 } }),
                       QVariantMap({ { "label", tr("Ballpoint pen M") },
                                     { "type", HWWriter::Ballpoint },
                                     { "size", 3 } }),
                       QVariantMap({ { "label", tr("Fountain Pen F") },
                                     { "type", HWWriter::Pen },
                                     { "size", 0.5 },
                                     { "minPressure", 0.25 },
                                     { "maxPressure", 0.32 },
                                     { "minSize", 0.25 },
                                     { "maxSizeRatio", 3.5 },
                                     { "fadeinLimit", -1 },
                                     { "fadeoutLimit", -1 } }),
                       QVariantMap({ { "label", tr("Fountain Pen M") },
                                     { "type", HWWriter::Pen },
                                     { "size", 1.2 },
                                     { "minPressure", 0.23 },
                                     { "maxPressure", 0.32 },
                                     { "minSize", 0.01 },
                                     { "maxSizeRatio", 1.7 },
                                     { "fadeinLimit", -1 },
                                     { "fadeoutLimit", -1 } }),
                       QVariantMap({ { "label", tr("Fountain Pen L") },
                                     { "type", HWWriter::Pen },
                                     { "size", 1.55 },
                                     { "minPressure", 0.23 },
                                     { "maxPressure", 0.32 },
                                     { "minSize", 0.01 },
                                     { "maxSizeRatio", 2 },
                                     { "fadeinLimit", -1 },
                                     { "fadeoutLimit", -1 } }) });
    int m_inkConsumption;

    int m_maxInkAmount = 64;
};

#endif  // HWSETTINGS_H
