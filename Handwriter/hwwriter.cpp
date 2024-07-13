#include "hwwriter.h"

#include <hwcanvas.h>

#include "hwsettings.h"
#include "qforeach.h"

HWWriter::HWWriter(QQuickItem* parent) : HWCanvas(parent) {}

void
HWWriter::classBegin() {
    HWCanvas::classBegin();

    /*m_guideLine = new HWGuideLine(this);
    m_guideLine->setZ(-1);
    m_guideLine->setVisible(false);

    QObject* anchors = qobject_cast<QObject*>(
        m_guideLine->property("anchors").value<QObject*>());
    if (anchors) anchors->setProperty("fill", QVariant::fromValue(this));*/

    loadPresetWriteParameter(0);
}

void
HWWriter::componentComplete() {
    HWCanvas::componentComplete();

    QObject::connect(this, &HWCanvas::combinedScaleChanged, this,
                     &HWWriter::scaledVelocityThresholdChanged);

    QObject::connect(this, &HWCanvas::cleared, this, [=]() {
        m_maxVelocity = 0;
        emit maxVelocityChanged();
    });
}

void
HWWriter::loadDefaultWriteParameter() {
    setStrokeType(Ballpoint);
    setStrokeColor(Black);
    setStrokeSize(m_settings->strokeSize());

    setVelocityThreshold(m_settings->velocityThreshold());

    setMinPenSize(m_settings->minPenSize());

    setFadeinLimit(m_settings->fadeinLimit());
    setFadeoutLimit(m_settings->fadeoutLimit());
}

bool
HWWriter::loadPresetWriteParameter(int presetIndex) {
    QVariantList penPresets = m_settings->penPresets();
    if (presetIndex < penPresets.length()) {
        // loadWriteParameter(qmlEngine(this)->toScriptValue(penPresets[preset]));
        QJSValue presetParam =
            qmlEngine(this)->toScriptValue(penPresets[presetIndex]);
        presetParam.setProperty("method", Preset);
        presetParam.setProperty("index", presetIndex);
        loadWriteParameter(presetParam);

        return true;
    } else
        return false;
}

bool
HWWriter::loadUserSavedWriteParameter(int userSavedIndex) {
    QVariantList penUserSaved = m_settings->userSavedStrokes();
    if (userSavedIndex < penUserSaved.length()) {
        QJSValue presetParam =
            qmlEngine(this)->toScriptValue(penUserSaved[userSavedIndex]);
        presetParam.setProperty("method", UseSaved);
        presetParam.setProperty("index", userSavedIndex);
        loadWriteParameter(presetParam);

        return true;
    } else
        return false;
}

void
HWWriter::loadWriteParameter(const QJSValue& writeParameter) {
    if (writeParameter.isObject()) {
        QJSValue jsvProp;

        jsvProp = writeParameter.property("strokeType");
        setStrokeType(
            jsvProp.isUndefined() ? Ballpoint : (StrokeType)jsvProp.toInt(),
            false);

        jsvProp = writeParameter.property("strokeColor");
        if (!jsvProp.isUndefined())
            setStrokeColor((StrokeColor)qMin(jsvProp.toInt(), 6));

        jsvProp = writeParameter.property("strokeSize");
        setStrokeSize(jsvProp.isUndefined() ? m_settings->strokeSize()
                                            : jsvProp.toNumber(),
                      false);

        jsvProp = writeParameter.property("minPenSize");
        setMinPenSize(jsvProp.isUndefined() ? m_settings->minPenSize()
                                            : jsvProp.toNumber(),
                      false);

        jsvProp = writeParameter.property("fadeoutLimit");
        setFadeoutLimit(jsvProp.isUndefined() ? m_settings->fadeoutLimit()
                                              : jsvProp.toNumber(),
                        false);

        jsvProp = writeParameter.property("fadeinLimit");
        setFadeinLimit(jsvProp.isUndefined() ? m_settings->fadeinLimit()
                                             : jsvProp.toNumber(),
                       false);

        jsvProp = writeParameter.property("velocityThreshold");
        setVelocityThreshold(jsvProp.isUndefined()
                                 ? m_settings->velocityThreshold()
                                 : jsvProp.toNumber(),
                             false);

        jsvProp = writeParameter.property("__method__");
        if (jsvProp.isUndefined()) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit writeParameterChanged(Custom, -1);
        } else {
            // WriteParameterMethod method =
            // (WriteParameterMethod)jsvProp.toInt();
            m_writeParameterMethod = (WriteParameterMethod)jsvProp.toInt();
            jsvProp = writeParameter.property("__index__");
            m_writeParameterIndex =
                jsvProp.isUndefined() ? -1 : jsvProp.toInt();
            emit writeParameterChanged(m_writeParameterMethod,
                                       m_writeParameterIndex);
        }
    }
}

