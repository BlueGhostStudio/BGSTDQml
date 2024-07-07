#include "hwwriter.h"

#include <hwcanvas.h>

#include "hwsettings.h"
#include "qforeach.h"

HWWriter::HWWriter(QQuickItem* parent) : HWCanvas(parent) {}

void
HWWriter::classBegin() {
    HWCanvas::classBegin();

    m_guideLine = new HWGuideLine(this);
    m_guideLine->setZ(-1);
    m_guideLine->setVisible(false);

    QObject* anchors = qobject_cast<QObject*>(
        m_guideLine->property("anchors").value<QObject*>());
    if (anchors) anchors->setProperty("fill", QVariant::fromValue(this));

    /*bool isTouchScreen = false;
    foreach (const QInputDevice* device, QInputDevice::devices()) {
        if (device->type() == QInputDevice::DeviceType::TouchScreen) {
            isTouchScreen = true;
            break;
        }
    }*/

    if (m_settings->isTouchScreen())
        setAcceptTouchEvents(true);
    else
        setAcceptedMouseButtons(Qt::AllButtons);

    // m_pressureSupport = (PressureSupport)m_settings->pressureSupport();

    loadSettings();
}

void
HWWriter::componentComplete() {
    HWCanvas::componentComplete();

    QObject::connect(this, &HWWriter::scaleChanged, this,
                     &HWWriter::cumulativeScale);

    QQuickItem* theParentItem = parentItem();
    while (theParentItem) {
        QObject::connect(theParentItem, &QQuickItem::scaleChanged, this,
                         &HWWriter::cumulativeScale);
        theParentItem = theParentItem->parentItem();
    }

    QObject::connect(this, &HWCanvas::cleared, this, [=]() {
        m_maxVelocity = 0;
        emit maxVelocityChanged();
    });
}

void
HWWriter::loadSettings() {
    setStrokeType(Ballpoint);
    setStrokeColor(Black);
    setStrokeSize(m_settings->strokeSize());

    /*setPressureSupport((PressureSupport)m_settings->pressureSupport());

    setMinPressure(m_settings->minPressure());
    setMaxPressure(m_settings->maxPressure());

    setMinEllipseDiameters(m_settings->minEllipseDiameters());
    setMaxEllipseDiameters(m_settings->maxEllipseDiameters());*/

    setVelocityThreshold(m_settings->velocityThreshold());

    setMinPenSize(m_settings->minPenSize());

    setFadeinLimit(m_settings->fadeinLimit());
    setFadeoutLimit(m_settings->fadeoutLimit());
}

bool
HWWriter::loadSettings(int preset) {
    QVariantList penPresets = m_settings->penPresets().toList();
    if (preset < penPresets.length()) {
        loadSettings(qmlEngine(this)->toScriptValue(penPresets[preset]));
        /*qDebug() <<
        qmlEngine(this)->toScriptValue(penPresets[preset]).property("size").toNumber();
        QVariantMap mPen = penPresets[preset].toMap();
        qDebug() << qjsEngine(this);

        setStrokeType(mPen.contains("type") ? (StrokeType)mPen["type"].toInt()
                                            : Ballpoint);
        setStrokeSize(mPen.contains("size") ? mPen["size"].toReal()
                                            : m_settings->strokeSize());

        setVelocityThreshold(mPen.contains("velocityThreshold")
                                 ? mPen["velocityThreshold"].toReal()
                                 : m_settings->velocityThreshold());

        setMinPenSize(mPen.contains("minSize") ? mPen["minSize"].toReal()
                                               : m_settings->minPenSize());

        setFadeinLimit(mPen.contains("fadeinLimit")
                           ? mPen["fadeinLimit"].toReal()
                           : m_settings->fadeinLimit());
        setFadeoutLimit(mPen.contains("fadeoutLimit")
                            ? mPen["fadeoutLimit"].toReal()
                            : m_settings->fadeoutLimit());*/

        return true;
    } else
        return false;
}

