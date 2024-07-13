#include "hwviewport.h"

#include <hwsettings.h>

#include <QQmlComponent>

#include "hwgesturearea.h"

HWViewport::HWViewport(QQuickItem* parent) : QQuickItem(parent) {
    setFlag(QQuickItem::ItemIsViewport, true);
}

HWViewportAttachedType*
HWViewport::qmlAttachedProperties(QObject* obj) {
    return new HWViewportAttachedType(obj);
}

void
HWViewport::classBegin() {
    QQuickItem::classBegin();

    m_qmlEngine = qmlEngine(this);
    m_settings = m_qmlEngine->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");

    if (m_settings->isTouchScreen())
        setAcceptTouchEvents(true);
    else
        setAcceptedMouseButtons(Qt::AllButtons);

    m_context = new QQmlContext(m_qmlEngine, this);
    m_context->setContextProperty("viewport", this);

    /*QQmlComponent cmpPaper(m_qmlEngine);
    cmpPaper.setData(R"QML(
import QtQuick
import BGStudio.Handwriter

HWPaper {
    id: paper

    parent: viewport
    canvas: viewport.canvas
    transformOrigin: Item.TopLeft

    Behavior on x {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: canvas
                property: "enabled"
                value: true
            }
        }
    }
    Behavior on y {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: canvas
                property: "enabled"
                value: true
            }
        }
    }
}
)QML",
                     QUrl());

    m_paper = qobject_cast<HWPaper*>(cmpPaper.create(m_context));*/

    /*QQmlComponent cmpPaperAni(m_qmlEngine);
    cmpPaperAni.setData(R"QML(
import QtQuick
import BGStudio.Handwriter

QtObject {
    property HWPaper paper: viewport.paper
    Behavior on paper.x {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: true
            }
        }
    }
    Behavior on paper.y {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: true
            }
        }
    }

    Component.onCompleted: console.log("-------", paper)
}
)QML", QUrl());

    cmpPaperAni.create(m_context);*/

    /*QObject::connect(m_paper, &HWPaper::scaleChanged, this,
                     &HWViewport::zoomedChanged);
    QObject::connect(m_paper, &HWPaper::scaleChanged, this,
                     &HWViewport::zoomRatioChanged);*/

    QQmlComponent indicatorCmp(m_qmlEngine);
    indicatorCmp.setData(R"QML(
import QtQuick
import QtQuick.Controls
Image {
    parent: viewport

    x: viewport.lastPos.x - width / 2
    y: viewport.lastPos.y - height / 2
    width: 120
    height: 120
    z: 1

    visible: false

    source: "qrc:/qt/qml/BGStudio/Handwriter/icons/indicator_move1.png"
}
)QML",
                         QUrl());

    m_singlePointIndicator =
        qobject_cast<QQuickItem*>(indicatorCmp.create(m_context));

    /*QObject::connect(this, &HWViewport::isGestureInProgressChanged, this,
                     [=]() { setPanAniEnabled(!m_isGestureInProgress); });*/

    /*QObject::connect(this, &QQuickItem::widthChanged, this, [=]() {
        if (m_paper) clampPositionX(m_paper->x());
    });
    QObject::connect(this, &QQuickItem::heightChanged, this, [=]() {
        if (m_paper) clampPositionY(m_paper->y());
    });*/
    /*QObject::connect(m_paper, &HWPaper::widthChanged, this,
                     [=]() { clampPositionX(m_paper->x()); });
    QObject::connect(m_paper, &HWPaper::heightChanged, this,
                     [=]() { clampPositionY(m_paper->y()); });*/
}

/*void
HWViewport::componentComplete() {
    QQuickItem::componentComplete();
    if (m_singlePointEnable)
        enableSinglePoint();
}*/

QQuickItem* HWViewport::paper() const {
    return m_paper;
}

