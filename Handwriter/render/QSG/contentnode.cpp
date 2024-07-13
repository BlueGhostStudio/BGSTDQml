#include "contentnode.h"

#include <hwcanvas.h>

#include <QMutexLocker>
#include <QSGSimpleRectNode>

// #include "hwcanvas.h"

ContentNode::ContentNode(HWCanvas* item) : m_ownItem(item) {
    // setFlag(QSGNode::UsePreprocess, true);
    setFlag(QSGNode::OwnedByParent, false);
    setFlag(QSGNode::OwnsGeometry, false);
    setFlag(QSGNode::OwnsMaterial, false);

    /*m_activeWritingStrokeNodes.setFlag(QSGNode::OwnedByParent, false);
    m_activeWritingStrokeNodes.setFlag(QSGNode::OwnsGeometry, false);
    m_activeWritingStrokeNodes.setFlag(QSGNode::OwnsMaterial, false);
    appendChildNode(&m_activeWritingStrokeNodes);*/

    /*m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnedByParent, false);
    m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnsGeometry, false);
    m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnsMaterial, false);*/

    // setActiveNodes(1);
    for (int i = 0; i < 100; ++i) {
        m_cache[i].setFlag(QSGNode::OwnedByParent, false);
    }
    m_activeCacheNode->setFlag(QSGNode::OwnedByParent, false);
    appendChildNode(m_activeCacheNode);
}

ContentNode::~ContentNode() {
    m_activeCacheNode->removeAllChildNodes();
    delete m_activeCacheNode;
    delete []m_cache;
}

QList<Stroke>
ContentNode::strokes() const {
    QList<Stroke> strokeList;

    for (int i = 0; i < CACHESIZE; ++i) {
        const QSGNode* node = &m_cache[i];
        for (int j = 0; j < node->childCount(); ++j) {
            StrokeGeometryNode* strokeGeoNode =
                static_cast<StrokeGeometryNode*>(node->childAtIndex(j));
            strokeList.append(strokeGeoNode->stroke());
        }
    }

    return strokeList;
}

QList<Stroke>
ContentNode::currentLineStrokes() const {
    QList<Stroke> strokeList;

    QRectF clipRect = m_ownItem->clipRect();
    for (int i = qMin(qFloor(clipRect.top() / CACHEHEIGHT), CACHEROW - 1);
         i <= qMin(qFloor(clipRect.bottom() / CACHEHEIGHT), CACHEROW - 1);
         ++i) {
        for (int j = 0; j <= CACHECOL - 1; ++j) {
            const QSGNode* node = &m_cache[i * CACHECOL + j];
            for (int k = 0; k < node->childCount(); ++k) {
                StrokeGeometryNode* strokeGeoNode =
                    static_cast<StrokeGeometryNode*>(node->childAtIndex(k));
                strokeList.append(strokeGeoNode->stroke());
            }
        }
    }

    return strokeList;
}

/*void
ContentNode::preprocess() {
    renderBuffer();
}*/

extern QMutex mutex;

void
ContentNode::clear() {
    // QMutexLocker locker(&mutex);
    /*m_strokesBuffer.clear();
    m_requestRenderBuffer = false;*/
    qDeleteAll(m_pendingRenderNodes.begin(), m_pendingRenderNodes.end());
    m_pendingRenderNodes.clear();
    m_requestClear = true;
}

void
ContentNode::addPendingRenderNode(const Stroke& stroke) {
    // QMutexLocker locker(&mutex);
    // updateRange(stroke);
    // m_strokesBuffer.append(stroke);
    // m_requestRenderBuffer = true;
    StrokeGeometryNode* node = new StrokeGeometryNode(stroke);
    node->setMaterial(!m_dark ? &m_materialPalette[stroke.color]
                              : &m_materialDarkPalette[stroke.color]);
    m_pendingRenderNodes.append(node);
}

void
ContentNode::prepareRender() {
    // QMutexLocker locker(&mutex);

    // activeCache();

    if (m_requestClear) {
        for (int i = 0; i < CACHESIZE; ++i) {
            m_cache[i].removeAllChildNodes();
        }
        m_requestClear = false;
    }

    /*bool willHide = m_activeWritingStrokeNodes.childCount() > 1024;
    while(!m_pendingRenderNodes.isEmpty()) {
        if(willHide) {
            QSGNode* firstChildNode = m_activeWritingStrokeNodes.firstChild();
            m_activeWritingStrokeNodes.removeChildNode(firstChildNode);
            m_inactiveWritingStrokeNodes.appendChildNode(firstChildNode);
        }
        QSGNode* node = m_pendingRenderNodes.takeFirst();
        m_activeWritingStrokeNodes.appendChildNode(node);
        // m_strokeNodesPool.appendChildNode(node);
    }*/
    while (!m_pendingRenderNodes.isEmpty()) {
        StrokeGeometryNode* strokeNode = m_pendingRenderNodes.takeFirst();
        Stroke stroke = strokeNode->stroke();
        QRectF strokeRect = QRectF(stroke.pos, stroke.prePos).normalized();
        int targetCacheIndex =
            qMin(qFloor(strokeRect.top() / CACHEHEIGHT), CACHEROW - 1) *
                CACHECOL +
            qMin(qFloor(strokeRect.left() / CACHEWIDTH), CACHECOL - 1);
        /*int targetCacheIndex = qMin(
            qMax(qFloor(QRectF(stroke.pos, stroke.prePos).normalized().top() /
                        25),
                 0),
            100);*/
        m_cache[targetCacheIndex].appendChildNode(strokeNode);
    }
    /*QMutexLocker locker(&mutex);

    if (m_requestClear) {
        while (childCount() > 0) {
            QSGNode* node = lastChild();
            removeChildNode(node);
            delete node;
        }
        m_requestClear = false;
    }

    if (m_requestRenderBuffer) {
        while (!m_strokesBuffer.isEmpty()) {
            const Stroke& stroke = m_strokesBuffer.takeFirst();
            StrokeGeometryNode* node = new StrokeGeometryNode(stroke);
            node->setMaterial(!m_dark ? &m_materialPalette[stroke.color]
                                      : &m_materialDarkPalette[stroke.color]);

            appendChildNode(node);
        }

        m_requestRenderBuffer = false;
    }*/
}

