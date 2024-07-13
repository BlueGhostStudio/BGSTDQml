#ifndef HWOUTPUT_H
#define HWOUTPUT_H

#include <hwcanvas.h>

#include <QPainter>
#include <QPicture>
#include "strokerenderbase.h"
// #include <QQmlEngine>
// #include <QQuickPaintedItem>

class HWOutput : public StrokeRenderBase {
public:
    HWOutput();

    QUrl background() const;
    bool setBackground(const QUrl& background);

    QColor color() const;
    bool setColor(const QColor& color);

    QList<QColor> palette() const;
    void setPalette(const QList<QColor>& palette);

    bool mono() const;
    bool setMono(bool mono);

    bool outline() const;
    bool setOutline(bool outline);

    void paint(const QList<Stroke>& strokeList);
    void output(QPainter* painter, const QSizeF& sourceSize,
                const QPointF& targetPos, qreal devicePixelRatio = 1);

protected:
    void render(qreal radius, const QPointF& pos) override;
    void render(qreal prePosRadius, qreal posRadius, const QPointF& prePos,
                const QPointF& pos, qreal prePosStartAngle,
                qreal prePosEndAngle, qreal posStartAngle,
                qreal posEndAngle) override;

private:
    QRectF m_rect;

    QUrl m_background;
    QColor m_color{ Qt::white };

    QList<QColor> m_palette;
    bool m_mono = false;
    bool m_outline = false;

    QImage m_backgroundImage;
    QPicture m_contentPicture;
    QPainter m_renderPainter;
};

#endif  // HWOUTPUT_H
