#ifndef HWSGCANVAS_H
#define HWSGCANVAS_H

#include <contentnode.h>
#include <hwcanvasbase.h>

#include <QPainter>

class HWSettings;

class HWCanvas : public QQuickItem, public HWCanvasBase {
    Q_OBJECT
    QML_ELEMENT

    QML_EXTENDED_NAMESPACE(HWCVType)

    Q_PROPERTY(QList<QColor> canvasPalette READ canvasPalette WRITE
                   setCanvasPalette NOTIFY canvasPaletteChanged)

    Q_PROPERTY(bool darkPalette READ darkPalette WRITE setDarkPalette NOTIFY
                   darkPaletteChanged)

    Q_PROPERTY(QRectF range READ range NOTIFY rangeChanged)
    Q_PROPERTY(HWCVType::ContentVisible contentVisible READ contentVisible WRITE
                   setContentVisible NOTIFY contentVisibleChanged)

    Q_PROPERTY(
        qreal combinedScale READ combinedScale NOTIFY combinedScaleChanged)

public:
    HWCanvas(QQuickItem* parent = nullptr);
    ~HWCanvas();

    /*enum ContentVisible {
        VisibleViewportOnly,
        VisibleFullContent,
        VisibleLine
    };
    Q_ENUM(ContentVisible)*/

    void classBegin() override;
    void componentComplete() override;

    QList<QColor> canvasPalette() const override;
    void setCanvasPalette(const QList<QColor>& palette) override;

    bool darkPalette() const override;
    void setDarkPalette(bool dark) override;

    HWCVType::ContentVisible contentVisible() const;
    void setContentVisible(HWCVType::ContentVisible contentVisible);

    qreal combinedScale() const;

    Q_INVOKABLE void clear() override;
    Q_INVOKABLE void drawStroke(const QVariant& varStroke,
                                bool dpi = false) override;
    Q_INVOKABLE void drawStroke(const Stroke& stroke,
                                bool dpi = false) override;

    Q_INVOKABLE void drawStrokes(const QVariantList& strokes,
                                 bool dpi = false) override;
    Q_INVOKABLE void drawStrokes(const QList<Stroke>& strokes,
                                 bool dpi = false) override;

    Q_INVOKABLE void copy(HWCanvasBase* canvas) override;

    QList<Stroke> strokes(HWCVType::ContentVisible range) override;

    void updateCanvas() override;

signals:
    void clicked(const QPointF& pos);
    void pressAndHold(const QPointF& pos);
    // void holdState(int state, const QPointF& pos);
    void holdAndMove(const QPointF& pos);
    void pointPressed(const QPointF& pos);
    void pointRelease(const QPointF& pos);
    void pointMove(const QPointF& pos);

    void canvasPaletteChanged();

    void darkPaletteChanged();

    /*void dpiChanged();
    void dpMmChanged();
    void byDevicePixelRatioChanged();*/

    void cleared();
    void rangeChanged();

    void strokesUpdated();

    void contentVisibleChanged();

    void combinedScaleChanged();

protected slots:
    void cumulativeScale();

protected:
    QObject* toQObject() override;
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
    // void drawStroke(Stroke stroke, bool dpi = false);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void touchEvent(QTouchEvent* event) override;
    // void renderStrokes(QPainter* painter, const QList<Stroke>& strokes)
    // const;

protected:
    HWSettings* m_settings;
    qreal m_combinedScale = 1.0;

    QPointF m_lastPressPos;
    bool m_pressAndHold = false;
    // int m_holdState = 0;

    QTimer m_holdTimer;

private:
    ContentNode* m_contentNode;
    HWCVType::ContentVisible m_contentVisible = HWCVType::VisibleViewportOnly;
};

#endif  // HWSGCANVAS_H
