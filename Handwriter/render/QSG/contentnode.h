#ifndef CONTENTNODE_H
#define CONTENTNODE_H

#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGNode>

// #include "dataconversion.h"
#include "strokegeometrynode.h"

#define CACHEROW 60
#define CACHEHEIGHT 25
#define CACHECOL 15
#define CACHEWIDTH 100
#define CACHESIZE CACHECOL* CACHEROW

class HWCanvas;

class ContentNode : public QSGNode {
public:
    ContentNode(HWCanvas* item);
    ~ContentNode();

    // void preprocess() override;

    QList<Stroke> strokes() const;
    QList<Stroke> currentLineStrokes() const;

    void clear();
    void addPendingRenderNode(const Stroke& stroke);
    void prepareRender();
    bool isRequestRenderBuffer() const;

    void setPalette(const QList<QColor>& palette);
    QList<QColor> palette() const;

    void setDark(bool dark);
    bool isDark() const;

    void activeCache();

    void setRenderRange(int range);
    int renderRange() const;
    /*void setActiveNodes(int active);
    int activeNodes() const;*/
    // QSGNode* activeNodesNode();

private:
    // void updateRange(const Stroke& stroke);
    /*QList<Stroke>&& strokes(const QPair<int, int>& rowRange,
                          const QPair<int, int>& colRange);*/

private:
    // QQuickItem* m_quickItem;
    //  QList<int> m_range = { -1, -1, -1, -1 };

    /*QList<Stroke> m_strokesBuffer;
    bool m_requestRenderBuffer = false;*/
    bool m_requestClear = true;
    QList<StrokeGeometryNode*> m_pendingRenderNodes;

    // QSGNode m_activeWritingStrokeNodes;
    // QSGNode m_inactiveWritingStrokeNodes;

    HWCanvas* m_ownItem;
    QSGNode* m_cache = new QSGNode[CACHESIZE];
    QPair<int, int> m_activeCacheRowRange{ -1, -1 };
    QPair<int, int> m_activeCacheColRange{ -1, -1 };
    QSGNode* m_activeCacheNode = new QSGNode;

    QSGFlatColorMaterial m_materialPalette[7];
    QSGFlatColorMaterial m_materialDarkPalette[7];
    bool m_dark = false;

    int m_renderRange = 0;
};

#endif  // CONTENTNODE_H
