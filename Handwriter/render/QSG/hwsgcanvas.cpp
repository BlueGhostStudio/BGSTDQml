#include "hwsgcanvas.h"

#include <hwsettings.h>

#include <QQmlEngine>
#include <QQuickWindow>

#include "strokegeometrynode.h"

HWCanvas::HWCanvas(QQuickItem* parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
    setClip(true);
}

void
HWCanvas::classBegin() {
    QQuickItem::classBegin();
    m_settings = qmlEngine(this)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");

    QList<QColor> pcolors;
    foreach (const QVariant& var, m_settings->palette().toList()) {
        pcolors.append(var.value<QColor>());
    }

    m_contentNode.setPalette(pcolors);
}

void
HWCanvas::componentComplete() {
    QQuickItem::componentComplete();

    auto init = [=](QQuickWindow* window) {
        QObject::connect(window, &QQuickWindow::beforeSynchronizing, this,
            [=]() { m_contentNode.renderBuffer(); }, Qt::DirectConnection);
    };

    if (window())
        init(window());
    else
        QObject::connect(this, &QQuickItem::windowChanged, this,
                         [=](QQuickWindow* window) {
                             if (window) init(window);
                         });
}

QList<QColor>
HWCanvas::canvasPalette() const {
    return m_contentNode.palette();
}

void
HWCanvas::setCanvasPalette(const QList<QColor>& palette) {
    m_contentNode.setPalette(palette);

    emit canvasPaletteChanged();
}

bool
HWCanvas::darkPalette() const {
    return m_contentNode.isDark();
}

void
HWCanvas::setDarkPalette(bool dark) {
    if (m_contentNode.isDark() != dark) {
        m_contentNode.setDark(dark);
        emit darkPaletteChanged();
    }
}

/*void
HWCanvas::setByDevicePixelRatio(bool) {
    emit byDevicePixelRatioChanged();
}

bool
HWCanvas::byDevicePixelRatio() const {
    return true;
}*/

void
HWCanvas::clear() {
    resetRange();
    m_contentNode.clear();
    updateCanvas();

    emit cleared();
    emit rangeChanged();
}

QObject*
HWCanvas::toQObject() {
    return this;
}

void
HWCanvas::drawStroke(const QVariant& varStroke, bool dpi) {
    drawStroke(variant2Stroke(varStroke), dpi);
}

void
HWCanvas::drawStrokes(const QVariantList& strokes, bool dpi) {
    foreach (const QVariant& varStroke, strokes) {
        drawStroke(varStroke, dpi);
    }
    emit strokesUpdated();
}

void
HWCanvas::copy(HWCanvasBase* canvas) {
    HWCanvas* sgCanvas = dynamic_cast<HWCanvas*>(canvas);
    if (sgCanvas) {
        ContentNode* contentNode = &sgCanvas->m_contentNode;

        if (contentNode->isRequestRenderBuffer()) {
            qDebug() << "waitting rendered"
                     << contentNode->isRequestRenderBuffer();

            QTimer::singleShot(500, this, [=]() { copy(canvas); });
        } else {
            qDebug() << "no requestRender OR after render"
                     << contentNode->childCount();

            QSGNode* strokeNode = contentNode->firstChild();
            while (strokeNode) {
                StrokeGeometryNode* stgNode =
                    static_cast<StrokeGeometryNode*>(strokeNode);
                if (stgNode) drawStroke(stgNode->stroke());

                strokeNode = strokeNode->nextSibling();
            }
        }
    }
}

void
HWCanvas::updateCanvas() {
    update();
}

void
HWCanvas::drawStroke(Stroke stroke, bool dpi) {
    if (dpi) {
        qreal r = m_settings->dpi() / 150;
        stroke.pos *= r;
        stroke.prePos *= r;
        stroke.size *= r;
        stroke.preSize *= r;
    }
    updateRange(stroke);
    m_contentNode.addStroke(stroke);
    update();
}

QSGNode*
HWCanvas::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    if (!oldNode) {
        /*oldNode = new QSGNode;

        oldNode->appendChildNode(&m_contentNode);*/
        oldNode = &m_contentNode;
    }

    return oldNode;
}