void
HWViewport::setPaper(QQuickItem* paper) {
    if (m_paper != paper) {
        if (m_paper) {
            QObject::disconnect(m_paper);
            m_paper->setParentItem(nullptr);
            disableSinglePoint();
            /*if (m_paperAni)
                m_paperAni->deleteLater();*/
        }

        m_paper = paper;
        if (m_paper) {
            m_paper->setParentItem(this);
            m_paper->setZ(0);
            m_paper->setTransformOrigin(QQuickItem::TopLeft);
            QObject::connect(m_paper, &HWPaper::widthChanged, this,
                             [=]() { clampPositionX(m_paper->x()); });
            QObject::connect(m_paper, &HWPaper::heightChanged, this,
                             [=]() { clampPositionY(m_paper->y()); });
            QObject::connect(m_paper, &HWPaper::scaleChanged, this,
                             &HWViewport::zoomedChanged);
            QObject::connect(m_paper, &HWPaper::scaleChanged, this,
                             &HWViewport::zoomRatioChanged);
            QObject::connect(this, &QQuickItem::widthChanged, this, [=]() {
                if (m_paper) clampPositionX(m_paper->x());
            });
            QObject::connect(this, &QQuickItem::heightChanged, this, [=]() {
                if (m_paper) clampPositionY(m_paper->y());
            });
            m_zoom = m_paper->scale();
            emit zoomRatioChanged();

            clampPosition(m_paper->x(), m_paper->y());

            if (m_singlePointEnable) enableSinglePoint();

            /*m_context->setContextProperty("currentPaper", m_paper);
            QQmlComponent cmpPaperAni(m_qmlEngine);
            cmpPaperAni.setData(R"QML(
import QtQuick
import BGStudio.Handwriter

QtObject {
    property HWPaper paper: currentPaper
    Behavior on paper.x {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: true
            }
        }
    }
    Behavior on paper.y {
        enabled: viewport.panAniEnabled
        SequentialAnimation {
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: false
            }
            NumberAnimation { duration: 250 }
            PropertyAction {
                target: paper.canvas
                property: "enabled"
                value: true
            }
        }
    }
}
)QML", QUrl());*/

            // m_paperAni = cmpPaperAni.create(m_context);
        }

        emit paperChanged();
    }
}

/*HWCanvas*
HWViewport::canvas() const {
    return m_canvas;
}*/

/*void
HWViewport::setCanvas(HWCanvas* canvas) {
    if (m_canvas != canvas) {
        if (m_canvas) m_canvas->disconnect(this);

        m_canvas = canvas;

        QObject::connect(
            m_canvas, &HWCanvas::clicked, this,
            [=](const QPointF& pos) { qDebug() << "clicked" << pos; });

        if (m_singlePointEnable) enableSinglePoint();
        emit canvasChanged();
    }
}*/

bool
HWViewport::isGestureInProgress() const {
    return m_isGestureInProgress;
}

void
HWViewport::setGestureInProgress(bool enable) {
    if (m_isGestureInProgress != enable) {
        m_isGestureInProgress = enable;
        m_panAniEnabled = !enable;
        emit isGestureInProgressChanged();
        emit panAniEnabledChanged();
    }
}

/*int
HWViewport::singlePointState() const {
    return m_singlePointState;
}*/

bool
HWViewport::singlePointEnable() const {
    return m_singlePointEnable;
}

void
HWViewport::setSinglePointEnable(bool singlePointEnable) {
    if (m_singlePointEnable != singlePointEnable) {
        m_singlePointEnable = singlePointEnable;

        if (m_singlePointEnable)
            enableSinglePoint();
        else
            disableSinglePoint();

        emit singlePointEnableChanged();
    }
}

QPointF
HWViewport::lastPos() const {
    return m_lastPos;
}

void
HWViewport::setLastPos(const QPointF& pos) {
    if (m_lastPos != pos) {
        m_lastPos = pos;
        emit lastPosChanged();
    }
}

bool
HWViewport::panAniEnabled() const {
    return m_panAniEnabled;
}

void
HWViewport::setPanAniEnabled(bool panAniEnabled) {
    if (m_panAniEnabled != panAniEnabled) {
        m_panAniEnabled = panAniEnabled;
        emit panAniEnabledChanged();
    }
}

void
HWViewport::pan(qreal dx, qreal dy) {
    if (m_paper) clampPosition(m_paper->x() + dx, m_paper->y() + dy);
}

void
HWViewport::panTo(qreal dx, qreal dy) {
    if (m_paper) clampPosition(-dx * m_paper->scale(), -dy * m_paper->scale());
}

void
HWViewport::panToX(qreal dx, qreal margin) {
    if (m_paper) clampPositionX(-dx * m_paper->scale() + margin);
}

void
HWViewport::panToY(qreal dy, qreal margin) {
    if (m_paper) clampPositionY(-dy * m_paper->scale() + margin);
}

void
HWViewport::panLeft(qreal dx, qreal margin) {
    if (m_paper) clampPositionX(m_paper->x() - dx * m_paper->scale() + margin);
}

