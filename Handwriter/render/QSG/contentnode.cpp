#include "contentnode.h"

#include <QMutexLocker>
#include <QSGSimpleRectNode>

// #include "hwcanvas.h"

ContentNode::ContentNode() {
    // setFlag(QSGNode::UsePreprocess, true);
    setFlag(QSGNode::OwnedByParent, false);
    setFlag(QSGNode::OwnsGeometry, false);
    setFlag(QSGNode::OwnsMaterial, false);

    m_activeWritingStrokeNodes.setFlag(QSGNode::OwnedByParent, false);
    m_activeWritingStrokeNodes.setFlag(QSGNode::OwnsGeometry, false);
    m_activeWritingStrokeNodes.setFlag(QSGNode::OwnsMaterial, false);
    appendChildNode(&m_activeWritingStrokeNodes);

    m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnedByParent, false);
    m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnsGeometry, false);
    m_inactiveWritingStrokeNodes.setFlag(QSGNode::OwnsMaterial, false);

    setActiveNodes(1);
}

ContentNode::~ContentNode() { clear(); }

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

    if (m_requestClear) {
        /*while (childCount() > 0) {
            QSGNode* node = lastChild();
            removeChildNode(node);
            delete node;
        }*/
        m_activeWritingStrokeNodes.removeAllChildNodes();
        m_inactiveWritingStrokeNodes.removeAllChildNodes();
        m_requestClear = false;
    }

    bool willHide = m_activeWritingStrokeNodes.childCount() > 1024;
    while(!m_pendingRenderNodes.isEmpty()) {
        if(willHide) {
            QSGNode* firstChildNode = m_activeWritingStrokeNodes.firstChild();
            m_activeWritingStrokeNodes.removeChildNode(firstChildNode);
            m_inactiveWritingStrokeNodes.appendChildNode(firstChildNode);
        }
        QSGNode* node = m_pendingRenderNodes.takeFirst();
        m_activeWritingStrokeNodes.appendChildNode(node);
        // m_strokeNodesPool.appendChildNode(node);
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
        float h, s, l;
        theColor.getHslF(&h, &s, &l);
        m_materialPalette[i].setColor(theColor);
        m_materialDarkPalette[i].setColor(QColor::fromHslF(h, s, 1 - l));
    }
}

QList<QColor>
ContentNode::palette() const {
    QList<QColor> p;
    for (int i = 0; i < 7; ++i) p.append(m_materialPalette[i].color());

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
}

/*QSGNode*
ContentNode::activeNodesNode() {
    if (m_activeNodes == 0) {
        return &m_activeWritingStrokeNodes;
    } else
        return this;
}*/
