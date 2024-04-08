#include "hwcanvasbase.h"

#include <QMetaObject>
#include <QPainter>

HWCanvasBase::HWCanvasBase() {}

/*qreal
HWCanvasBase::dpi() const {
    return m_dpi;
}

void
HWCanvasBase::setDpi(qreal dots) {
    if (m_dpi != dots) {
        m_dpi = dots;
        QMetaObject::invokeMethod(toQObject(), "dpiChanged");
        QMetaObject::invokeMethod(toQObject(), "dpMmChanged");
    }
}

qreal
HWCanvasBase::dpMm() const {
    return m_dpi * 0.0393701;
}

void
HWCanvasBase::setDpMm(qreal dots) {
    setDpi(dots / 0.0393701);
}*/

bool
HWCanvasBase::darkPalette() const {
    return m_darkPalette;
}

void
HWCanvasBase::setDarkPalette(bool dark) {
    if (m_darkPalette != dark) {
        m_darkPalette = dark;
        QMetaObject::invokeMethod(toQObject(), "darkPaletteChanged");
    }
}

QRectF
HWCanvasBase::range() const {
    return m_range;
}

void
HWCanvasBase::resetRange() {
    m_range = QRectF();
    QMetaObject::invokeMethod(toQObject(), "rangeChanged");
}

void
HWCanvasBase::updateCanvasSize(const QSize& size) {
    QQuickItem* qitCanvas = qobject_cast<QQuickItem*>(toQObject());
    if (qitCanvas) qitCanvas->setSize(size);
}

void
HWCanvasBase::updateRange(const Stroke& stroke) {
    qreal maxStrokeSizeRadius = qMax(stroke.preSize, stroke.size) / 2;
    if (m_range.isNull()) {
        m_range.setLeft(qMin(stroke.prePos.x(), stroke.pos.x()) -
                        maxStrokeSizeRadius);
        m_range.setTop(qMin(stroke.prePos.y(), stroke.pos.y()) -
                       maxStrokeSizeRadius);
        m_range.setRight(qMax(stroke.prePos.x(), stroke.pos.x()) -
                         maxStrokeSizeRadius);
        m_range.setBottom(qMax(stroke.prePos.y(), stroke.pos.y()) -
                          maxStrokeSizeRadius);
    } else {
        m_range.setLeft(qMin<qreal>(
            m_range.left(),
            qMin(stroke.prePos.x(), stroke.pos.x()) - maxStrokeSizeRadius));
        m_range.setTop(qMin<qreal>(
            m_range.top(),
            qMin(stroke.prePos.y(), stroke.pos.y()) - maxStrokeSizeRadius));
        m_range.setRight(qMax<qreal>(
            m_range.right(),
            qMax(stroke.prePos.x(), stroke.pos.x()) + maxStrokeSizeRadius));
        m_range.setBottom(qMax<qreal>(
            m_range.bottom(),
            qMax(stroke.prePos.y(), stroke.pos.y()) + maxStrokeSizeRadius));
    }

    QMetaObject::invokeMethod(toQObject(), "rangeChanged");
}
