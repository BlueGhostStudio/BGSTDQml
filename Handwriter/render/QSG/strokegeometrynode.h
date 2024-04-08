#ifndef STROKEGEOMETRYNODE_H
#define STROKEGEOMETRYNODE_H

#include <QSGGeometryNode>

#include "dataconversion.h"
#include "strokerenderbase.h"

class StrokeGeometryNode : public QSGGeometryNode, public StrokeRenderBase {
public:
    StrokeGeometryNode(const Stroke& stroke);

    Stroke stroke() const;

protected:
    void render(qreal radius, const QPointF& pos) override;
    void render(qreal prePosRadius, qreal posRadius, const QPointF& prePos,
                const QPointF& pos, qreal prePosStartAngle,
                qreal prePosEndAngle, qreal posStartAngle,
                qreal posEndAngle) override;

private:
    QSGGeometry m_geometry;

    Stroke m_stroke;
};

#endif  // STROKEGEOMETRYNODE_H
