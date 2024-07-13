#include "hwsgcanvas.h"

#include <hwsettings.h>

#include <QQmlEngine>
#include <QQuickWindow>

#include "strokegeometrynode.h"

extern QMutex mutex;

HWCanvas::HWCanvas(QQuickItem* parent)
    : QQuickItem(parent), m_contentNode(new ContentNode(this)) {
    setFlag(QQuickItem::ItemObservesViewport, true);
    setFlag(ItemHasContents, true);
    setClip(true);

    m_holdTimer.setSingleShot(true);
    m_holdTimer.setInterval(750);
    m_holdTimer.callOnTimeout([=]() {
        m_pressAndHold = true;

        // if (m_holdState == 0)
            emit pressAndHold(m_lastPressPos);

        // emit holdState(m_holdState, m_prePos);
        // m_holdState++;

        // m_holdTimer.start();
    });

    /*QObject::connect(this, &HWCanvas::destroyed, this, [=]() {
        qDebug() << "HWCanvas destroyed";
        delete m_contentNode;
    });*/
}

HWCanvas::~HWCanvas() {
    delete m_contentNode;
}

void
HWCanvas::classBegin() {
    QQuickItem::classBegin();
    m_settings = qmlEngine(this)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");

    if (m_settings->isTouchScreen())
        setAcceptTouchEvents(true);
    else
        setAcceptedMouseButtons(Qt::AllButtons);

    QList<QColor> pcolors;
    foreach (const QVariant& var, m_settings->palette().toList()) {
        pcolors.append(var.value<QColor>());
    }

    m_contentNode->setPalette(pcolors);
}

void
HWCanvas::componentComplete() {
    QQuickItem::componentComplete();

    QObject::connect(this, &QQuickItem::scaleChanged, this,
                     &HWCanvas::cumulativeScale);

    QQuickItem* theParentItem = parentItem();
    while (theParentItem) {
        QObject::connect(theParentItem, &QQuickItem::scaleChanged, this,
                         &HWCanvas::cumulativeScale);
        theParentItem = theParentItem->parentItem();
    }

    auto init = [=](QQuickWindow* window) {
        QObject::disconnect(window, &QQuickWindow::beforeSynchronizing, this,
                            nullptr);
        QObject::connect(
            window, &QQuickWindow::beforeSynchronizing, this,
            [=]() { m_contentNode->activeCache(); }, Qt::DirectConnection);
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
    return m_contentNode->palette();
}

void
HWCanvas::setCanvasPalette(const QList<QColor>& palette) {
    m_contentNode->setPalette(palette);

    emit canvasPaletteChanged();
}

bool
HWCanvas::darkPalette() const {
    return m_contentNode->isDark();
}

void
HWCanvas::setDarkPalette(bool dark) {
    if (m_contentNode->isDark() != dark) {
        m_contentNode->setDark(dark);
        emit darkPaletteChanged();
    }
}

HWCVType::ContentVisible
HWCanvas::contentVisible() const {
    return m_contentVisible;
}

void
HWCanvas::setContentVisible(HWCVType::ContentVisible contentVisible) {
    if (m_contentVisible != contentVisible) {
        m_contentVisible = contentVisible;
        m_contentNode->setRenderRange(m_contentVisible);
        update();
        emit contentVisibleChanged();
    }
}

qreal
HWCanvas::combinedScale() const {
    return m_combinedScale;
}

void
HWCanvas::clear() {
    resetRange();
    m_contentNode->clear();
    update();
    // updateCanvas();

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
HWCanvas::drawStroke(const Stroke& stroke, bool dpi) {
    Stroke theStroke(std::move(stroke));

    if (dpi) {
        qreal r = m_settings->dpi() / 150;
        theStroke.pos *= r;
        theStroke.prePos *= r;
        theStroke.size *= r;
        theStroke.preSize *= r;
    }
    updateRange(theStroke);
    m_contentNode->addPendingRenderNode(theStroke);
    update();
}

void
HWCanvas::drawStrokes(const QVariantList& strokes, bool dpi) {
    foreach (const QVariant& varStroke, strokes) {
        drawStroke(varStroke, dpi);
    }
    emit strokesUpdated();
}

void
HWCanvas::drawStrokes(const QList<Stroke>& strokes, bool dpi) {
    foreach(const Stroke& stroke, strokes) {
        drawStroke(stroke, dpi);
    }
    emit strokesUpdated();
}

void
HWCanvas::copy(HWCanvasBase* canvas) {
    HWCanvas* sgCanvas = dynamic_cast<HWCanvas*>(canvas);
    if (sgCanvas) {
        ContentNode* contentNode = sgCanvas->m_contentNode;

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

QList<Stroke>
HWCanvas::strokes(HWCVType::ContentVisible range) {
    if (range == HWCVType::VisibleFullContent)
        return m_contentNode->strokes();
    else
        return m_contentNode->currentLineStrokes();
}

void
HWCanvas::updateCanvas() {
    update();
}

void
HWCanvas::cumulativeScale() {
    qreal theScale = scale();

    QQuickItem* theParentItem = parentItem();
    while (theParentItem) {
        theScale *= theParentItem->scale();
        theParentItem = theParentItem->parentItem();
    }

    if (!qFuzzyCompare(theScale, m_combinedScale)) {
        m_combinedScale = theScale;
        emit combinedScaleChanged();
    }
}

/*void
HWCanvas::drawStroke(Stroke stroke, bool dpi) {
    if (dpi) {
        qreal r = m_settings->dpi() / 150;
        stroke.pos *= r;
        stroke.prePos *= r;
        stroke.size *= r;
        stroke.preSize *= r;
    }
    updateRange(stroke);
    m_contentNode->addPendingRenderNode(stroke);
    update();
}*/

void
HWCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
        event->modifiers() == Qt::NoModifier) {
        m_lastPressPos = event->position();
        // emit clicked(m_prePos);
        m_holdTimer.start();
    }

    emit pointPressed(event->position());
}

void
HWCanvas::mouseMoveEvent(QMouseEvent* event) {
    m_holdTimer.stop();

    if (m_pressAndHold)
        emit holdAndMove(event->position());

    emit pointMove(event->position());
}

void
HWCanvas::mouseReleaseEvent(QMouseEvent* event) {
    m_holdTimer.stop();
    if (event->position() == event->point(0).pressPosition())
        emit clicked(event->position());

    m_pressAndHold = false;

    emit pointRelease(event->position());
}

void
HWCanvas::touchEvent(QTouchEvent* event) {
    if (event->pointCount() == 1) {
        switch (event->touchPointStates()) {
        case Qt::TouchPointReleased:
            m_holdTimer.stop();
            m_pressAndHold = false;
            emit pointRelease(event->point(0).position());
            // m_holdState = 0;
            break;
        case Qt::TouchPointMoved:
            m_holdTimer.stop();
            if (m_pressAndHold)
                emit holdAndMove(event->point(0).position());
            else
                emit pointMove(event->point(0).position());
            break;
        case Qt::TouchPointPressed:
            m_lastPressPos = event->point(0).position();
            m_holdTimer.start();
            emit pointPressed(m_lastPressPos);
            break;
        }
    }
}

QSGNode*
HWCanvas::updatePaintNode(QSGNode* /*oldNode*/, UpdatePaintNodeData*) {
    m_contentNode->prepareRender();

    return m_contentNode;
}
