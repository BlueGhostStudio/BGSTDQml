#include "hwsettings.h"

#include <QGuiApplication>
#include <QJsonDocument>

#define HW_DPI "handwriter/DPI"

#define HW_STROKESIZE "handwriter/strokeSize"
#define HW_STROKESIZESRULE HW_STROKESIZE "/rule"
#define HW_DEFAULTSTROKESIZE HW_STROKESIZE "/default"

#define HW_PEN "handwriter/pen"
#define HW_PALETTE HW_PEN "/palette"
#define HW_MINPENSIZE HW_PEN "/minPenSize"
//#define HW_MAXPENSIZERATIO HW_PEN "/maxPenSizeRatio"

#define HW_PRESSURESUPPORT HW_PEN "/pressureSupport"

#define HW_MINPRESSURE HW_PEN "/minPressure"
#define HW_MAXPRESSURE HW_PEN "/maxPressure"

#define HW_MINELLIPSEDIAMETERS HW_PEN "/minEllipseDiameters"
#define HW_MAXELLIPSEDIAMETERS HW_PEN "/maxEllipseDiameters"

#define HW_VELOCITYTHRESHOLD HW_PEN "/velocityThreshold"

#define HW_FADEOUTLIMIT HW_PEN "/fadeoutLimit"
#define HW_FADEINLIMIT HW_PEN "/fadeinLimit"

#define HW_CUSTOMIZEDPENS "handwriter/customizedPens"

#define HW_ZOOM "handwriter/zoom"
#define HW_ZOOMRATIO HW_ZOOM "/zoomRatio"
#define HW_AUTOSCROLLINTERVAL HW_ZOOM "/autoScroll/interval"
#define HW_AUTOSCROLLLINEHEIGHT HW_ZOOM "/autoScroll/lineHeight"
#define HW_AUTOSCROLLNEWLINELEFT HW_ZOOM "/autoScroll/newlineLeft"
#define HW_AUTOSCROLLTORIGHT HW_ZOOM "/autoScroll/toRight"
#define HW_AUTOSCROLLTRIGGERPOS HW_ZOOM "/autoScroll/triggerPos"

#define HW_INK_CONSUMPTION "ink/consumption"

HWSettings::HWSettings(QObject* parent)
    : QObject(parent), m_settings("BGStudio", "Handwriter") {
    m_inkConsumption = m_settings.value(HW_INK_CONSUMPTION, 0).toInt();
    qDebug() << m_settings.fileName();
}

HWSettings::~HWSettings() { setInkConsumption(m_inkConsumption); }

void
HWSettings::setDpi(qreal dpi) {
    m_settings.setValue(HW_DPI, dpi);
}

qreal
HWSettings::dpi() const {
    return m_settings
        .value(HW_DPI, qGuiApp->primaryScreen()->physicalDotsPerInch())
        .toReal();
}

void
HWSettings::setStrokeSizes(const QVariant& sizes) {
    m_settings.setValue(HW_STROKESIZESRULE, sizes);
    int DSS = 0;
    QVariantList strokeSizes = sizes.toList();

    //    m_strokeSizes = sizes;
    //    m_defaultStrokeSize = 0;

    for (int i = 0; i < strokeSizes.count(); ++i) {
        QVariantMap&& map = strokeSizes[i].toMap();
        if (map.contains("default") && map["default"].toBool()) {
            DSS = i;
            break;
        }
    }

    m_settings.setValue(HW_DEFAULTSTROKESIZE, DSS);

    emit strokeSizesChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
}

QVariant
HWSettings::strokeSizes() const {
    return m_settings.value(HW_STROKESIZESRULE, m_defaultStrokeSizes);
}

qreal
HWSettings::defaultStrokeSize() const {
    QVariantList strokeSizes =
        m_settings.value(HW_STROKESIZESRULE, m_defaultStrokeSizes).toList();
    int defaultStrokeSize =
        m_settings.value(HW_DEFAULTSTROKESIZE, m_defaultDefaultStrokeSize)
            .toInt();
    return strokeSizes[defaultStrokeSize].toMap()["value"].toReal();
}

int
HWSettings::defaultStrokeSizeRule() const {
    return m_settings.value(HW_DEFAULTSTROKESIZE, m_defaultDefaultStrokeSize)
        .toInt();
}

void
HWSettings::setPalette(const QVariant& p) {
    m_settings.setValue(HW_PALETTE, p);
    emit paletteChanged();
}