void
HWViewport::zoom(qreal zoom, qreal zoomCenterX, qreal zoomCenterY,
                 bool viewportCenter) {
    // qreal newScale = qMax(zoom * 100, 1.00);
    if (m_paper && zoom != m_zoom) {
        /* QPointF paperPos = m_paper->mapFromItem(this, zoomCenterX,
        zoomCenterY);

        qDebug() << zoomCenterX << zoomCenterY << m_paper->scale() << "paperPos"
                 << paperPos;*/
        if (viewportCenter) {
            setGestureInProgress(true);
            QPointF paperPos =
                m_paper->mapFromItem(this, zoomCenterX, zoomCenterY);
            m_paper->setScale(zoom);

            QPointF vpPos = mapFromItem(m_paper, paperPos);
            QPointF vpPaperPos = m_paper->position() +
                            QPointF(width() / 2, height() / 2) -
                            vpPos;

            clampPosition(vpPaperPos.x(), vpPaperPos.y());
            setGestureInProgress(false);

            vpPos = mapFromItem(m_paper, paperPos);
            setLastPos(vpPos);
        } else {
            qreal scaleFactor = zoom / m_paper->scale();
            m_paper->setScale(zoom);

            /*qDebug() << "scaled viewport center"
                     << m_paper->position() - (mapFromItem(m_paper, paperPos) -
               paperPos);*/

            qreal x = m_paper->x() -
                      (zoomCenterX - m_paper->x()) * scaleFactor + zoomCenterX -
                      m_paper->x();
            qreal y = m_paper->y() -
                      (zoomCenterY - m_paper->y()) * scaleFactor + zoomCenterY -
                      m_paper->y();

            clampPosition(x, y);
        }

        m_zoom = zoom;
    }
}

void
HWViewport::clampPositionX(qreal x) {
    if (!m_paper) return;

    qreal scaledWidth = m_paper->width() * m_paper->scale();
    if (scaledWidth < width())
        m_paper->setProperty("x", (width() - scaledWidth) / 2);
    else
        m_paper->setProperty("x", qMax(qMin(x, 0.00), width() - scaledWidth));
}

void
HWViewport::clampPositionY(qreal y) {
    if (!m_paper) return;

    qreal scaledHeight = m_paper->height() * m_paper->scale();
    if (scaledHeight < height())
        m_paper->setProperty("y", (height() - scaledHeight) / 2);
    else
        m_paper->setProperty("y", qMax(qMin(y, 0.00), height() - scaledHeight));
}

void
HWViewport::clampPosition(qreal x, qreal y) {
    clampPositionX(x);
    clampPositionY(y);
}

void
HWViewport::enableSinglePoint() {
    HWPaper* thePaper = qobject_cast<HWPaper*>(m_paper);
    if (thePaper) {
        HWCanvas* canvas = thePaper->canvas();
        if (canvas) {
            QObject::connect(canvas, &HWCanvas::pressAndHold, this,
                             [=](const QPointF& pos) {
                                 m_singlePointIndicator->setVisible(true);
                                 /*m_lastPos = mapFromItem(m_canvas, pos);
                                 emit lastPosChanged();*/
                                 qDebug() << ">>>>>" << pos << mapFromItem(canvas, pos);
                                 setLastPos(mapFromItem(canvas, pos));
                             });
            QObject::connect(canvas, &HWCanvas::holdAndMove, this,
                             [=](const QPointF& pos) {
                                 m_singlePointIndicator->setVisible(false);
                                 setGestureInProgress(true);
                                 QPointF vpPos = mapFromItem(canvas, pos);
                                 QPointF offset = vpPos - m_lastPos;
                                 pan(offset.x(), offset.y());

                                 setLastPos(vpPos);
                             });
            QObject::connect(canvas, &HWCanvas::pointRelease, this,
                             [=](const QPointF&) {
                                 m_singlePointIndicator->setVisible(false);
                                 setGestureInProgress(false);
                             });
        }
    }
}

void
HWViewport::disableSinglePoint() {
    // QObject::disconnect(m_canvas, &HWCanvas::holdState, this, nullptr);
    HWPaper* thePaper = qobject_cast<HWPaper*>(m_paper);
    if (thePaper) {
        HWCanvas* canvas = thePaper->canvas();
        if (canvas) {
            QObject::disconnect(canvas, &HWCanvas::holdAndMove, this,
                                nullptr);
            QObject::disconnect(canvas, &HWCanvas::pointRelease, this,
                                nullptr);
            QObject::disconnect(canvas, &HWCanvas::pointPressed, this,
                                nullptr);
        }
    }
}