void
HWWriter::loadSettings(const QJSValue& strokeOptions) {
    if (strokeOptions.isObject()) {
        QJSValue jsvStrokeProp;

        jsvStrokeProp = strokeOptions.property("strokeType");
        setStrokeType(jsvStrokeProp.isUndefined()
                          ? Ballpoint
                          : (StrokeType)jsvStrokeProp.toInt());

        jsvStrokeProp = strokeOptions.property("strokeColor");
        setStrokeColor(jsvStrokeProp.isUndefined()
                           ? Black
                           : (StrokeColor)qMin(jsvStrokeProp.toInt(), 6));

        jsvStrokeProp = strokeOptions.property("strokeSize");
        setStrokeSize(jsvStrokeProp.isUndefined() ? m_settings->strokeSize()
                                                  : jsvStrokeProp.toNumber());

        jsvStrokeProp = strokeOptions.property("minPenSize");
        setMinPenSize(jsvStrokeProp.isUndefined() ? m_settings->minPenSize()
                                                  : jsvStrokeProp.toNumber());

        jsvStrokeProp = strokeOptions.property("fadeoutLimit");
        setFadeoutLimit(jsvStrokeProp.isUndefined() ? m_settings->fadeoutLimit()
                                                    : jsvStrokeProp.toNumber());

        jsvStrokeProp = strokeOptions.property("fadeinLimit");
        setFadeinLimit(jsvStrokeProp.isUndefined() ? m_settings->fadeinLimit()
                                                   : jsvStrokeProp.toNumber());

        jsvStrokeProp = strokeOptions.property("velocityThreshold");
        setVelocityThreshold(jsvStrokeProp.isUndefined()
                                 ? m_settings->velocityThreshold()
                                 : jsvStrokeProp.toNumber());

        emit strokeOptionsChanged();
    }
}

HWGuideLine*
HWWriter::guideLine() const {
    return m_guideLine;
}

void
HWWriter::setStrokeOptions(const QJSValue& options) {
    loadSettings(options);
}

QJSValue
HWWriter::strokeOptions() const {
    QJSValue options;
    options.setProperty("strokeType", m_strokeType);
    options.setProperty("strokeColor", m_strokeColor);
    options.setProperty("strokeSize", m_strokeSize);
    options.setProperty("minPenSize", m_minPenSize);
    options.setProperty("fadeoutLimit", m_fadeoutLimit);
    options.setProperty("fadeinLimit", m_fadeinLimit);
    options.setProperty("velocityThreshold", m_velocityThreshold);

    return options;
}

void
HWWriter::setStrokeType(HWWriter::StrokeType type) {
    m_strokeType = type;
    emit strokeTypeChanged();
}

HWWriter::StrokeType
HWWriter::strokeType() const {
    return (StrokeType)m_strokeType;
}

void
HWWriter::setStrokeColor(HWWriter::StrokeColor color) {
    m_strokeColor = color;
    emit strokeColorChanged();
}

HWWriter::StrokeColor
HWWriter::strokeColor() const {
    return (StrokeColor)m_strokeColor;
}

void
HWWriter::setStrokeSize(qreal size) {
    m_strokeSize = size;
    emit strokeSizeChanged();
}

qreal
HWWriter::strokeSize() const {
    return m_strokeSize;
}

/*void
HWWriter::setPressureSupport(PressureSupport type) {
    m_pressureSupport = type;
    emit pressureSupportChanged();
}

HWWriter::PressureSupport
HWWriter::pressureSupport() const {
    return m_pressureSupport;
}

void
HWWriter::setMinPressure(qreal pressure) {
    m_minPressure = pressure;
    emit minPressureChanged();
}

qreal
HWWriter::minPressure() const {
    return m_minPressure;
}

void
HWWriter::setMaxPressure(qreal pressure) {
    m_maxPressure = pressure;
    emit maxPressureChanged();
}

qreal
HWWriter::maxPressure() const {
    return m_maxPressure;
}

void
HWWriter::setMinEllipseDiameters(qreal ELLD) {
    m_minEllipseDiameters = ELLD;

    emit minEllipseDiametersChanged();
}

qreal
HWWriter::minEllipseDiameters() const {
    return m_minEllipseDiameters;
}

void
HWWriter::setMaxEllipseDiameters(qreal ELLD) {
    m_maxEllipseDiameters = ELLD;

    emit maxEllipseDiametersChanged();
}

qreal
HWWriter::maxEllipseDiameters() const {
    return m_maxEllipseDiameters;
}*/

/*qreal
HWWriter::minPE() const {
    switch (m_pressureSupport) {
    case PressureSupport::Pressure:
        return m_minPressure;
    case PressureSupport::TouchSize:
        return m_minEllipseDiameters;
    case PressureSupport::None:
    default:
        return -1;
    }
}

qreal
HWWriter::maxPE() const {
    switch (m_pressureSupport) {
    case PressureSupport::Pressure:
        return m_maxPressure;
    case PressureSupport::TouchSize:
        return m_maxEllipseDiameters;
    case PressureSupport::None:
    default:
        return -1;
    }
}*/

