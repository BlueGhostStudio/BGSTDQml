#include "contentnode.h"

#include <QMutexLocker>
#include <QSGSimpleRectNode>

// #include "hwcanvas.h"
#include "strokegeometrynode.h"

ContentNode::ContentNode() {
    // setFlag(QSGNode::UsePreprocess, true);
    setFlag(QSGNode::OwnedByParent, false);
    setFlag(QSGNode::OwnsGeometry, false);
    setFlag(QSGNode::OwnsMaterial, false);
}

ContentNode::~ContentNode() { clear(); }

/*void
ContentNode::preprocess() {
    renderBuffer();
}*/

extern QMutex mutex;

void
ContentNode::clear() {
    QMutexLocker locker(&mutex);
    m_strokesBuffer.clear();
    m_requestRenderBuffer = false;
    m_requestClear = true;
}

void
ContentNode::addStroke(const Stroke& stroke) {
    QMutexLocker locker(&mutex);
    // updateRange(stroke);
    m_strokesBuffer.append(stroke);
    m_requestRenderBuffer = true;
}

void
ContentNode::renderBuffer() {
    QMutexLocker locker(&mutex);

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
    }
}

bool
ContentNode::isRequestRenderBuffer() const {
    return m_requestRenderBuffer;
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