QVariant
HWSettings::palette() const {
    return m_settings.value(
        HW_PALETTE,
        QVariantList({ QColor("#000000"), QColor("#064980"), QColor("#6a1012"),
                       QColor("#a07001"), QColor("#053c14"), QColor("#237066"),
                       QColor("#641d59") }));
}

void
HWSettings::restore2DefaultStrokeSizeRule() {
    m_settings.remove(HW_STROKESIZE);
    emit strokeSizesChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
}

void
HWSettings::setMinPenSize(qreal size) {
    m_settings.setValue(HW_MINPENSIZE, size);
    emit minPenSizeChanged();
}

qreal
HWSettings::minPenSize() const {
    return m_settings.value(HW_MINPENSIZE, 0.1).toReal();
}

/*void
HWSettings::setMaxPenSizeRatio(qreal ratio) {
    m_settings.setValue(HW_MAXPENSIZERATIO, ratio);
    emit maxPenSizeRatioChanged();
}

qreal
HWSettings::maxPenSizeRatio() const {
    return m_settings.value(HW_MAXPENSIZERATIO, 2).toReal();
}*/

void
HWSettings::setPressureSupport(int pressure) {
    if (pressureSupport() != pressure) {
        m_settings.setValue(HW_PRESSURESUPPORT, pressure);
        emit pressureSupportChanged();
    }
}

int
HWSettings::pressureSupport() const {
    return m_settings.value(HW_PRESSURESUPPORT, 0).toInt();
}

void
HWSettings::setMinPressure(qreal pressure) {
    m_settings.setValue(HW_MINPRESSURE, pressure);
    emit minPressureChanged();
}

qreal
HWSettings::minPressure() const {
    return m_settings.value(HW_MINPRESSURE, 0.215).toReal();
}

void
HWSettings::setMaxPressure(qreal pressure) {
    m_settings.setValue(HW_MAXPRESSURE, pressure);
    emit maxPressureChanged();
}

qreal
HWSettings::maxPressure() const {
    return m_settings.value(HW_MAXPRESSURE, 0.345).toReal();
}

void
HWSettings::setMinEllipseDiameters(qreal ELLD) {
    m_settings.setValue(HW_MINELLIPSEDIAMETERS, ELLD);
    emit minEllipseDiametersChanged();
}

qreal
HWSettings::minEllipseDiameters() const {
    return m_settings.value(HW_MINELLIPSEDIAMETERS, 30).toReal();
}

void
HWSettings::setMaxEllipseDiameters(qreal ELLD) {
    m_settings.setValue(HW_MAXELLIPSEDIAMETERS, ELLD);
    emit maxEllipseDiametersChanged();
}

qreal
HWSettings::maxEllipseDiameters() const {
    return m_settings.value(HW_MAXELLIPSEDIAMETERS, 80).toReal();
}

void
HWSettings::setVelocityThreshold(qreal vel) {
    m_settings.setValue(HW_VELOCITYTHRESHOLD, vel);
    emit velocityThresholdChanged();
}

qreal
HWSettings::velocityThreshold() const {
    return m_settings.value(HW_VELOCITYTHRESHOLD, 400).toReal();
}

void
HWSettings::setFadeoutLimit(qreal fol) {
    m_settings.setValue(HW_FADEOUTLIMIT, fol);
    emit fadeoutLimitChanged();
}

qreal
HWSettings::fadeoutLimit() const {
    return m_settings.value(HW_FADEOUTLIMIT, -1).toReal();
}

void
HWSettings::setFadeinLimit(qreal fil) {
    m_settings.setValue(HW_FADEINLIMIT, fil);
    emit fadeinLimitChanged();
}

qreal
HWSettings::fadeinLimit() const {
    return m_settings.value(HW_FADEINLIMIT, -1).toReal();
}

void
HWSettings::restore2DefaultPen() {
    m_settings.remove(HW_PEN);

    emit minPenSizeChanged();
    // emit maxPenSizeRatioChanged();
    emit minPressureChanged();
    emit maxPressureChanged();
    emit minEllipseDiametersChanged();
    emit maxEllipseDiametersChanged();
    emit velocityThresholdChanged();
    emit fadeoutLimitChanged();
    emit fadeinLimitChanged();
}

