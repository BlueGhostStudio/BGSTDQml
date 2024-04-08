#include "hwsettings.h"

#include <QGuiApplication>
#include <QJsonDocument>

#define HW_DPI "handwriter/DPI"

#define HW_BYDEVICEPIXELRATIO "handwriter/byDevicePixelRatio"

// #define HW_STROKESIZE "handwriter/strokeSize"
// #define HW_STROKESIZESRULE HW_STROKESIZE "/rule"
// #define HW_DEFAULTSTROKESIZE HW_STROKESIZE "/default"

#define HW_PEN "handwriter/pen"
#define HW_PALETTE HW_PEN "/palette"
#define HW_STROKESIZE HW_PEN "/strokeSize"
#define HW_MINPENSIZE HW_PEN "/minPenSize"
// #define HW_MAXPENSIZERATIO HW_PEN "/maxPenSizeRatio"

/*#define HW_PRESSURESUPPORT HW_PEN "/pressureSupport"

#define HW_MINPRESSURE HW_PEN "/minPressure"
#define HW_MAXPRESSURE HW_PEN "/maxPressure"

#define HW_MINELLIPSEDIAMETERS HW_PEN "/minEllipseDiameters"
#define HW_MAXELLIPSEDIAMETERS HW_PEN "/maxEllipseDiameters"*/

#define HW_VELOCITYTHRESHOLD HW_PEN "/velocityThreshold"

#define HW_FADEOUTLIMIT HW_PEN "/fadeoutLimit"
#define HW_FADEINLIMIT HW_PEN "/fadeinLimit"

#define HW_PENPRESETS "handwriter/penPresets"

#define HW_ZOOM "handwriter/zoom"
#define HW_ZOOMRATIO HW_ZOOM "/zoomRatio"
#define HW_AUTOSCROLLINTERVAL HW_ZOOM "/autoScroll/interval"
#define HW_AUTOSCROLLLINEHEIGHT HW_ZOOM "/autoScroll/lineHeight"
#define HW_AUTOSCROLLNEWLINELEFT HW_ZOOM "/autoScroll/newlineLeft"
#define HW_AUTOSCROLLTORIGHT HW_ZOOM "/autoScroll/toRight"
#define HW_AUTOSCROLLTRIGGERPOS HW_ZOOM "/autoScroll/triggerPos"

#define HW_USERSAVEDSTROKES "handwriter/userSavedStrokes"

#define HW_INK_CONSUMPTION "ink/consumption"

HWSettings::HWSettings(QObject* parent)
    : QSettings{ "BGStudio", "Handwriter", parent } {
    foreach (const QInputDevice* device, QInputDevice::devices()) {
        if (device->type() == QInputDevice::DeviceType::TouchScreen) {
            m_isTouchScreen = true;
            break;
        }
    }

    m_dpi =
        value(HW_DPI, qGuiApp->primaryScreen()->physicalDotsPerInch()).toReal();
}

bool
HWSettings::isTouchScreen() const {
    return m_isTouchScreen;
}

void
HWSettings::setDpi(qreal dpi) {
    if (m_dpi != dpi) {
        m_dpi = dpi;
        setValue(HW_DPI, dpi);
        emit dpiChanged();
        emit dpMmChanged();
    }
}

qreal
HWSettings::dpi() const {
    return m_dpi;
}

qreal
HWSettings::dpMm() const {
    return dpi() * 0.0393701;
}

void
HWSettings::setDpMm(qreal dots) {
    if (dpMm() != dots) {
        setDpi(dots / 0.0393701);
    }
}

bool
HWSettings::byDevicePixelRatio() const {
    return value(HW_BYDEVICEPIXELRATIO, true).toBool();
}

void
HWSettings::setByDevicePixelRatio(bool bdpr) {
    if (byDevicePixelRatio() != bdpr) {
        setValue(HW_BYDEVICEPIXELRATIO, bdpr);
        emit byDevicePixelRatioChanged();
    }
}