bool
ContentNode::isRequestRenderBuffer() const {
    return false;  // m_requestRenderBuffer;
}

void
ContentNode::setPalette(const QList<QColor>& palette) {
    if (palette.size() != 7) return;

    for (int i = 0; i < 7; i++) {
        const QColor& theColor = palette[i];
        m_materialPalette[i].setColor(theColor);
        m_materialDarkPalette[i].setColor(Stroke::darkColor(theColor));
    }
}

QList<QColor>
ContentNode::palette() const {
    QList<QColor> p;
    for (int i = 0; i < 7; ++i) {
        p.append(m_dark ? m_materialDarkPalette[i].color()
                        : m_materialPalette[i].color());
    }

    return p;
}

void
ContentNode::setDark(bool dark) {
    if (m_dark != dark) m_dark = dark;
}

bool
ContentNode::isDark() const {
    return m_dark;
}

void
ContentNode::activeCache() {
    QQuickItem* viewport = m_ownItem->viewportItem();
    if (m_renderRange != 1 && viewport && m_ownItem->combinedScale() != 1) {
        QRectF clipRect = m_ownItem->clipRect();
        QPair<int, int> rowRange(
            qMin(qFloor(clipRect.top() / CACHEHEIGHT), CACHEROW - 1),
            qMin(qFloor(clipRect.bottom() / CACHEHEIGHT), CACHEROW - 1));
        QPair<int, int> colRange;
        if (m_renderRange == 0) {
            colRange.first =
                qMin(qFloor(clipRect.left() / CACHEWIDTH), CACHECOL - 1);
            colRange.second =
                qMin(qFloor(clipRect.right() / CACHEWIDTH), CACHECOL - 1);
        } else if (m_renderRange == 2) {
            colRange.first = 0;
            colRange.second = CACHECOL - 1;
        }

        if (rowRange == m_activeCacheRowRange &&
            colRange == m_activeCacheColRange)
            return;

        m_activeCacheRowRange = rowRange;
        m_activeCacheColRange = colRange;
        m_activeCacheNode->removeAllChildNodes();
        for (int i = m_activeCacheRowRange.first;
             i <= m_activeCacheRowRange.second; ++i) {
            for (int j = m_activeCacheColRange.first;
                 j <= m_activeCacheColRange.second; ++j) {
                m_activeCacheNode->appendChildNode(&m_cache[i * CACHECOL + j]);
            }
        }
    } else if (m_activeCacheRowRange != QPair<int, int>{ 0, CACHEROW } &&
               m_activeCacheColRange != QPair<int, int>{ 0, CACHECOL }) {
        m_activeCacheNode->removeAllChildNodes();
        m_activeCacheRowRange = QPair<int, int>{ 0, CACHEROW };
        m_activeCacheColRange = QPair<int, int>{ 0, CACHECOL };

        for (int i = 0; i < CACHESIZE; ++i)
            m_activeCacheNode->appendChildNode(&m_cache[i]);
    }

    /*int i = 0;
    QList<int> inRange;
    while (i < m_activeCacheNode.childCount()) {
        CacheNode* cacheNode =
            static_cast<CacheNode*>(m_activeCacheNode.childAtIndex(i));
        if (cacheNode->m_index < m_activeCacheRange.first ||
            cacheNode->m_index > m_activeCacheRange.second)
            m_activeCacheNode.removeChildNode(cacheNode);
        else {
            inRange.append(i);
            i++;
        }
    }

    qDebug() << m_activeCacheRange << inRange;

    for (i = m_activeCacheRange.first; i <= m_activeCacheRange.second; i++)
    { qDebug() << "----" << i;
        // if (!inRange.contains(i))
    m_activeCacheNode.appendChildNode(&m_cache[i]);
    }

    qDebug() << m_activeCacheNode.childCount();*/
    /*m_activeCacheNode.removeAllChildNodes();
    for (int i = m_activeCacheRange.first; i <= m_activeCacheRange.second;
         ++i) {
        m_activeCacheNode.appendChildNode(&m_cache[i]);
    }*/
}

void
ContentNode::setRenderRange(int range) {
    if (m_renderRange != range) m_renderRange = range;
}

int
ContentNode::renderRange() const {
    return m_renderRange;
}

/*QList<Stroke>&&
ContentNode::strokes(const QPair<int, int>& rowRange,
                     const QPair<int, int>& colRange) {
    QList<Stroke> strokeList;
    for(int i = rowRange.first; i < rowRange.second; ++i)
}*/

/*void
ContentNode::setActiveNodes(int active) {
    if (m_activeNodes != active) {
        m_activeNodes = active;

        if (m_activeNodes == 0)
            removeChildNode(&m_inactiveWritingStrokeNodes);
        else
            appendChildNode(&m_inactiveWritingStrokeNodes);
    }
}

int
ContentNode::activeNodes() const {
    return m_activeNodes;
}*/

/*QSGNode*
ContentNode::activeNodesNode() {
    if (m_activeNodes == 0) {
        return &m_activeWritingStrokeNodes;
    } else
        return this;
}*/
