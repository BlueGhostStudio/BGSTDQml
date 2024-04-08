#ifndef BGBUTTONCOMMON_H
#define BGBUTTONCOMMON_H

#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlProperty>
#include <QQuickItem>

#include "bgcommonbase.h"

class BGButtonCommon : public BGCommonBase {
    Q_OBJECT
    QML_NAMED_ELEMENT(BGButton)
    QML_ATTACHED(BGButtonCommon)

    Q_PROPERTY(QQuickItem* contentItem READ contentItem CONSTANT)
    Q_PROPERTY(QColor itemColor READ itemColor NOTIFY itemColorChanged)
    Q_PROPERTY(QColor background READ background NOTIFY backgroundChanged)
    Q_PROPERTY(Qt::Alignment labelAlignment READ labelAlignment WRITE
                   setLabelAlignment NOTIFY labelAlignmentChanged)

    Q_PROPERTY(bool isDelegate READ isDelegate WRITE setIsDelegate NOTIFY
                   isDelegateChanged)

public:
    explicit BGButtonCommon(QObject* parent = nullptr);

    static BGButtonCommon* qmlAttachedProperties(QObject* obj);

    QQuickItem* contentItem() const;

    QColor itemColor() const;

    Q_INVOKABLE QColor generateBackground(bool highlighted, bool hovered, bool checked,
                      bool down) const;
    QColor background() const;

    bool isDelegate() const;
    void setIsDelegate(bool delegate);

    Qt::Alignment labelAlignment() const;
    void setLabelAlignment(const Qt::Alignment& labelAlignment);

signals:
    void itemColorChanged();
    void backgroundChanged();
    void isDelegateChanged();
    void labelAlignmentChanged();

private:
    QQmlContext* m_controlContext;

    //BGPaletteCommon* m_palette;

    QQuickItem* m_contentItem = nullptr;

    bool m_isDelegate = false;
    Qt::Alignment m_labelAlignment = Qt::AlignHCenter;
};

#endif  // BGBUTTONCOMMON_H
