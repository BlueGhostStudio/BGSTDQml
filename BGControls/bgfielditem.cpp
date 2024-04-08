#include "bgfielditem.h"

#include <QDebug>
#include <QMetaObject>
// #include <QQmlProperty>
#include <QQmlComponent>

BGFieldItem::BGFieldItem(QObject* parent) : QObject{ parent } {}

BGFieldItemAttachedType*
BGFieldItem::qmlAttachedProperties(QObject* obj) {
    return new BGFieldItemAttachedType(obj);
}

void
BGFieldItem::classBegin() {
    m_qmlEngine = qmlEngine(this);
    m_context = new QQmlContext(m_qmlEngine, this);
    m_context->setContextProperty("fieldItem", this);
}

void
BGFieldItem::componentComplete() {}

QString
BGFieldItem::label() const {
    return m_label;
}

void
BGFieldItem::setLabel(const QString& label) {
    if (m_label == label) return;
    m_label = label;
    emit labelChanged();
}

QString
BGFieldItem::field() const {
    return m_field;
}

void
BGFieldItem::setField(const QString& field) {
    if (m_field == field) return;
    m_field = field;
    emit fieldChanged();
}

QQuickItem*
BGFieldItem::control() const {
    return m_control;
}

void
BGFieldItem::setControl(QQuickItem* control) {
    if (m_control == control) return;

    m_control = control;

    BGFieldItemAttachedType* attach = qobject_cast<BGFieldItemAttachedType*>(
        qmlAttachedPropertiesObject<BGFieldItem>(m_control, true));
    attach->setField(this);

    emit controlChanged();
}

QJSValue
BGFieldItem::value() const {
    if (m_value.isUndefined()) {
        QVariant theValue;
        bool ok = QMetaObject::invokeMethod(m_control, "fieldValue",
                                            qReturnArg(theValue));

        if (ok)
            return m_qmlEngine->toScriptValue(theValue);
        else
            return m_value;
    } else
        return m_value;
}

void
BGFieldItem::setValue(const QJSValue& value) {
    m_value = value;
    emit valueChanged();
}

QJSValue
BGFieldItem::initialValue() const {
    return m_initialValue;
}

void
BGFieldItem::setInitialValue(const QJSValue& value) {
    m_initialValue = value;
    emit initialValueChanged();
}

Qt::Orientation
BGFieldItem::orientation() const {
    return m_orientation;
}

void
BGFieldItem::setOrientation(Qt::Orientation orientation) {
    if (m_orientation == orientation) return;
    m_orientation = orientation;
    emit orientationChanged();
}

bool
BGFieldItem::fillWidth() const {
    return m_fillWidth;
}

void
BGFieldItem::setFillWidth(bool fillWidth) {
    if (m_fillWidth == fillWidth) return;
    m_fillWidth = fillWidth;
    emit fillWidthChanged();
}

QQuickItem*
BGFieldItem::labelCtrl() {
    if (!m_labelCtrl) {
        QQmlComponent component(m_qmlEngine);
        component.setData(R"QML(
import QtQuick
import BGStudio.Controls

Text {
    text: fieldItem.label
    horizontalAlignment: fieldItem.orientation === Qt.Horizontal ? Text.AlignRight : Text.AlignLeft
}
)QML",
                          QUrl());
        m_labelCtrl = qobject_cast<QQuickItem*>(component.create(m_context));
    }
    return m_labelCtrl;
}

void
BGFieldItem::setLabelCtrl(QQuickItem* labelCtrl) {
    if (m_labelCtrl != labelCtrl) {
        if (m_labelCtrl) m_labelCtrl->deleteLater();

        m_labelCtrl = labelCtrl;
        emit labelCtrlChanged();
    }
}

QQuickItem*
BGFieldItem::controlWrapper() {
    if (!m_controlWrapper) {
        QQmlComponent component(m_qmlEngine);
        component.setData(R"QML(
import QtQuick
import QtQuick.Layouts
import BGStudio.Controls

Item {
    //implicitWidth: fieldItem.control.implicitWidth
    implicitHeight: fieldItem.control.implicitHeight
    Layout.fillWidth: true

    Component.onCompleted: {
        fieldItem.control.parent = this
    }
}
)QML", QUrl());

        m_controlWrapper = qobject_cast<QQuickItem*>(component.create(m_context));
        m_control->setParentItem(m_controlWrapper);
    }

    return m_controlWrapper;
}

void
BGFieldItem::setControlWrapper(QQuickItem* controlWrapper) {
    if (m_controlWrapper != controlWrapper) {
        if (m_controlWrapper) m_controlWrapper->deleteLater();

        m_controlWrapper = controlWrapper;
        emit controlWrapperChanged();
    }
}