/*void
HWSettings::setStrokeSizes(const QVariant& sizes) {
    QJSValue jsvSizes = sizes.value<QJSValue>();
    setValue(HW_STROKESIZESRULE, jsvSizes.toVariant());
    int DSS = 0;
    QVariantList strokeSizes = sizes.toList();

    for (int i = 0; i < strokeSizes.count(); ++i) {
        QVariantMap&& map = strokeSizes[i].toMap();
        if (map.contains("default") && map["default"].toBool()) {
            DSS = i;
            break;
        }
    }

    setValue(HW_DEFAULTSTROKESIZE, DSS);

    emit strokeSizesChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
}

QVariant
HWSettings::strokeSizes() const {
    return value(HW_STROKESIZESRULE, m_defaultStrokeSizes);
}

qreal
HWSettings::defaultStrokeSize() const {
    QVariantList strokeSizes =
        value(HW_STROKESIZESRULE, m_defaultStrokeSizes).toList();
    int defaultStrokeSize =
        value(HW_DEFAULTSTROKESIZE, m_defaultDefaultStrokeSize).toInt();
    return strokeSizes[defaultStrokeSize].toMap()["value"].toReal();
}

int
HWSettings::defaultStrokeSizeRule() const {
    return value(HW_DEFAULTSTROKESIZE, m_defaultDefaultStrokeSize).toInt();
}*/

void
HWSettings::setPalette(const QVariant& p) {
    setValue(HW_PALETTE, p.toList());
    emit paletteChanged();
}

QVariant
HWSettings::palette() const {
    return value(HW_PALETTE, m_defaultPalette);
}

void
HWSettings::setStrokeSize(qreal size) {
    setValue(HW_STROKESIZE, size);
    emit strokeSizeChanged();
}

qreal
HWSettings::strokeSize() const {
    return value(HW_STROKESIZE, 1).toReal();
}

/*void
HWSettings::restore2DefaultStrokeSizeRule() {
    remove(HW_STROKESIZE);
    emit strokeSizesChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
}*/

void
HWSettings::setMinPenSize(qreal size) {
    setValue(HW_MINPENSIZE, size);
    emit minPenSizeChanged();
}

qreal
HWSettings::minPenSize() const {
    return value(HW_MINPENSIZE, 0.1).toReal();
}

/*void
HWSettings::setPressureSupport(int pressure) {
    if (pressureSupport() != pressure) {
        setValue(HW_PRESSURESUPPORT, pressure);
        emit pressureSupportChanged();
    }
}

int
HWSettings::pressureSupport() const {
    return value(HW_PRESSURESUPPORT, 0).toInt();
}

void
HWSettings::setMinPressure(qreal pressure) {
    setValue(HW_MINPRESSURE, pressure);
    emit minPressureChanged();
}

qreal
HWSettings::minPressure() const {
    return value(HW_MINPRESSURE, 0.215).toReal();
}

void
HWSettings::setMaxPressure(qreal pressure) {
    setValue(HW_MAXPRESSURE, pressure);
    emit maxPressureChanged();
}

qreal
HWSettings::maxPressure() const {
    return value(HW_MAXPRESSURE, 0.345).toReal();
}

void
HWSettings::setMinEllipseDiameters(qreal ELLD) {
    setValue(HW_MINELLIPSEDIAMETERS, ELLD);
    emit minEllipseDiametersChanged();
}

qreal
HWSettings::minEllipseDiameters() const {
    return value(HW_MINELLIPSEDIAMETERS, 30).toReal();
}

void
HWSettings::setMaxEllipseDiameters(qreal ELLD) {
    setValue(HW_MAXELLIPSEDIAMETERS, ELLD);
    emit maxEllipseDiametersChanged();
}

qreal
HWSettings::maxEllipseDiameters() const {
    return value(HW_MAXELLIPSEDIAMETERS, 80).toReal();
}*/

void
HWSettings::setVelocityThreshold(qreal vel) {
    setValue(HW_VELOCITYTHRESHOLD, vel);
    emit velocityThresholdChanged();
}

qreal
HWSettings::velocityThreshold() const {
    return value(HW_VELOCITYTHRESHOLD, 400).toReal();
}

void
HWSettings::setFadeoutLimit(qreal fol) {
    setValue(HW_FADEOUTLIMIT, fol);
    emit fadeoutLimitChanged();
}

qreal
HWSettings::fadeoutLimit() const {
    return value(HW_FADEOUTLIMIT, 1).toReal();
}

void
HWSettings::setFadeinLimit(qreal fil) {
    setValue(HW_FADEINLIMIT, fil);
    emit fadeinLimitChanged();
}

qreal
HWSettings::fadeinLimit() const {
    return value(HW_FADEINLIMIT, 1).toReal();
}

void
HWSettings::restore2DefaultPen() {
    remove(HW_PEN);

    emit strokeSizeChanged();
    emit minPenSizeChanged();
    // emit maxPenSizeRatioChanged();
    /*emit minPressureChanged();
    emit maxPressureChanged();
    emit minEllipseDiametersChanged();
    emit maxEllipseDiametersChanged();*/
    emit velocityThresholdChanged();
    emit fadeoutLimitChanged();
    emit fadeinLimitChanged();
}

