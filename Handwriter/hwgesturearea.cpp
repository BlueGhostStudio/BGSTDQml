#include "hwgesturearea.h"

#include <QPointerEvent>
#include <QQmlEngine>

#include "hwsettings.h"
#include "hwviewport.h"

HWGestureArea::HWGestureArea(QQuickItem* parent)
    : QQuickItem(parent), m_viewport(qobject_cast<HWViewport*>(parent)) {
    setFlag(ItemHasContents, true);
    setWidth(m_viewport->width());
    setHeight(m_viewport->height());
}

void
HWGestureArea::classBegin() {
    QQuickItem::classBegin();

    HWSettings* settings = qmlEngine(m_viewport)
                               ->singletonInstance<HWSettings*>(
                                   "BGStudio.Handwriter", "HWSettings");

    if (settings->isTouchScreen())
        setAcceptTouchEvents(true);
    else
        setAcceptedMouseButtons(Qt::AllButtons);

    QObject::connect(m_viewport, &QQuickItem::widthChanged, this,
                     [=]() { setWidth(m_viewport->width()); });
    QObject::connect(m_viewport, &QQuickItem::heightChanged, this,
                     [=]() { setHeight(m_viewport->height()); });
}

bool
HWGestureArea::isGestureInProgress() const {
    return m_isGestureInProgress;
}

void
HWGestureArea::mousePressEvent(QMouseEvent* event) {
    m_pressPos = m_lastPos = event->position();
    if (event->modifiers() & Qt::ShiftModifier)
        m_lastZoom = m_zoom;
    else if (!(event->modifiers() & Qt::ControlModifier))
        event->ignore();
}

void
HWGestureArea::mouseMoveEvent(QMouseEvent* event) {
    m_isGestureInProgress = true;
    emit isGestureInProgressChanged();
    if (event->modifiers() & Qt::ShiftModifier) {
        qreal dist = event->position().x() - m_pressPos.x();
        m_zoom = (m_lastZoom + (dist >= 0 ? 0 : m_viewport->zoomFactor())) +
                 qMin(dist / 200, 1.00) * 100;
        m_zoom = qFloor(m_zoom / m_viewport->zoomFactor()) *
                 m_viewport->zoomFactor();
        m_zoom = qMin(qMax(m_zoom, 1.00), m_viewport->maxZoomRatio());

        m_viewport->zoom(m_zoom, m_pressPos.x(), m_pressPos.y());
    } else if (event->modifiers() & Qt::ControlModifier) {
        QPointF panPos = event->position() - m_lastPos;
        m_viewport->pan(panPos.x(), panPos.y());
    }

    m_lastPos = event->position();
}

void
HWGestureArea::mouseReleaseEvent(QMouseEvent* /*event*/) {
    m_isGestureInProgress = false;
    emit isGestureInProgressChanged();
}

void
HWGestureArea::touchEvent(QTouchEvent* event) {
    QEventPoint p0 = event->point(0);
    QPointF ps0 = p0.position();
    event->addPassiveGrabber(p0, m_viewport->canvas());
    event->addPassiveGrabber(p0, this);

    if (event->pointCount() >= 2) {
        QPointF ps1 = event->point(1).position();

        QPointF curPos = ps0 + (ps1 - ps0) / 2;
        qreal curPinchDist = QLineF(ps0, ps1).length();

        switch (event->touchPointStates()) {
        case Qt::TouchPointPressed | Qt::TouchPointStationary:
            m_lastPos = curPos;
            m_lastZoom = m_zoom;
            m_pressPinchDist = curPinchDist;

            break;
        case Qt::TouchPointMoved:
            m_isGestureInProgress = true;
            emit isGestureInProgressChanged();
            m_zoom = (curPinchDist - m_pressPinchDist) / 50 + m_lastZoom;
            // m_zoom = curPinchDist / m_pressPinchDist * m_lastZoom;
            m_zoom = qFloor(m_zoom / m_viewport->zoomFactor()) *
                     m_viewport->zoomFactor();
            m_zoom = qMin(qMax(m_zoom, 1.00), m_viewport->maxZoomRatio());

            {
                m_viewport->zoom(m_zoom, curPos.x(), curPos.y());

                QPointF offset = curPos - m_lastPos;
                m_viewport->pan(offset.x(), offset.y());
            }

            m_lastPos = curPos;
            break;
        default:
            m_isGestureInProgress = false;
            emit isGestureInProgressChanged();
        }
    } else
        event->ignore();
}
