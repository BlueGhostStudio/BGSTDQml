#ifndef HWSGCANVAS_H
#define HWSGCANVAS_H

#include <contentnode.h>
#include <hwcanvasbase.h>

class HWSettings;

class HWCanvas : public QQuickItem, public HWCanvasBase {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<QColor> canvasPalette READ canvasPalette WRITE
                   setCanvasPalette NOTIFY canvasPaletteChanged)

    Q_PROPERTY(bool darkPalette READ darkPalette WRITE setDarkPalette NOTIFY
                   darkPaletteChanged)

    /*Q_PROPERTY(qreal dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(qreal dpMm READ dpMm WRITE setDpMm NOTIFY dpMmChanged)
    Q_PROPERTY(bool byDevicePixelRatio READ byDevicePixelRatio WRITE
                   setByDevicePixelRatio NOTIFY byDevicePixelRatioChanged)*/

    Q_PROPERTY(QRectF range READ range NOTIFY rangeChanged)

public:
    HWCanvas(QQuickItem* parent = nullptr);

    void classBegin() override;
    void componentComplete() override;

    QList<QColor> canvasPalette() const override;
    void setCanvasPalette(const QList<QColor>& palette) override;

    bool darkPalette() const override;
    void setDarkPalette(bool dark) override;

    /*void setByDevicePixelRatio(bool) override;
    bool byDevicePixelRatio() const override;*/

    Q_INVOKABLE void clear() override;
    Q_INVOKABLE void drawStroke(const QVariant& varStroke,
                                bool dpi = false) override;
    Q_INVOKABLE void drawStrokes(const QVariantList& strokes,
                                 bool dpi = false) override;
    Q_INVOKABLE void copy(HWCanvasBase* canvas) override;

    void updateCanvas() override;

signals:
    void canvasPaletteChanged();

    void darkPaletteChanged();

    /*void dpiChanged();
    void dpMmChanged();
    void byDevicePixelRatioChanged();*/

    void cleared();
    void rangeChanged();

    void strokesUpdated();

protected:
    QObject* toQObject() override;
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    void drawStroke(Stroke stroke, bool dpi = false);

protected:
    HWSettings* m_settings;

private:
    ContentNode m_contentNode;
};

#endif  // HWSGCANVAS_H
