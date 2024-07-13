#ifndef HWSGGUIDELINE_H
#define HWSGGUIDELINE_H

#include <hwguidelinebase.h>

#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGNode>

class HWGuideLine : public QQuickItem, public HWGuideLineBase {
    Q_OBJECT
    QML_ELEMENT

    QML_EXTENDED_NAMESPACE(HWGLType)

    Q_PROPERTY(QJSValue layout READ layout WRITE setLayout NOTIFY
                   layoutChanged)

    Q_PROPERTY(HWGLType::Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(qreal spacingPx READ spacingPx NOTIFY spacingPxChanged)
    Q_PROPERTY(qreal lineHeightPx READ lineHeightPx NOTIFY lineHeightPxChanged)
    Q_PROPERTY(qreal horizontalPaddingPx READ horizontalPaddingPx NOTIFY
                   horizontalPaddingPxChanged)
    Q_PROPERTY(qreal topPaddingPx READ topPaddingPx NOTIFY topPaddingPxChanged)
    Q_PROPERTY(qreal bottomPaddingPx READ bottomPaddingPx NOTIFY
                   bottomPaddingPxChanged)
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ columnCount NOTIFY columnCountChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

    Q_PROPERTY(int preset READ preset WRITE setPreset NOTIFY presetChanged)

public:
    HWGuideLine(QQuickItem* parent = nullptr);

    // Q_ENUM(GLType)

    /*void classBegin() override;
    void componentComplete() override;*/

    void drawGuideLine();
    void updateGuideLine() override;

signals:
    void layoutChanged();

    void typeChanged();
    void spacingPxChanged();
    void lineHeightPxChanged();
    void horizontalPaddingPxChanged();
    void topPaddingPxChanged();
    void bottomPaddingPxChanged();
    void rowCountChanged();
    void columnCountChanged();
    void colorChanged();

    void presetChanged();

/*private slots:
    void resize();*/

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    QObject* toQObject() const override;

protected:
    QSGGeometry* m_geometry;
    QSGFlatColorMaterial* m_material;
};

#endif  // HWSGGUIDELINE_H
