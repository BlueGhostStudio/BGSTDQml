#include "bgborderbackground.h"

#include <QPainter>

BGBorderBackground::BGBorderBackground(QQuickItem* parent)
    : QQuickPaintedItem(parent) {}
QColor
BGBorderBackground::color() const {
    return m_color;
}

void
BGBorderBackground::setColor(const QColor& color) {
    m_color = color;
    emit colorChanged();
    update();
}

QColor
BGBorderBackground::background2D() const {
    return m_background2D;
}

void
BGBorderBackground::setBackground2D(const QColor& color) {
    m_background2D = color;
    emit background2DChanged();

    update();
}

BGBorderBackground::Shape
BGBorderBackground::shape() const {
    return m_shape;
}

void
BGBorderBackground::setShape(Shape shape) {
    m_shape = shape;
    emit shapeChanged();
    update();
}

BGBorderBackground::Style
BGBorderBackground::style() const {
    return m_style;
}

void
BGBorderBackground::setStyle(Style style) {
    m_style = style;
    emit styleChanged();
    update();
}

int
BGBorderBackground::cornerRadius() const {
    return m_cornerRadius;
}

void
BGBorderBackground::setCornerRadius(int radius) {
    m_cornerRadius = radius;
    emit cornerRadiusChanged();
    update();
}

bool
BGBorderBackground::border() const {
    return m_border;
}

void
BGBorderBackground::setBorder(bool border) {
    m_border = border;
    emit borderChanged();
    update();
}

qreal
BGBorderBackground::borderWidth() const {
    return m_borderWidth;
}

void
BGBorderBackground::setBorderWidth(qreal width) {
    m_borderWidth = width;
    emit borderWidthChanged();
    update();
}

void
BGBorderBackground::paint(QPainter* painter) {
    painter->setRenderHints(QPainter::Antialiasing);

    switch (m_shape) {
    case Fill:
        fill(painter);
        break;
    case Rectangle:
        drawRectangle(painter);
        break;
    case RoundedRectangle:
        drawRoundedRectangle(painter);
        break;
    case Ellipse:
        drawEllipse(painter);
        break;
    case Lower:
        drawLower(painter);
        break;
    case Upper:
        drawUpper(painter);
        break;
    case LowerV:
        drawLowerV(painter);
        break;
    case UpperV:
        drawUpperV(painter);
        break;
    }
}

QColor
BGBorderBackground::highlightColor() const {
    return m_color.lighter(150);
}

QColor
BGBorderBackground::shadowColor() const {
    return m_color.darker(150);
}

void
BGBorderBackground::fill(QPainter* painter) {
    painter->fillRect(boundingRect(), m_color);
}

void
BGBorderBackground::drawRectangle(QPainter* painter) {
    painter->setPen(Qt::NoPen);
    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawRect(boundingRect() - QMargins(1, 1, 1, 1));
            painter->setBrush(highlightColor());
            painter->drawRect(boundingRect() - QMargins(2, 2, 2, 2));
            painter->setBrush(m_color);
            painter->drawRect(boundingRect() - QMargins(3, 3, 3, 3));
        } else {
            painter->setBrush(m_color);
            painter->drawRect(boundingRect() - QMargins(1, 1, 1, 1));
            if (m_background2D.alpha() == 0)
                painter->setCompositionMode(
                    QPainter::CompositionMode_SourceOut);
            else
                painter->setBrush(m_background2D);
            painter->drawRect(boundingRect() -
                              QMargins(m_borderWidth + 1, m_borderWidth + 1,
                                       m_borderWidth + 1, m_borderWidth + 1));
        }
    } else {
        painter->setBrush(m_color);
        painter->drawRect(boundingRect() - QMargins(1, 1, 1, 1));
    }
}

void
BGBorderBackground::drawRoundedRectangle(QPainter* painter) {
    painter->setPen(Qt::NoPen);
    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawRoundedRect(boundingRect() - QMargins(1, 1, 1, 1),
                                     m_cornerRadius, m_cornerRadius);
            painter->setBrush(highlightColor());
            painter->drawRoundedRect(boundingRect() - QMargins(2, 2, 2, 2),
                                     m_cornerRadius - 1, m_cornerRadius - 1);
            painter->setBrush(m_color);
            painter->drawRoundedRect(boundingRect() - QMargins(3, 3, 3, 3),
                                     m_cornerRadius - 2, m_cornerRadius - 2);
        } else {
            painter->setBrush(m_color);
            painter->drawRoundedRect(boundingRect() - QMargins(1, 1, 1, 1),
                                     m_cornerRadius, m_cornerRadius);
            if (m_background2D.alpha() == 0)
                painter->setCompositionMode(
                    QPainter::CompositionMode_SourceOut);
            else
                painter->setBrush(m_background2D);
            painter->drawRoundedRect(
                boundingRect() - QMargins(m_borderWidth + 1, m_borderWidth + 1,
                                          m_borderWidth + 1, m_borderWidth + 1),
                m_cornerRadius - m_borderWidth, m_cornerRadius - m_borderWidth);
        }
    } else {
        painter->setBrush(m_color);
        painter->drawRoundedRect(boundingRect() - QMargins(1, 1, 1, 1),
                                 m_cornerRadius, m_cornerRadius);
    }
}

