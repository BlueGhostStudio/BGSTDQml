#include "hwsgguideline.h"

#include <hwpaper.h>
#include <hwsettings.h>

HWGuideLine::HWGuideLine(QQuickItem* parent)
    : QQuickItem(parent), HWGuideLineBase(parent) {
    setFlag(ItemHasContents, true);

    if (parent) {
        setX(0);
        setY(0);
        /*QObject::connect(parent, &QQuickItem::widthChanged, this,
                         &HWGuideLine::resize);
        QObject::connect(parent, &QQuickItem::heightChanged, this,
                         &HWGuideLine::resize);*/
        QObject::connect(this, &HWGuideLine::widthChanged, this,
                         [=]() { updateGuideLine(); });
        QObject::connect(this, &HWGuideLine::heightChanged, this,
                         [=]() { updateGuideLine(); });

        // resize();
        updateGuideLine();
    }

    QObject::connect(this, &HWGuideLine::visibleChanged, this,
                     &HWGuideLine::update);
}

/*void
HWGuideLine::classBegin() {
    QQuickItem::classBegin();
    qDebug() << "=-=============";
}

void
HWGuideLine::componentComplete() {
    QQuickItem::componentComplete();
    qDebug() << "=-=============";
}*/

void
HWGuideLine::drawGuideLine() {
    QQuickItem* parentItem = this->parentItem();

    if (!m_geometry || !m_material) return;

    QSizeF paperSize = parentItem->size();

    if (m_guideLineStyle.type == 0) {
        qreal posX = m_horizontalPaddingPx;
        qreal posY = m_topPaddingPx;
        qreal rightPosX = paperSize.width() - posX;
        qreal bottomPosY = paperSize.height() -
                           m_bottomPaddingPx;  // topOffset - bottomPaddingPx;

        int index = 0;
        int count = (m_rowCount + m_columnCount + 2) * 2;

        if (count > 0) {
            m_geometry->allocate(count);
            QSGGeometry::Point2D* vds = m_geometry->vertexDataAsPoint2D();
            for (int i = 0; i <= m_rowCount; i++) {
                vds[index].set(posX, posY + m_spacingPx * i);
                index++;
                vds[index].set(rightPosX, posY + m_spacingPx * i);
                index++;
            }
            for (int i = 0; i <= m_columnCount; i++) {
                vds[index].set(posX + m_spacingPx * i, posY);
                index++;
                vds[index].set(posX + m_spacingPx * i, bottomPosY);
                index++;
            }
        }
    } else if (m_guideLineStyle.type == 1) {
        qreal posX = m_horizontalPaddingPx;
        qreal posY = m_topPaddingPx;
        qreal rightPosX = paperSize.width() - m_horizontalPaddingPx;

        if (m_rowCount > 0) {
            m_geometry->allocate(m_rowCount * 2 * (m_spacingPx > 0 ? 2 : 1));

            QSGGeometry::Point2D* vds = m_geometry->vertexDataAsPoint2D();
            for (int i = 0; i < m_rowCount; i++) {
                vds[i * 4].set(posX, posY);
                vds[i * 4 + 1].set(rightPosX, posY);
                posY += m_lineHeightPx;
                vds[i * 4 + 2].set(posX, posY);
                vds[i * 4 + 3].set(rightPosX, posY);
                posY += m_spacingPx;
            }
        }
    }

    m_material->setColor(m_guideLineStyle.color);
}

void
HWGuideLine::updateGuideLine() {
    HWGuideLineBase::updateGuideLine();
    update();
}

/*void
HWGuideLine::resize() {
    updateGuideLine();
    QQuickItem* parent = parentItem();

    if (parent) {
        setWidth(parent->width());
        setHeight(parent->height());

        updateGuideLine();
    }
}*/

QSGNode*
HWGuideLine::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    QSGNode* resultNode = nullptr;
    if (isVisible()) {
        QSGGeometryNode* n = static_cast<QSGGeometryNode*>(oldNode);

        if (!n) {
            n = new QSGGeometryNode();
            m_geometry =
                new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            m_geometry->setDrawingMode(QSGGeometry::DrawLines);
            m_geometry->setLineWidth(1);
            n->setGeometry(m_geometry);
            n->setFlag(QSGNode::OwnsGeometry);

            m_material = new QSGFlatColorMaterial;
            n->setMaterial(m_material);
            n->setFlag(QSGNode::OwnsMaterial);
        } else {
            m_geometry = n->geometry();
            n->material();
        }

        drawGuideLine();

        n->markDirty(QSGNode::DirtyGeometry);

        resultNode = n;
    } else if (oldNode) {
        delete oldNode;

        resultNode = nullptr;
    }

    return resultNode;
}

QObject*
HWGuideLine::toQObject() const {
    return const_cast<HWGuideLine*>(this);
}