void
HWViewport::mousePressEvent(QMouseEvent* event) {
    m_pressPos = m_lastPos = event->position();
    emit lastPosChanged();
    if (event->modifiers() & Qt::ShiftModifier)
        m_lastZoom = m_zoom;
    else if (!(event->modifiers() & Qt::ControlModifier))
        event->ignore();
}

void
HWViewport::mouseMoveEvent(QMouseEvent* event) {
    /*m_isGestureInProgress = true;
    emit isGestureInProgressChanged();*/
    setGestureInProgress(true);

    if (event->modifiers() & Qt::ShiftModifier) {
        qreal dist = event->position().x() - m_pressPos.x();
        m_zoom = (m_lastZoom + (dist >= 0 ? 0 : m_zoomFactor)) +
                 qMin(dist / 200, 1.00) * 100;
        m_zoom = qFloor(m_zoom / m_zoomFactor) * m_zoomFactor;
        m_zoom = qMin(qMax(m_zoom, 1.00), m_maxZoomRatio);

        zoom(m_zoom, m_pressPos.x(), m_pressPos.y());
    } else if (event->modifiers() & Qt::ControlModifier) {
        QPointF panPos = event->position() - m_lastPos;
        pan(panPos.x(), panPos.y());
    }

    /*m_lastPos = event->position();
    emit lastPosChanged();*/
    setLastPos(event->position());
}

void
HWViewport::mouseReleaseEvent(QMouseEvent*) {
    /*m_isGestureInProgress = false;
    emit isGestureInProgressChanged();*/
    setGestureInProgress(false);
}

void
HWViewport::touchEvent(QTouchEvent* event) {
    QEventPoint p0 = event->point(0);
    QPointF ps0 = p0.position();

    event->addPassiveGrabber(p0, this);

    if (event->pointCount() >= 2) {
        QPointF ps1 = event->point(1).position();

        QPointF curPos = ps0 + (ps1 - ps0) / 2;
        qreal curPinchDist = QLineF(ps0, ps1).length();

        switch (event->touchPointStates()) {
        case Qt::TouchPointPressed | Qt::TouchPointStationary:
            /*m_lastPos = curPos;
            emit lastPosChanged();*/
            setLastPos(curPos);
            m_lastZoom = m_zoom;
            m_pressPinchDist = curPinchDist;

            break;
        case Qt::TouchPointMoved:
            /*m_isGestureInProgress = true;
            emit isGestureInProgressChanged();*/
            setGestureInProgress(true);
            {
                qreal qrZoom =
                    (curPinchDist - m_pressPinchDist) / 50 + m_lastZoom;
                // m_zoom = curPinchDist / m_pressPinchDist * m_lastZoom;
                qrZoom = qFloor(qrZoom / m_zoomFactor) * m_zoomFactor;
                qrZoom = qMin(qMax(qrZoom, 1.00), m_maxZoomRatio);
                zoom(qrZoom, curPos.x(), curPos.y());

                QPointF offset = curPos - m_lastPos;
                pan(offset.x(), offset.y());
            }

            /*m_lastPos = curPos;
            emit lastPosChanged();*/
            setLastPos(curPos);
            break;
        default:
            setGestureInProgress(false);
            /*m_isGestureInProgress = false;
            emit isGestureInProgressChanged();*/
        }
    }
}

bool
HWViewport::zoomed() const {
    return m_paper ? m_paper->scale() > 1 : false;
}

qreal
HWViewport::zoomRatio() const {
    return m_paper ? m_paper->scale() : 1;
}

qreal
HWViewport::zoomFactor() const {
    return m_zoomFactor;
}

void
HWViewport::setZoomFactor(qreal zoomFactor) {
    if (qFuzzyCompare(zoomFactor, m_zoomFactor)) return;
    m_zoomFactor = zoomFactor;
    emit zoomFactorChanged();
}

qreal
HWViewport::maxZoomRatio() const {
    return m_maxZoomRatio;
}

void
HWViewport::setMaxZoomRatio(qreal maxZoomRatio) {
    if (qFuzzyCompare(m_maxZoomRatio, maxZoomRatio)) return;
    m_maxZoomRatio = maxZoomRatio;
    emit maxZoomRatioChanged();
}