void
HWWriter::setVelocityThreshold(qreal vel) {
    m_velocityThreshold = vel;

    emit velocityThresholdChanged();
    emit scaledVelocityThresholdChanged();
}

qreal
HWWriter::velocityThreshold() const {
    return m_velocityThreshold;
}

qreal
HWWriter::scaledVelocityThreshold() const {
    // return m_combinedScale > 1 ? m_velocityThreshold *
    // m_m_scaledVelocityThreshold;
    return m_velocityThreshold * qMax(m_combinedScale, 1.00);
}

qreal
HWWriter::maxVelocity() const {
    return m_maxVelocity;
}

qreal
HWWriter::combinedScale() const {
    return m_combinedScale;
}

void
HWWriter::setMinPenSize(qreal size) {
    m_minPenSize = size;
    emit minPenSizeChanged();
}

qreal
HWWriter::minPenSize() const {
    return m_minPenSize;
}

void
HWWriter::setReadOnly(bool ro) {
    m_readOnly = ro;

    if (m_readOnly) {
        if (m_settings->isTouchScreen())
            setAcceptTouchEvents(false);
        else
            setAcceptedMouseButtons(Qt::NoButton);
    } else {
        if (m_settings->isTouchScreen())
            setAcceptTouchEvents(true);
        else
            setAcceptedMouseButtons(Qt::AllButtons);
    }

    emit readOnlyChanged();
}

bool
HWWriter::readOnly() const {
    return m_readOnly;
}

void
HWWriter::setFadeoutLimit(qreal fol) {
    m_fadeoutLimit = fol;
    emit fadeoutLimitChanged();
}

qreal
HWWriter::fadeoutLimit() const {
    return m_fadeoutLimit;
}

void
HWWriter::setFadeinLimit(qreal fil) {
    m_fadeinLimit = fil;
    emit fadeinLimitChanged();
}

qreal
HWWriter::fadeinLimit() const {
    return m_fadeinLimit;
}

bool
HWWriter::writing() const {
    return m_writing;
}

void
HWWriter::setStrokeSizeCalc(const QJSValue& callback) {
    m_strokeSizeCalcFun = callback;
    emit strokeSizeCalcChanged();
}

QJSValue
HWWriter::strokeSizeCalc() const {
    return m_strokeSizeCalcFun;
}

qreal
HWWriter::fadeLimitCalc(qreal preSize, qreal size) const {
    if (preSize > 0) {
        if (m_fadeinLimit > 0) size = qMin(preSize * m_fadeinLimit, size);
        if (m_fadeoutLimit > 0) size = qMax(preSize * m_fadeoutLimit, size);
    }

    return size;
}

bool
HWWriter::recording() const {
    return m_record;
}

void
HWWriter::record() {
    m_recordedStrokes.clear();
    m_record = true;
    emit recordingChanged();
}

void
HWWriter::stopRecord() {
    m_record = false;
    emit recordingChanged();
}

void
HWWriter::replayRecord() {
    // stopRecord();
    clear();

    replayStrokesWrite(m_recordedStrokes);
}

void
HWWriter::resetRecord() {
    clear();
    record();
}

void
HWWriter::penDown(const StrokePoint& point, bool signal) {
    m_prePos = point.position;
    m_preSize = calcStrokeSize(-1, point);
    if (signal) emit writeStart(point.position, point);
}

Stroke
HWWriter::penSegment(const StrokePoint& point, bool signal) {
    qreal size = calcStrokeSize(m_preSize, point);
    if (qMax(m_preSize, size) / 2 > QLineF(m_prePos, point.position).length())
        return Stroke();

    /*QVariant strokeVariant =
        toStrokeVariant(m_strokeType, m_strokeColor, point.position, m_prePos,
                        m_strokeType == 0 ? m_strokeSize : size,
                        m_strokeType == 0 ? m_strokeSize : m_preSize);*/
    Stroke stroke{ point.position,
                   m_prePos,
                   m_strokeType,
                   m_strokeColor,
                   m_strokeType == 0 ? m_strokeSize : size,
                   m_strokeType == 0 ? m_strokeSize : m_preSize };
    drawStroke(stroke);
    // drawStroke(strokeVariant);

    // emit strokeUpdated(strokeVariant);
    // m_settings->setPressureSupport(0);
    m_preSize = size;
    m_prePos = point.position;

    if (signal && stroke.type != -1) emit writeUpdated(stroke, point);

    return stroke;
}

