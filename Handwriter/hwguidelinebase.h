#ifndef HWGUIDELINEBASE_H
#define HWGUIDELINEBASE_H

#include <QColor>
#include <QObject>
#include <QQuickItem>
#include <QVariant>
#include <QJSValue>

struct t_guideLine {
    int type = 0;
    qreal spacingMm;
    qreal horizontalPadding;
    qreal topPadding;
    qreal bottomPadding;
    qreal lineHeightMm;
    QColor color;
};

class HWSettings;

namespace HWGLType {
Q_NAMESPACE
enum Type { Grid, Line };
Q_ENUM_NS(Type)
}

class HWGuideLineBase {
public:
    HWGuideLineBase(QQuickItem* parent);

    void setLayout(const QJSValue& define);
    QJSValue layout() const;

    HWGLType::Type type() const;

    qreal spacingPx() const;
    qreal lineHeightPx() const;
    qreal horizontalPaddingPx() const;
    qreal topPaddingPx() const;
    qreal bottomPaddingPx() const;
    int rowCount() const;
    int columnCount() const;
    QColor color() const;

    void setPreset(int preset);
    int preset() const;

    virtual void updateGuideLine();

protected:
    virtual QObject* toQObject() const = 0;

protected:
    HWSettings* m_settings = nullptr;

    int m_preset = -1;
    t_guideLine m_guideLineStyle;
    qreal m_spacingPx;
    qreal m_lineHeightPx;
    qreal m_topPaddingPx;
    qreal m_bottomPaddingPx;
    qreal m_horizontalPaddingPx;
    int m_columnCount;
    int m_rowCount;

    QList<t_guideLine> m_guideLinePresets = { { 0,    // type
                                                5,    // spacing
                                                3,    // horizontalPadding
                                                3.5,  // topPadding
                                                4,    // bottomPadding
                                                0,    // lineHeight
                                                QColor(127, 127, 127, 96) },
                                              { 1,   // type
                                                3,   // spacing
                                                10,  // horizontalPadding
                                                10,  // topPadding
                                                15,  // bottomPadding
                                                6,   // lineHeight
                                                QColor(127, 127, 127, 96) } };
};
#endif  // HWGUIDELINEBASE_H