void
BGBorderBackground::drawEllipse(QPainter* painter) {
    painter->setPen(Qt::NoPen);
    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawEllipse(boundingRect() - QMargins(1, 1, 1, 1));
            painter->setBrush(highlightColor());
            painter->drawEllipse(boundingRect() - QMargins(2, 2, 2, 2));
            painter->setBrush(m_color);
            painter->drawEllipse(boundingRect() - QMargins(3, 3, 3, 3));
        } else {
            painter->setBrush(m_color);
            painter->drawEllipse(boundingRect() - QMargins(1, 1, 1, 1));
            painter->setCompositionMode(QPainter::CompositionMode_SourceOut);
            painter->drawEllipse(boundingRect() - QMargins(m_borderWidth + 1,
                                                           m_borderWidth + 1,
                                                           m_borderWidth + 1,
                                                           m_borderWidth + 1));
        }
    } else {
        painter->setBrush(m_color);
        painter->drawEllipse(boundingRect() - QMargins(1, 1, 1, 1));
    }
}

void
BGBorderBackground::drawLower(QPainter* painter) {
    qreal halfWidth = width() / 2;

    painter->setPen(Qt::NoPen);

    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(halfWidth, 0),
                90 * 16, 180 * 16);

            painter->setClipRect(0, 0, width() - 1, height());
            painter->setBrush(highlightColor());
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(halfWidth, 0),
                90 * 16, 180 * 16);

            painter->setClipRect(0, 0, width() - 2, height());
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(3, 3, -3, -3).translated(halfWidth, 0),
                90 * 16, 180 * 16);
        } else {
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(halfWidth, 0),
                90 * 16, 180 * 16);

            painter->setCompositionMode(QPainter::CompositionMode_SourceOut);

            painter->setClipRect(0, 0, width() - 1, height());
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(halfWidth, 0),
                90 * 16, 180 * 16);
        }
    } else {
        painter->setBrush(m_color);
        painter->drawChord(
            boundingRect().adjusted(1, 1, -1, -1).translated(halfWidth, 0),
            90 * 16, 180 * 16);
    }
}

void
BGBorderBackground::drawUpper(QPainter* painter) {
    qreal halfWidth = width() / 2;

    painter->setPen(Qt::NoPen);

    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(-halfWidth, 0),
                270 * 16, 180 * 16);

            painter->setClipRect(1, 0, width(), height());
            painter->setBrush(highlightColor());
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(-halfWidth, 0),
                270 * 16, 180 * 16);

            painter->setClipRect(2, 0, width(), height());
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(3, 3, -3, -3).translated(-halfWidth, 0),
                270 * 16, 180 * 16);
        } else {
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(-halfWidth, 0),
                270 * 16, 180 * 16);

            painter->setCompositionMode(QPainter::CompositionMode_SourceOut);

            painter->setClipRect(1, 0, width(), height());
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(-halfWidth, 0),
                270 * 16, 180 * 16);
        }
    } else {
        painter->setBrush(m_color);
        painter->drawChord(
            boundingRect().adjusted(1, 1, -1, -1).translated(-halfWidth, 0),
            270 * 16, 180 * 16);
    }
}

void
BGBorderBackground::drawLowerV(QPainter* painter) {
    qreal halfHeight = height() / 2;

    painter->setPen(Qt::NoPen);
    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawChord(boundingRect()
                                   .adjusted(1, 1, -1, -1)
                                   .translated(0, -halfHeight),
                               180 * 16, 180 * 16);

            painter->setClipRect(0, 1, width(), height());
            painter->setBrush(highlightColor());
            painter->drawChord(boundingRect()
                                   .adjusted(2, 2, -2, -2)
                                   .translated(0, -halfHeight),
                               180 * 16, 180 * 16);

            painter->setClipRect(0, 2, width(), height());
            painter->setBrush(m_color);
            painter->drawChord(boundingRect()
                                   .adjusted(3, 3, -3, -3)
                                   .translated(0, -halfHeight),
                               180 * 16, 180 * 16);
        } else {
            painter->setBrush(m_color);
            painter->drawChord(boundingRect()
                                   .adjusted(1, 1, -1, -1)
                                   .translated(0, -halfHeight),
                               180 * 16, 180 * 16);

            painter->setCompositionMode(QPainter::CompositionMode_SourceOut);

            painter->setClipRect(0, 1, width(), height());
            painter->drawChord(boundingRect()
                                   .adjusted(2, 2, -2, -2)
                                   .translated(0, -halfHeight),
                               180 * 16, 180 * 16);
        }
    } else {
        painter->setBrush(m_color);
        painter->drawChord(
            boundingRect().adjusted(1, 1, -1, -1).translated(0, -halfHeight),
            180 * 16, 180 * 16);
    }
}

void
BGBorderBackground::drawUpperV(QPainter* painter) {
    qreal halfHeight = height() / 2;

    painter->setPen(Qt::NoPen);

    if (m_border) {
        if (m_style == ThreeDimensional) {
            painter->setBrush(shadowColor());
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(0, halfHeight),
                0, 180 * 16);

            painter->setClipRect(0, 0, width(), height() - 1);
            painter->setBrush(highlightColor());
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(0, halfHeight),
                0, 180 * 16);

            painter->setClipRect(0, 0, width(), height() - 2);
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(3, 3, -3, -3).translated(0, halfHeight),
                0, 180 * 16);
        } else {
            painter->setBrush(m_color);
            painter->drawChord(
                boundingRect().adjusted(1, 1, -1, -1).translated(0, halfHeight),
                0, 180 * 16);

            painter->setCompositionMode(QPainter::CompositionMode_SourceOut);

            painter->setClipRect(0, 0, width(), height() - 1);
            painter->drawChord(
                boundingRect().adjusted(2, 2, -2, -2).translated(0, halfHeight),
                0, 180 * 16);
        }
    } else {
        painter->setBrush(m_color);
        painter->drawChord(
            boundingRect().adjusted(1, 1, -1, -1).translated(0, halfHeight), 0,
            180 * 16);
    }
}