void
HWWriter::cumulativeScale() {
    qreal theScale = scale();

    QQuickItem* theParentItem = parentItem();
    while (theParentItem) {
        theScale *= theParentItem->scale();
        theParentItem = theParentItem->parentItem();
    }

    if (!qFuzzyCompare(theScale, m_combinedScale)) {
        m_maxVelocity = 0;
        m_combinedScale = theScale;
        emit combinedScaleChanged();
        emit scaledVelocityThreshold();
    }
}

/*void
HWWriter::replayStrokesWrite(const QJSValue& strokes) {
    if (!strokes.isArray()) return;

    for (int i = 0; i < strokes.property("length").toInt(); ++i) {
        StrokePoint point;
        point.position.setX(strokes.property(i).property("x").toNumber());
        point.position.setY(strokes.property(i).property("y").toNumber());
        point.velocity = strokes.property(i).property("velocity").toNumber();

        switch (strokes.property(i).property("event").toInt()) {
        case 0:
            penDown(point);
            break;
        case 1:
            penSegment(point);
            break;
        }
    }
}*/

/*qreal
HWWriter::getPE(const t_strokePoint& point) const {
    switch (m_pressureSupport) {
    case PressureSupport::Pressure:
        return point.pressure;
    case PressureSupport::TouchSize:
        return point.ellipseDiameters;
    case PressureSupport::None:
    default:
        return -1;
    }
}*/

QVariant
HWWriter::toStrokeVariant(int type, int color, const QPointF& pos,
                          const QPointF& prePos, qreal size,
                          qreal preSize) const {
    QVariantMap strokeMap;
    strokeMap["type"] = type;
    strokeMap["color"] = color;

    QVariantMap strokePosMap;

    strokePosMap["x"] = pos.x();
    strokePosMap["y"] = pos.y();
    strokeMap["pos"] = strokePosMap;

    strokePosMap["x"] = prePos.x();
    strokePosMap["y"] = prePos.y();
    strokeMap["prePos"] = strokePosMap;

    strokeMap["size"] = size;
    strokeMap["preSize"] = preSize;

    return strokeMap;
}

qreal
HWWriter::calcStrokeSize(qreal preSize, const StrokePoint& point) {
    // qreal PE = getPE(point);
    /*qreal theScale = scale();
    QQuickItem* item = parentItem();
    while (item) {
        theScale *= item->scale();
        item = item->parentItem();
    }*/

    // qreal velocity = point.velocity * theScale;
    // qDebug() << velocity << point.velocity << point.velocity / theScale;
    qreal velocity = point.velocity /* - 70 * theScale*/;

    if (velocity > m_maxVelocity) {
        m_maxVelocity = velocity;
        emit maxVelocityChanged();
    }

    if (m_strokeSizeCalcFun.isUndefined())
        return defaultStrokeSizeCalc(preSize,
                                     /*PE,*/ velocity);
    else if (m_strokeSizeCalcFun.isCallable())
        return m_strokeSizeCalcFun
            .call(QJSValueList() << preSize /*<< PE*/ << velocity)
            .toNumber();
    else
        return m_strokeSize;
}

qreal
HWWriter::defaultStrokeSizeCalc(qreal preSize,
                                /*qreal PE,*/ qreal velocity) const {
    if (preSize == -1) {
        if (m_fadeoutLimit > 0)
            return m_strokeSize * m_fadeoutLimit;
        else
            return m_minPenSize;  // m_strokeSize;
    } else {
        auto f = [](qreal x, qreal r, qreal o) -> qreal {
            return qCos(qMin(x / r, 1.00) * M_PI + o) / 2 + 0.5;
            // return qMax(1.00 - x / r, 0.00);

            /*qreal v = qMin(x, r);
            qreal vth_2 = qPow(r, 2);
            return (vth_2 - qPow(v, 2)) / vth_2;*/
        };

        // qreal PEScale = 1;
        /*if (m_pressureSupport != PressureSupport::None) {
            qreal minPE = HWWriter::minPE();
            qreal maxPE = HWWriter::maxPE();
            qreal PER = maxPE - minPE;
            qreal avPE = qMin(qMax(PE, minPE), maxPE) - minPE;

            PEScale = f(avPE, PER, M_PI);
        }*/

        // velocity /= 10;
        qreal velocityScale = f(velocity, scaledVelocityThreshold(), 0);

        // qreal maxSize = m_strokeSize * m_maxPenSizeRatio;
        qreal minSize = m_minPenSize;
        qreal sizeR = m_strokeSize - minSize;

        // qreal size = PEScale * velocityScale * sizeR + minSize;
        qreal size = velocityScale * sizeR + minSize;

        return fadeLimitCalc(preSize, size);
    }
}