QJSValue
HWWriter::writeParameter() const {
    QJSValue options = qmlEngine(this)->newObject();
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
HWWriter::saveUserSavedWriteParameter(const QJSValue& writeParameter) {
    int index = m_settings->saveUserStroke(writeParameter);

    // QJSValue theWriteParameter(std::move(writeParameter));
    /*theWriteParameter.setProperty("method", HWWriter::UseSaved);
    theWriteParameter.setProperty("index", index);*/
    emit userSavedWriteParameterSaved(index, writeParameter);
}

void
HWWriter::updateUserSavedWriteParameter(int index,
                                        const QJSValue& writeParameter) {
    m_settings->updateUserStroke(index, writeParameter);
    emit userSavedWriteParameterUpdated(index, writeParameter);
}

void
HWWriter::removeUserSavedWriteParameter(int index) {
    m_settings->removeUserStroke(index);
    emit userSavedWriteParameterRemoved(index);
}

/*HWGuideLine*
HWWriter::guideLine() const {
    return m_guideLine;
}*/

/*void
HWWriter::setStrokeOptions(const QJSValue& options) {
    loadWriteParameter(options);
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
}*/

void
HWWriter::setStrokeType(HWWriter::StrokeType type, bool adjusted) {
    if (m_strokeType != type) {
        m_strokeType = type;
        emit strokeTypeChanged();

        if (adjusted) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit strokeAdjusted();
        }
    }
}

HWWriter::StrokeType
HWWriter::strokeType() const {
    return (StrokeType)m_strokeType;
}

void
HWWriter::setStrokeColor(HWWriter::StrokeColor color /*, bool adjusted*/) {
    if (m_strokeColor != color) {
        m_strokeColor = color;
        if (m_writeParameterMethod == UseSaved) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
        }

        emit strokeColorChanged();
    }
}

HWWriter::StrokeColor
HWWriter::strokeColor() const {
    return (StrokeColor)m_strokeColor;
}

void
HWWriter::setStrokeSize(qreal size, bool adjusted) {
    if (!qFuzzyCompare(m_strokeSize, size)) {
        m_strokeSize = size;
        emit strokeSizeChanged();

        if (adjusted) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit strokeAdjusted();
        }
    }
}

qreal
HWWriter::strokeSize() const {
    return m_strokeSize;
}

void
HWWriter::setMinPenSize(qreal size, bool adjusted) {
    if (!qFuzzyCompare(m_minPenSize, size)) {
        m_minPenSize = size;
        emit minPenSizeChanged();

        if (adjusted) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit strokeAdjusted();
        }
    }
}

qreal
HWWriter::minPenSize() const {
    return m_minPenSize;
}

qreal
HWWriter::fadeoutLimit() const {
    return m_fadeoutLimit;
}

void
HWWriter::setFadeoutLimit(qreal fol, bool adjusted) {
    if (!qFuzzyCompare(m_fadeoutLimit, fol)) {
        m_fadeoutLimit = fol;
        emit fadeoutLimitChanged();

        if (adjusted) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit strokeAdjusted();
        }
    }
}

void
HWWriter::setFadeinLimit(qreal fil, bool adjusted) {
    if (!qFuzzyCompare(m_fadeinLimit, fil)) {
        m_fadeinLimit = fil;
        emit fadeinLimitChanged();

        if (adjusted) {
            m_writeParameterMethod = Custom;
            m_writeParameterIndex = -1;
            emit strokeAdjusted();
        }
    }
}

qreal
HWWriter::fadeinLimit() const {
    return m_fadeinLimit;
}

void
HWWriter::setVelocityThreshold(qreal vel, bool adjusted) {
    if (!qFuzzyCompare(m_velocityThreshold, vel)) {
        m_velocityThreshold = vel;

        m_writeParameterMethod = Custom;
        m_writeParameterIndex = -1;

        if (adjusted) {
            emit velocityThresholdChanged();
            emit scaledVelocityThresholdChanged();
            emit strokeAdjusted();
        }
    }
}