void
HWSettings::setZoomRatio(qreal ratio) {
    m_settings.setValue(HW_ZOOMRATIO, ratio);

    emit zoomRatioChanged();
}

qreal
HWSettings::zoomRatio() const {
    return m_settings.value(HW_ZOOMRATIO, 10).toReal();
}

void
HWSettings::setAutoScrollInterval(int asi) {
    m_settings.setValue(HW_AUTOSCROLLINTERVAL, asi);
    emit autoScrollIntervalChanged();
}

int
HWSettings::autoScrollInterval() const {
    return m_settings.value(HW_AUTOSCROLLINTERVAL, 250).toInt();
}

void
HWSettings::setAutoScrollLineHeight(int lh) {
    m_settings.setValue(HW_AUTOSCROLLLINEHEIGHT, lh);
    emit autoScrollLineHeightChanged();
}

int
HWSettings::autoScrollLineHeight() const {
    return m_settings.value(HW_AUTOSCROLLLINEHEIGHT, 32).toInt();
}

void
HWSettings::setAutoScrollNewlineLeft(int nl) {
    m_settings.setValue(HW_AUTOSCROLLNEWLINELEFT, nl);
    emit autoScrollNewlineLeftChanged();
}

int
HWSettings::autoScrollNewlineLeft() const {
    return m_settings.value(HW_AUTOSCROLLNEWLINELEFT, 32).toInt();
}

void
HWSettings::setAutoScrollToRight(int tl) {
    m_settings.setValue(HW_AUTOSCROLLTORIGHT, tl);
    emit autoScrollToRightChanged();
}

int
HWSettings::autoScrollToRight() const {
    return m_settings.value(HW_AUTOSCROLLTORIGHT, 16).toInt();
}

void
HWSettings::setAutoScrollTriggerPos(qreal tp) {
    m_settings.setValue(HW_AUTOSCROLLTRIGGERPOS, tp);
    emit autoScrollTriggerPos();
}

qreal
HWSettings::autoScrollTriggerPos() const {
    return m_settings.value(HW_AUTOSCROLLTRIGGERPOS, 0.66).toReal();
}

void
HWSettings::restore2DefaultZoomAutoScroll() {
    m_settings.remove(HW_ZOOM);
    emit zoomRatioChanged();
    emit autoScrollIntervalChanged();
    emit autoScrollLineHeightChanged();
    emit autoScrollNewlineLeftChanged();
    emit autoScrollToRightChanged();
}

QVariant
HWSettings::customizedPens() const {
    return m_settings.value(HW_CUSTOMIZEDPENS, m_defaultCustomizedPens);
}

void
HWSettings::setCustomizedPens(const QVariant& cp) {
    qDebug() << cp.toList();
    m_settings.setValue(HW_CUSTOMIZEDPENS, cp.toList());
    emit customizedPensChanged();
}

void
HWSettings::restore2DefaultCustomizedPens() {
    m_settings.remove(HW_CUSTOMIZEDPENS);
    emit customizedPensChanged();
}

int
HWSettings::inkConsumption() const {
    return m_inkConsumption;
}

void
HWSettings::setInkConsumption(int c) {
    m_inkConsumption = c;
    m_settings.setValue(HW_INK_CONSUMPTION, m_inkConsumption);
    emit inkConsumptionChanged();
    emit remainingInkChanged();
}

qreal
HWSettings::remainingInk() const {
    if (m_inkConsumption >= m_maxInkAmount)
        return 0;
    else
        return (qreal)(m_maxInkAmount - m_inkConsumption) /
               (qreal)m_maxInkAmount;
}

int
HWSettings::maxInkAmount() const {
    return m_maxInkAmount;
}

void
HWSettings::setMaxInkAmount(int mia) {
    m_maxInkAmount = mia;
    emit maxInkAmountChanged();
    emit remainingInkChanged();
}

void
HWSettings::consumeInk() {
    m_inkConsumption++;
    if (m_inkConsumption >= m_maxInkAmount) {
        emit inkEmptied();
        m_inkConsumption = 0;
    }

    emit inkConsumptionChanged();
    emit remainingInkChanged();
}

void
HWSettings::restore2Default() {
    m_settings.clear();
    emit strokeSizesChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
    emit minPenSizeChanged();
    // emit maxPenSizeRatioChanged();
    emit minPressureChanged();
    emit maxPressureChanged();
    emit zoomRatioChanged();
    emit customizedPensChanged();
}
