#ifndef HWCANVASBASE_H
#define HWCANVASBASE_H

#include <QQuickItem>

#include <dataconversion.h>

class HWCanvasBase {
public:
    HWCanvasBase();

    virtual void setCanvasPalette(const QList<QColor>& palette) = 0;
    virtual QList<QColor> canvasPalette() const = 0;

    virtual bool darkPalette() const = 0;
    virtual void setDarkPalette(bool dark) = 0;

    /*qreal dpi() const;
    void setDpi(qreal dots);
    qreal dpMm() const;
    void setDpMm(qreal dots);*/
    // bool extracted() const;

    /*virtual void setByDevicePixelRatio(bool bdpr) = 0;
    virtual bool byDevicePixelRatio() const = 0;*/

    virtual void clear() = 0;
    virtual void drawStroke(const QVariant& varStroke, bool dpi = false) = 0;
    virtual void drawStrokes(const QVariantList& strokes, bool dpi = false) = 0;
    virtual void copy(HWCanvasBase* canvas) = 0;

    QRectF range() const;
    void resetRange();

    virtual void updateCanvas() = 0;
    void updateCanvasSize(const QSize& size);

protected:
    virtual QObject* toQObject() = 0;

    void updateRange(const Stroke& stroke);

protected:
    //qreal m_dpi = 96;

    bool m_darkPalette = false;

    QRectF m_range;
};

#endif  // HWCANVASBASE_H
