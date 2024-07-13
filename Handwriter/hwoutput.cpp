#include "hwoutput.h"

#include <QGuiApplication>
#include <QPainterPath>

HWOutput::HWOutput() {}

QUrl
HWOutput::background() const {
    return m_background;
}

bool
HWOutput::setBackground(const QUrl& background) {
    if (m_background != background) {
        m_background = background;

        if (m_background.isEmpty())
            m_backgroundImage = QImage();
        else if (m_background.scheme() == "qrc")
            m_backgroundImage.load(":" + m_background.path());
        else
            m_backgroundImage.load(m_background.path());

        return true;
    } else
        return false;
}

QColor
HWOutput::color() const {
    return m_color;
}

bool
HWOutput::setColor(const QColor& color) {
    if (m_color != color) {
        m_color = color;
        return true;
    } else
        return false;
}

QList<QColor>
HWOutput::palette() const {
    return m_palette;
}

void
HWOutput::setPalette(const QList<QColor>& palette) {
    m_palette = palette;
}

bool
HWOutput::mono() const {
    return m_mono;
}

bool
HWOutput::setMono(bool mono) {
    if (m_mono != mono) {
        m_mono = mono;
        return true;
    } else
        return false;
}

bool
HWOutput::outline() const {
    return m_outline;
}

bool
HWOutput::setOutline(bool outline) {
    if (m_outline != outline) {
        m_outline = outline;
        return true;
    } else
        return false;
}

void
HWOutput::paint(const QList<Stroke>& strokeList) {
    m_renderPainter.begin(&m_contentPicture);
    m_renderPainter.eraseRect(m_contentPicture.boundingRect());
    QList<Stroke>::const_iterator it;

    for (it = strokeList.constBegin(); it < strokeList.constEnd(); ++it) {
        if (m_outline) {
            if (!m_mono && !m_palette.isEmpty() &&
                it->color < m_palette.length())
                m_renderPainter.setPen(m_palette[it->color]);
            else
                m_renderPainter.setPen(QColor(Qt::black));

            m_renderPainter.drawLine(it->prePos, it->pos);
        } else {
            m_renderPainter.setPen(Qt::NoPen);

            if (!m_mono && !m_palette.isEmpty() &&
                it->color < m_palette.length())
                m_renderPainter.setBrush(m_palette[it->color]);
            else
                m_renderPainter.setBrush(QColor(Qt::black));

            drawStroke(*it);
        }
    }
    m_renderPainter.end();
}

void
HWOutput::output(QPainter* painter, const QSizeF& sourceSize,
                 const QPointF& targetPos, qreal devicePixelRatio) {
    painter->eraseRect(0, 0, sourceSize.width(), sourceSize.height());

    painter->setWindow(
        QRect(targetPos.toPoint(), sourceSize.toSize() * devicePixelRatio));

    if (!m_backgroundImage.isNull())
        painter->drawImage(QRectF(QPointF(0, 0), sourceSize),
                           m_backgroundImage);
    else if (m_color.isValid()){
        QPen pen(QColor(Qt::gray), 1);
        painter->setPen(pen);
        painter->setBrush(m_color);
        painter->drawRect(0, 0, sourceSize.width() - 1,
                          sourceSize.height() - 1);
    }

    painter->drawPicture(0, 0, m_contentPicture);
}

void
HWOutput::render(qreal radius, const QPointF& pos) {
    m_renderPainter.drawEllipse(pos, radius, radius);
}

void
HWOutput::render(qreal prePosRadius, qreal posRadius, const QPointF& prePos,
                 const QPointF& pos, qreal prePosStartAngle,
                 qreal prePosEndAngle, qreal posStartAngle, qreal posEndAngle) {
    QPainterPath strokePath;

    QRectF arcRect(prePos - QPointF(prePosRadius, prePosRadius),
                   QSizeF(prePosRadius * 2, prePosRadius * 2));
    strokePath.arcMoveTo(arcRect, qRadiansToDegrees(prePosStartAngle));
    strokePath.arcTo(arcRect, qRadiansToDegrees(prePosStartAngle),
                     qRadiansToDegrees(prePosEndAngle - prePosStartAngle));

    arcRect = { pos - QPointF(posRadius, posRadius),
                QSizeF(posRadius * 2, posRadius * 2) };
    // strokePath.arcMoveTo(arcRect, qRadiansToDegrees(posStartAngle));
    strokePath.arcTo(arcRect, qRadiansToDegrees(posStartAngle),
                     360 + qRadiansToDegrees(posEndAngle - posStartAngle));
    strokePath.closeSubpath();

    m_renderPainter.drawPath(strokePath);
}
