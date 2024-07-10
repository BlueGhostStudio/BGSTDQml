#include "hwviewport.h"

#include <QQmlComponent>

#include "hwgesturearea.h"

HWViewport::HWViewport(QQuickItem* parent) : QQuickItem(parent) {
    setFlag(QQuickItem::ItemIsViewport, true);
    /*m_qmlEngine = qmlEngine(this);

    m_context = new QQmlContext(m_qmlEngine, this);
    m_context->setContextProperty("viewport", this);

    QQmlComponent cmpPaper(m_qmlEngine);
    cmpPaper.setData(R"QML(
import QtQuick
import BGStudio.Handwriter

HWPaper {
    id: paper

    parent: viewport
    canvas: viewport.canvas
    transformOrigin: Item.TopLeft

    Behavior on x {
        enabled: !gestureArea.isGestureInProgress
        NumberAnimation { duration: 250 }
    }
    Behavior on y {
        enabled: !gestureArea.isGestureInProgress
        NumberAnimation { duration: 250 }
    }
}
)QML",
                     QUrl());

    m_paper = qobject_cast<HWPaper*>(cmpPaper.create(m_context));

    QQmlComponent cmpGestureArea(m_qmlEngine);
    cmpGestureArea.setData(R"QML(
import QtQuick
import BGStudio.Handwriter
import "Private" as P

P.GestureArea {
    id: gestureArea

    parent: viewport
    anchors.fill: parent
}
)QML",
                           QUrl());

    m_gestureArea = qobject_cast<QQuickItem*>(cmpGestureArea.create(m_context));

    QObject::connect(this, &QQuickItem::widthChanged, this, [=]() {
        if (m_paper)
            clampPositionX(m_paper->x());
    });
    QObject::connect(this, &QQuickItem::heightChanged, this, [=]() {
        if (m_paper)
            clampPositionY(m_paper->y());
    });*/
}

HWViewportAttachedType*
HWViewport::qmlAttachedProperties(QObject* obj) {
    return new HWViewportAttachedType(obj);
}

void
HWViewport::classBegin() {
    QQuickItem::classBegin();

    m_qmlEngine = qmlEngine(this);
    m_context = new QQmlContext(m_qmlEngine, this);
    m_context->setContextProperty("viewport", this);

    QQmlComponent cmpPaper(m_qmlEngine);
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

    m_paper = qobject_cast<HWPaper*>(cmpPaper.create(m_context));

    QObject::connect(m_paper, &HWPaper::scaleChanged, this,
                     &HWViewport::zoomedChanged);

    /*QQmlComponent cmpGestureArea(m_qmlEngine);
    cmpGestureArea.setData(R"QML(
import QtQuick
import "qrc:/qt/qml/BGStudio/Handwriter/Private" as P

P.GestureArea {
    id: gestureArea

    parent: viewport
    anchors.fill: parent
}
)QML",
                           QUrl());

    m_gestureArea =
qobject_cast<QQuickItem*>(cmpGestureArea.create(m_context));*/
    m_gestureArea = new HWGestureArea(this);
    m_gestureArea->classBegin();
    QObject::connect(
        m_gestureArea, &HWGestureArea::isGestureInProgressChanged, this,
        [=]() { setPanAniEnabled(!m_gestureArea->isGestureInProgress()); });
    emit gestureAreaChanged();

    QObject::connect(this, &QQuickItem::widthChanged, this, [=]() {
        if (m_paper) clampPositionX(m_paper->x());
    });
    QObject::connect(this, &QQuickItem::heightChanged, this, [=]() {
        if (m_paper) clampPositionY(m_paper->y());
    });
    QObject::connect(m_paper, &HWPaper::widthChanged, this,
                     [=]() { clampPositionX(m_paper->x()); });
    QObject::connect(m_paper, &HWPaper::heightChanged, this,
                     [=]() { clampPositionY(m_paper->y()); });
}

HWPaper*
HWViewport::paper() const {
    return m_paper;
}

void
HWViewport::setPaper(HWPaper* paper) {
    if (m_paper != paper) {
        m_paper = paper;
        emit paperChanged();
    }
}

HWCanvas*
HWViewport::canvas() const {
    return m_canvas;
}

void
HWViewport::setCanvas(HWCanvas* canvas) {
    if (m_canvas != canvas) {
        m_canvas = canvas;
        emit canvasChanged();
    }
}

QQuickItem*
HWViewport::gestureArea() const {
    return m_gestureArea;
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
    clampPosition(m_paper->x() + dx, m_paper->y() + dy);
}

void
HWViewport::panTo(qreal dx, qreal dy) {
    clampPosition(-dx * m_paper->scale(), -dy * m_paper->scale());
}

void
HWViewport::panToX(qreal dx, qreal margin) {
    clampPositionX(-dx * m_paper->scale() + margin);
}

void
HWViewport::panToY(qreal dy, qreal margin) {
    clampPositionY(-dy * m_paper->scale() + margin);
}

void
HWViewport::panLeft(qreal dx, qreal margin) {
    clampPositionX(m_paper->x() - dx * m_paper->scale() + margin);
}

void
HWViewport::zoom(qreal zoom, qreal zoomCenterX, qreal zoomCenterY) {
    // qreal newScale = qMax(zoom * 100, 1.00);
    if (zoom != m_paper->scale()) {
        qreal scaleFactor = zoom / m_paper->scale();
        m_paper->setScale(zoom);

        qreal x = m_paper->x() - (zoomCenterX - m_paper->x()) * scaleFactor +
                  zoomCenterX - m_paper->x();
        qreal y = m_paper->y() - (zoomCenterY - m_paper->y()) * scaleFactor +
                  zoomCenterY - m_paper->y();

        clampPosition(x, y);
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

bool
HWViewport::zoomed() const {
    return m_paper ? m_paper->scale() > 1 : false;
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