qreal
HWWriter::velocityThreshold() const {
    return m_velocityThreshold;
}

int
HWWriter::velocityFactor() const {
    return m_velocityFactor;
}

void
HWWriter::setVelocityFactor(int scaleAdjustsVelocity) {
    if (m_velocityFactor != scaleAdjustsVelocity) {
        m_velocityFactor = scaleAdjustsVelocity;
        emit velocityFactorChanged();
    }
}

qreal
HWWriter::scaledVelocityThreshold() const {
    return m_velocityThreshold *
           qMax(m_velocityFactor != -1 ? m_velocityFactor : m_combinedScale,
                1.00);
}

qreal
HWWriter::maxVelocity() const {
    return m_maxVelocity;
}

bool
HWWriter::writing() const {
    return m_writing;
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
HWWriter::setStrokeSizeCalc(const QJSValue& callback) {
    m_strokeSizeCalcFun = callback;
    emit strokeSizeCalcChanged();
}

QJSValue
HWWriter::strokeSizeCalc() const {
    return m_strokeSizeCalcFun;
}

HWWriter::WriteParameterMethod
HWWriter::writeParameterMethod() const {
    return m_writeParameterMethod;
}

int
HWWriter::writeParameterIndex() const {
    return m_writeParameterIndex;
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
    clear();

    replayStrokesWrite(m_recordedStrokes);
}

QVariantList
HWWriter::exportRecord() {
    qreal minX = m_range.x();
    qreal minY = m_range.y();

    QVariantList vlRecords;

    foreach (auto point, m_recordedStrokes) {
        QVariantMap vmPoint;

        vmPoint["event"] = point.event;
        /*vmPoint["position"] = QVariantMap(
            { { "x", qFloor((point.position.x() - minX) * 100) / 100.00 },
              { "y", qFloor((point.position.y() - minY) * 100) / 100.00 } });*/
        vmPoint["position"] = QVariantMap{
            { "x", qFloor((point.position.x() - minX) * 100) / 100.00 },
            { "y", qFloor((point.position.y() - minY) * 100) / 100.00 }
        };
        vmPoint["velocity"] = qFloor(point.velocity * 100) / 100.00;

        vlRecords.append(vmPoint);
    }
    QFile strokeSampleFile(
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) +
        "/strokeSample.json");
    if (strokeSampleFile.open(QIODevice::WriteOnly)) {
        strokeSampleFile.write(QJsonDocument::fromVariant(vlRecords).toJson());
        strokeSampleFile.close();
    } else
        qDebug() << "can't write file";

    return vlRecords;
}

void
HWWriter::resetRecord() {
    clear();
    record();
}

void
HWWriter::penDown(const StrokePoint& point, bool signal) {
    m_penDownStrokeDrawed = false;
    m_prePos = point.position;
    m_preSize = calcStrokeSize(-1, point);
    if (signal) emit writeStart(point.position, point);
}

