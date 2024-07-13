#include "hwgrabpreview.h"

#include <QPainter>
#include <QQuickWindow>

HWGrabPreview::HWGrabPreview() { setAntialiasing(true); }

void
HWGrabPreview::paint(QPainter* painter) {
    output(painter, size(), QPointF(0, 0),
           window() ? window()->devicePixelRatio() : 1);
}

HWCanvas*
HWGrabPreview::canvas() const {
    return m_canvas;
}

void
HWGrabPreview::setCanvas(HWCanvas* canvas, bool set) {
    if (m_canvas != canvas && (!m_paper || !set)) {
        m_canvas = canvas;
        setPalette(m_canvas->canvasPalette());
        emit canvasChanged();
    }
}

HWPaper*
HWGrabPreview::paper() const {
    return m_paper;
}

void
HWGrabPreview::setPaper(HWPaper* paper) {
    if (m_paper != paper) {
        if (m_paper) m_paper->disconnect(this);

        m_paper = paper;

        QObject::connect(m_paper, &HWPaper::canvasChanged, this, [=]() {
            setCanvas(m_paper->canvas(), false);
        });
        QObject::connect(m_paper, &HWPaper::paperSizeChanged, this, [=]() {
            setImplicitSize(m_paper->paperSize().width(),
                            m_paper->paperSize().height());
            setSize(m_paper->paperSize());
        });
        QObject::connect(m_paper, &HWPaper::backgroundChanged, this, [=]() {
            setBackground(m_paper->background());
        });

        setCanvas(m_paper->canvas(), false);

        setImplicitSize(m_paper->paperSize().width(),
                        m_paper->paperSize().height());
        setSize(m_paper->paperSize());
        setBackground(m_paper->background());

        emit paperChanged();
    }
}

HWCVType::ContentVisible
HWGrabPreview::grabRange() const {
    return m_grabRange;
}

void
HWGrabPreview::setGrabRange(HWCVType::ContentVisible grabRange) {
    if (m_grabRange == grabRange) return;
    m_grabRange = grabRange;
    emit grabRangeChanged();
}

/*QUrl
HWGrabPreview::background() const {
    return m_background;
}*/

void
HWGrabPreview::setBackground(const QUrl& background) {
    if (HWOutput::setBackground(background)) {
        emit backgroundChanged();
        update();
    }
}

void
HWGrabPreview::setColor(const QColor& color) {
    if (HWOutput::setColor(color)) emit colorChanged();
}

void
HWGrabPreview::setMono(bool mono) {
    if (HWOutput::setMono(mono)) emit monoChanged();
}

void
HWGrabPreview::setOutline(bool outline) {
    if (HWOutput::setOutline(outline)) emit outlineChanged();
}

void
HWGrabPreview::grab() {
    if (m_canvas) {
        HWOutput::paint(m_canvas->strokes(m_grabRange));
        update();
    }
}
