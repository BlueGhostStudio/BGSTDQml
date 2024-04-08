#include "strokegeometrynode.h"

StrokeGeometryNode::StrokeGeometryNode(const Stroke& stroke)
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0),
      m_stroke(stroke) {
    StrokeRenderBase::drawStroke(stroke);

    setGeometry(&m_geometry);
    setFlag(QSGNode::OwnedByParent, true);
    setFlag(QSGNode::OwnsGeometry, false);
    setFlag(QSGNode::OwnsMaterial, false);
}

Stroke
StrokeGeometryNode::stroke() const {
    return m_stroke;
}

void
StrokeGeometryNode::render(qreal radius, const QPointF& pos) {
    int sampling = 8;

    qreal ur = 2 * M_PI / sampling;
    m_geometry.allocate(sampling);

    QSGGeometry::Point2D* vds = m_geometry.vertexDataAsPoint2D();

    int i = 0;
    int j = sampling - 1;
    int k = 0;

    while (i < sampling) {
        QPointF p0(qCos(j * ur) * radius, qSin(j * ur) * radius);
        p0 += pos;
        vds[i].set(p0.x(), p0.y());
        i++;
        j--;

        if (i < sampling) {
            QPointF p1(qCos(k * ur) * radius, qSin(k * ur) * radius);
            p1 += pos;
            vds[i].set(p1.x(), p1.y());
            i++;
            k++;
        }
    }
}

void
StrokeGeometryNode::render(qreal prePosRadius, qreal posRadius,
                           const QPointF& prePos, const QPointF& pos,
                           qreal prePosStartAngle, qreal prePosEndAngle,
                           qreal posStartAngle, qreal posEndAngle) {
    int sampling = 4;

    qreal ur0 = qAbs(prePosEndAngle - prePosStartAngle) / sampling;
    qreal ur1 = (2 * M_PI - qAbs(posEndAngle - posStartAngle)) / sampling;

    int ang0_0 = qFloor(sampling / 2);
    int ang0_1 = ang0_0 + 1;

    m_geometry.allocate((sampling + 1) * 2);
    QSGGeometry::Point2D* vds = m_geometry.vertexDataAsPoint2D();

    int i = 0;
    do {
        if (ang0_0 >= 0) {
            QPointF p(qCos(ang0_0 * ur0 + prePosStartAngle) * prePosRadius,
                      -qSin(ang0_0 * ur0 + prePosStartAngle) * prePosRadius);
            p += prePos;
            vds[i].set(p.x(), p.y());
            i++;
        }

        if (ang0_1 <= sampling) {
            QPointF p(qCos(ang0_1 * ur0 + prePosStartAngle) * prePosRadius,
                      -qSin(ang0_1 * ur0 + prePosStartAngle) * prePosRadius);
            p += prePos;
            vds[i].set(p.x(), p.y());
            i++;
        }

        ang0_0--;
        ang0_1++;
    } while (i <= sampling);

    bool odd = sampling % 2;
    int ang1_0 = odd ? sampling : 0;
    int ang1_1 = odd ? 0 : sampling;

    int j = 0;
    int mAn = qFloor(sampling / 2);

    do {
        if ((odd && ang1_0 > mAn) || ang1_0 <= mAn) {
            QPointF p(qCos(ang1_0 * ur1 + posStartAngle) * posRadius,
                      -qSin(ang1_0 * ur1 + posStartAngle) * posRadius);
            p += pos;
            vds[i + j].set(p.x(), p.y());
            j++;
        }

        if ((odd && ang1_1 <= mAn) || ang1_1 > mAn) {
            QPointF p(qCos(ang1_1 * ur1 + posStartAngle) * posRadius,
                      -qSin(ang1_1 * ur1 + posStartAngle) * posRadius);
            p += pos;
            vds[i + j].set(p.x(), p.y());
            j++;
        }

        odd ? ang1_0-- : ang1_0++;
        odd ? ang1_1++ : ang1_1--;
    } while (j <= sampling);
}