void
HWSettings::setZoomRatio(qreal ratio) {
    setValue(HW_ZOOMRATIO, ratio);

    emit zoomRatioChanged();
}

qreal
HWSettings::zoomRatio() const {
    return value(HW_ZOOMRATIO, 10).toReal();
}

void
HWSettings::setAutoScrollInterval(int asi) {
    setValue(HW_AUTOSCROLLINTERVAL, asi);
    emit autoScrollIntervalChanged();
}

int
HWSettings::autoScrollInterval() const {
    return value(HW_AUTOSCROLLINTERVAL, 750).toInt();
}

void
HWSettings::setAutoScrollLineHeight(int lh) {
    setValue(HW_AUTOSCROLLLINEHEIGHT, lh);
    emit autoScrollLineHeightChanged();
}

int
HWSettings::autoScrollLineHeight() const {
    return value(HW_AUTOSCROLLLINEHEIGHT, 32).toInt();
}

void
HWSettings::setAutoScrollNewlineLeft(int nl) {
    setValue(HW_AUTOSCROLLNEWLINELEFT, nl);
    emit autoScrollNewlineLeftChanged();
}

int
HWSettings::autoScrollNewlineLeft() const {
    return value(HW_AUTOSCROLLNEWLINELEFT, 32).toInt();
}

void
HWSettings::setAutoScrollToRight(int tl) {
    setValue(HW_AUTOSCROLLTORIGHT, tl);
    emit autoScrollToRightChanged();
}

int
HWSettings::autoScrollToRight() const {
    return value(HW_AUTOSCROLLTORIGHT, 16).toInt();
}

void
HWSettings::setAutoScrollTriggerPos(qreal tp) {
    setValue(HW_AUTOSCROLLTRIGGERPOS, tp);
    emit autoScrollTriggerPos();
}

qreal
HWSettings::autoScrollTriggerPos() const {
    return value(HW_AUTOSCROLLTRIGGERPOS, 0.66).toReal();
}

void
HWSettings::restore2DefaultZoomAutoScroll() {
    remove(HW_ZOOM);
    emit zoomRatioChanged();
    emit autoScrollIntervalChanged();
    emit autoScrollLineHeightChanged();
    emit autoScrollNewlineLeftChanged();
    emit autoScrollToRightChanged();
}

QVariant
HWSettings::penPresets() const {
    return value(HW_PENPRESETS, m_defaultPenPresets);
}

void
HWSettings::setPenPresets(const QVariant& cp) {
    setValue(HW_PENPRESETS, cp.toList());
    emit penPresetsChanged();
}

void
HWSettings::restore2DefaultPenPresets() {
    remove(HW_PENPRESETS);
    emit penPresetsChanged();
}

int
HWSettings::inkConsumption() const {
    return m_inkConsumption;
}

void
HWSettings::setInkConsumption(int c) {
    m_inkConsumption = c;
    setValue(HW_INK_CONSUMPTION, m_inkConsumption);
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
    clear();
    emit strokeSizeChanged();
    emit defaultStrokeSizeChanged();
    emit defaultStrokeSizeRuleChanged();
    emit minPenSizeChanged();
    // emit maxPenSizeRatioChanged();
    /*emit minPressureChanged();
    emit maxPressureChanged();*/
    emit zoomRatioChanged();
    emit penPresetsChanged();
}

QVariantList
HWSettings::userSavedStrokes() const {
    return value(HW_USERSAVEDSTROKES, QVariantList{}).toList();
}

void
HWSettings::setUserSavedStrokes(const QVariantList& userSavedStrokes) {
    setValue(HW_USERSAVEDSTROKES, userSavedStrokes);
    emit userSavedStrokesChanged();
}

void
HWSettings::saveUserStroke(const QJSValue& options) {
    QVariantList userSavedStrokes = this->userSavedStrokes();
    userSavedStrokes.append(options.toVariant());
    setUserSavedStrokes(userSavedStrokes);
}

void
HWSettings::updateUserStroke(int index, const QJSValue& options) {
    QVariantList userSavedStrokes = this->userSavedStrokes();
    userSavedStrokes[index] = options.toVariant();
    setUserSavedStrokes(userSavedStrokes);
}

void
HWSettings::removeUserStroke(int index) {
    QVariantList userSavedStrokes = this->userSavedStrokes();
    userSavedStrokes.remove(index);
    setUserSavedStrokes(userSavedStrokes);
}