Stroke
HWWriter::penSegment(const StrokePoint& point, bool signal) {
    qreal size = calcStrokeSize(m_preSize, point);
    /*if (qMax(m_preSize, size) / 2 > QLineF(m_prePos, point.position).length())
        // size = m_preSize;
        return Stroke();*/

    /*QVariant strokeVariant =
        toStrokeVariant(m_strokeType, m_strokeColor, point.position, m_prePos,
                        m_strokeType == 0 ? m_strokeSize : size,
                        m_strokeType == 0 ? m_strokeSize : m_preSize);*/
    auto __drawStroke__ = [=]() -> Stroke {
        Stroke theStroke{ point.position,
                          m_prePos,
                          m_strokeType,
                          m_strokeColor,
                          m_strokeType == 0 ? m_strokeSize : size,
                          m_strokeType == 0 ? m_strokeSize : m_preSize };

        drawStroke(theStroke);
        if (signal && theStroke.type != -1) emit writeUpdated(theStroke, point);

        if (!m_penDownStrokeDrawed) m_penDownStrokeDrawed = true;

        return theStroke;
    };
    Stroke stroke{ point.position,
                   m_prePos,
                   m_strokeType,
                   m_strokeColor,
                   m_strokeType == 0 ? m_strokeSize : size,
                   m_strokeType == 0 ? m_strokeSize : m_preSize };

    qreal dist = QLineF(m_prePos, point.position).length();
    if (qMax(m_preSize, size) / 2 > dist) {
        /*if (!m_penDownStrokeDrawed) {
            qreal pre_per = m_preSize / (m_preSize + size);

            m_preSize = dist * pre_per;
            size = dist * (1 - pre_per);

            Stroke theStroke = __drawStroke__();

            m_preSize = size;
            m_prePos = point.position;

            return theStroke;
        } else {
            m_penDownStrokeDrawed = true;
            return Stroke();
        }*/
        return Stroke();
    } else {
        Stroke theStroke = __drawStroke__();
        // drawStroke(strokeVariant);

        // emit strokeUpdated(strokeVariant);
        // m_settings->setPressureSupport(0);
        m_preSize = size;
        m_prePos = point.position;

        return theStroke;
    }
}

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
        };

        qreal velocityScale = f(velocity, scaledVelocityThreshold(), 0);

        qreal minSize = m_minPenSize;
        qreal sizeR = m_strokeSize - minSize;

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
HWWriter::replayStrokesWrite(const QVariantList& strokePoints) {
    foreach (const QVariant& vPoint, strokePoints) {
        QVariantMap vmPoint = vPoint.toMap();

        QVariantMap vmPosition = vmPoint["position"].toMap();

        StrokePoint point{ vmPoint["event"].toInt(),
                           QPointF(vmPosition["x"].toReal(),
                                   vmPosition["y"].toReal()),
                           vmPoint["velocity"].toFloat() };

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
    HWCanvas::mousePressEvent(event);

    if (!m_readOnly && event->button() == Qt::LeftButton &&
        event->modifiers() == Qt::NoModifier) {
        m_writing = true;
        emit writingChanged();

        StrokePoint point{ 0, event->point(0) };
        penDown(point);
    } else
        event->ignore();

    emit pointPressed(event->position());
}

void
HWWriter::mouseMoveEvent(QMouseEvent* event) {
    HWCanvas::mouseMoveEvent(event);

    if (!m_readOnly) {
        m_writing = true;
        emit writingChanged();

        StrokePoint point{ 1, event->point(0) };
        penSegment(point);
    } else
        m_prePos = event->position();

    emit pointMove(event->position());
}

void
HWWriter::mouseReleaseEvent(QMouseEvent* event) {
    HWCanvas::mouseReleaseEvent(event);

    m_writing = false;
    emit writingChanged();

    if (event->button() == Qt::LeftButton) emit writeEnd();

    emit pointRelease(event->position());
}

void
HWWriter::touchEvent(QTouchEvent* event) {
    HWCanvas::touchEvent(event);

    QEventPoint p0 = event->point(0);
    QPointF ps0 = p0.position();
    if (event->touchPointStates() == Qt::TouchPointReleased) {
        if (m_pressTimeStamp > 0 &&
            event->timestamp() - m_pressTimeStamp > 500) {
            event->ignore();
            emit writeIgnore();
        } else if (!m_keepTouchIgnore) {
            if (ps0 == p0.pressPosition())
                emit clicked(ps0);
            else if (QLineF(ps0, p0.pressPosition()).length() > 0.5)
                emit writeEnd();
        }

        m_writing = false;
        emit writingChanged();

        m_keepTouchIgnore = false;
        // m_holdState = 0;
        m_preSize = -1;

        emit pointRelease(ps0);
    } else if (event->pointCount() > 1) {
        m_keepTouchIgnore = true;
        event->ignore();
    } else if (m_keepTouchIgnore)
        event->ignore();
    else if (!m_pressAndHold) {
        StrokePoint point{ 0, p0 };
        QPointF pos = point.position;

        m_writing = true;
        emit writingChanged();

        switch (event->touchPointStates()) {
        case Qt::TouchPointPressed:
            if (!m_readOnly) {
                m_pressTimeStamp = event->timestamp();

                point.event = 0;
                penDown(point);
            }
            break;
        case Qt::TouchPointMoved:
            qreal dist = QLineF(m_prePos, pos).length();
            if (!m_readOnly) {
                m_pressTimeStamp = 0;
                point.event = 1;
                penSegment(point);
            }
            break;
        }
    }
}