void
HWWriter::replayStrokesWrite(const QList<StrokePoint>& strokePoints) {
    foreach (const auto& point, strokePoints) {
        switch (point.event) {
        case 0:
            penDown(point, false);
            break;
        case 1:
            penSegment(point, false);
            break;
        }
    }
}

void
HWWriter::mousePressEvent(QMouseEvent* event) {
    if (!m_readOnly && event->button() == Qt::LeftButton &&
        event->modifiers() == Qt::NoModifier) {
        m_writing = true;
        emit writingChanged();

        StrokePoint point{ 0, event->point(0) };
        penDown(point);
        // emit writeStart(point.position, point);
        // if (m_record) m_recordedStrokes.append({ 0, point });
    } else
        event->ignore();

    emit clicked(event->position());
}

void
HWWriter::mouseMoveEvent(QMouseEvent* event) {
    if (!m_readOnly) {
        m_writing = true;
        emit writingChanged();

        StrokePoint point{ 1, event->point(0) };
        Stroke theStroke = penSegment(point);
        /*if (theStroke.type != -1) {
            emit writeUpdated(theStroke, point);
        }*/

        // if (m_record) m_recordedStrokes.append({ 1, point });
    } else
        m_prePos = event->position();
}

void
HWWriter::mouseReleaseEvent(QMouseEvent* event) {
    m_writing = false;
    emit writingChanged();

    if (event->button() == Qt::LeftButton) emit writeEnd();
}

void
HWWriter::touchEvent(QTouchEvent* event) {
    if (event->touchPointStates() == Qt::TouchPointReleased) {
        if (m_pressTimeStamp > 0 &&
            event->timestamp() - m_pressTimeStamp > 500) {
            event->ignore();
            emit writeIgnore();
        } else if (!m_keepTouchIgnore)
            emit writeEnd();

        m_writing = false;
        emit writingChanged();

        m_keepTouchIgnore = false;
        m_preSize = -1;
    } else if (event->points().count() > 1) {
        m_keepTouchIgnore = true;
        event->ignore();
    } else if (m_keepTouchIgnore)
        event->ignore();
    else {
        StrokePoint point{ 0, event->points()[0] };
        QPointF pos = point.position;

        m_writing = true;
        emit writingChanged();

        switch (event->touchPointStates()) {
        case Qt::TouchPointPressed:
            /*if (m_readOnly)*/ emit clicked(pos);
            /*m_prePos = pos;
            m_preSize = calcStrokeSize(-1, point);*/
            m_pressTimeStamp = event->timestamp();

            point.event = 0;
            penDown(point);
            // emit writeStart(pos, point);
            break;
        case Qt::TouchPointMoved:
            /*qreal size = calcStrokeSize(m_preSize, point);
            QRectF measureDistRect(m_prePos, pos);
            qreal measureDist =
                qSqrt(measureDistRect.width() * measureDistRect.width() +
                      measureDistRect.height() * measureDistRect.height());*/
            qreal dist = QLineF(m_prePos, pos).length();
            if (dist > 0.5) {
                if (!m_readOnly) {
                    m_pressTimeStamp = 0;
                    /*QVariant strokeVarinat;
                    strokeVarinat = toStrokeVariant(
                        m_strokeType, m_strokeColor, pos, m_prePos,
                        m_strokeType == 0 ? m_strokeSize : size,
                        m_strokeType == 0 ? m_strokeSize : m_preSize);

                    drawStroke(strokeVarinat);
                    emit strokeUpdated(strokeVarinat);*/
                    point.event = 1;
                    Stroke theStroke = penSegment(point);
                    /*if (theStroke.type != -1) {
                        emit writeUpdated(theStroke, point);
                    }*/

                    /*qreal ELLD = point.ellipseDiameters().width();
                    qreal pressure = point.pressure();
                    if (m_record) {
                        emit recording(m_prePos, pos, ELLD, pressure);
                    }*/
                }
                /*m_prePos = pos;
                m_preSize = size;*/
            }
            break;
        }
    }
}
