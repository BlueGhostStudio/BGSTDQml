#ifndef BGFIELDITEM_H
#define BGFIELDITEM_H

#include <QJSValue>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlParserStatus>
#include <QQuickItem>

#include "bgfielditemattachedtype.h"

class BGFieldItem : public QObject, public QQmlParserStatus {
    Q_OBJECT
    QML_NAMED_ELEMENT(FieldItem)
    QML_ATTACHED(BGFieldItemAttachedType)
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString field READ field WRITE setField NOTIFY fieldChanged)
    Q_PROPERTY(
        QQuickItem* control READ control WRITE setControl NOTIFY controlChanged)
    Q_PROPERTY(QJSValue value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QJSValue initialValue READ initialValue WRITE setInitialValue
                   NOTIFY initialValueChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation
                   NOTIFY orientationChanged)
    Q_PROPERTY(bool fillWidth READ fillWidth WRITE setFillWidth NOTIFY
                   fillWidthChanged)

    Q_PROPERTY(QQuickItem* labelCtrl READ labelCtrl WRITE setLabelCtrl NOTIFY
                   labelCtrlChanged)
    Q_PROPERTY(QQuickItem* controlWrapper READ controlWrapper WRITE
                   setControlWrapper NOTIFY controlWrapperChanged)

    Q_CLASSINFO("DefaultProperty", "control")

public:
    explicit BGFieldItem(QObject* parent = nullptr);

    static BGFieldItemAttachedType* qmlAttachedProperties(QObject* obj);

    void classBegin() override;
    void componentComplete() override;

    QString label() const;
    void setLabel(const QString& label);

    QString field() const;
    void setField(const QString& field);

    QQuickItem* control() const;
    void setControl(QQuickItem* control);

    QJSValue value() const;
    void setValue(const QJSValue& value);

    QJSValue initialValue() const;
    void setInitialValue(const QJSValue& value);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    bool fillWidth() const;
    void setFillWidth(bool fillWidth);

    QQuickItem* labelCtrl();
    void setLabelCtrl(QQuickItem* labelCtrl);

    QQuickItem* controlWrapper();
    void setControlWrapper(QQuickItem* controlWrapper);

signals:
    void labelChanged();
    void fieldChanged();
    void controlChanged();
    void valueChanged();
    void initialValueChanged();
    void orientationChanged();
    void fillWidthChanged();

    void labelCtrlChanged();
    void controlWrapperChanged();

private:
    QString m_label;
    QString m_field;
    QQuickItem* m_control = nullptr;
    QJSValue m_value;
    QJSValue m_initialValue;
    Qt::Orientation m_orientation = Qt::Horizontal;
    bool m_fillWidth = true;

    QQmlEngine* m_qmlEngine = nullptr;
    QQmlContext* m_context = nullptr;

    QQuickItem* m_labelCtrl = nullptr;
    QQuickItem* m_controlWrapper = nullptr;
};

#endif  // BGFIELDITEM_H
