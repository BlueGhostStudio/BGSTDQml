#ifndef CONTENTNODE_H
#define CONTENTNODE_H

#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGNode>

#include "dataconversion.h"
#include "strokegeometrynode.h"

class ContentNode : public QSGNode {
public:
    ContentNode();
    ~ContentNode();

    // void preprocess() override;

    void clear();
    void addPendingRenderNode(const Stroke& stroke);
    void prepareRender();
    bool isRequestRenderBuffer() const;

    void setPalette(const QList<QColor>& palette);
    QList<QColor> palette() const;

    void setDark(bool dark);
    bool isDark() const;

    void setActiveNodes(int active);
    int activeNodes() const;
    // QSGNode* activeNodesNode();

private:
    // void updateRange(const Stroke& stroke);

private:
    // QQuickItem* m_quickItem;
    //  QList<int> m_range = { -1, -1, -1, -1 };

    /*QList<Stroke> m_strokesBuffer;
    bool m_requestRenderBuffer = false;*/
    bool m_requestClear = true;
    QList<StrokeGeometryNode*> m_pendingRenderNodes;

    QSGNode m_activeWritingStrokeNodes;
    QSGNode m_inactiveWritingStrokeNodes;

    QSGFlatColorMaterial m_materialPalette[7];
    QSGFlatColorMaterial m_materialDarkPalette[7];
    bool m_dark = false;

    int m_activeNodes = -1;
};

#endif  // CONTENTNODE_H
