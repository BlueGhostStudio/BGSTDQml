#include "bgbuttoncommon.h"

#include <QDebug>
#include <QQmlComponent>

#include "bgcoloroperation.h"

BGButtonCommon::BGButtonCommon(QObject* parent) : BGCommonBase{ parent } {
    m_controlContext = new QQmlContext(m_qmlEngine, m_control);
    m_controlContext->setContextProperty("control", m_control);
    m_controlContext->setContextProperty("bgButton", this);

    // m_palette = otherAttach<BGPaletteCommon>();

    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGButtonCommon::itemColorChanged);
    QObject::connect(m_BGControls, &BGControls::forceRefresh, this,
                     &BGButtonCommon::itemColorChanged);

    QQmlComponent component(m_qmlEngine);
    component.setData(R"QML(
import QtQuick
import BGStudio.Controls

IconLabel {
    id: iconLabel

    BGText.textScale: BGText.Button
    font.pointSize: BGText.font.pointSize
    font.letterSpacing: BGText.font.letterSpacing / 100 - 1
    font.weight: BGText.font.weight
    font.capitalization: BGText.font.capitalization
    font.family: control.font.family
    text: control.text || ""
    color: bgButton.itemColor

    spacing: control.spacing
    mirrored: control.mirrored
    display: control.display || AbstractButton.TextBesideIcon

    alignment: bgButton.labelAlignment

    Component.onCompleted: {
        icon = Qt.binding(() => { return control.icon })
        icon.color = Qt.binding(() => { return bgButton.itemColor })
    }
}
)QML",
                      QUrl());

    m_contentItem =
        qobject_cast<QQuickItem*>(component.create(m_controlContext));

    QQmlProperty(m_control, "flat")
        .connectNotifySignal(this, SIGNAL(itemColorChanged()));
    QQmlProperty(m_control, "highlighted")
        .connectNotifySignal(this, SIGNAL(itemColorChanged()));
    QQmlProperty(m_control, "hovered")
        .connectNotifySignal(this, SIGNAL(itemColorChanged()));
    QQmlProperty(m_control, "checked")
        .connectNotifySignal(this, SIGNAL(itemColorChanged()));
    QQmlProperty(m_control, "down")
        .connectNotifySignal(this, SIGNAL(itemColorChanged()));

    QObject::connect(this, &BGButtonCommon::itemColorChanged, this,
                     &BGButtonCommon::backgroundChanged);
}

BGButtonCommon*
BGButtonCommon::qmlAttachedProperties(QObject* obj) {
    return new BGButtonCommon(obj);
}

QQuickItem*
BGButtonCommon::contentItem() const {
    return m_contentItem;
}

/*void
BGButtonCommon::onItemColorChanged() {
    qDebug() << "item mixColor changed";
}*/

QColor
BGButtonCommon::itemColor() const {
    bool flat = QQmlProperty::read(m_control, "flat").toBool();
    bool highlighted = QQmlProperty::read(m_control, "highlighted").toBool();
    bool hovered = QQmlProperty::read(m_control, "hovered").toBool();
    bool checked = QQmlProperty::read(m_control, "checked").toBool();
    bool down = QQmlProperty::read(m_control, "down").toBool();

    QColor baseBG = generateBackground(highlighted, hovered, checked, down);
    QColor baseFG = BGColor_contrast(baseBG);

    if (m_isDelegate) {
        if (hovered || highlighted)
            return baseFG;
        else
            return m_palette->mixColor("button/VAR_FLAT", m_BGControls, false, checked,
                                       false, false, BGPalette::PALETTE_UNSET,
                                       baseFG, baseBG);
    } else if (flat)
        return m_palette->mixColor("button/VAR_FLAT", m_BGControls, false,
                                   highlighted || checked, hovered, down,
                                   BGPalette::PALETTE_UNSET, baseFG, baseBG);
    else
        return baseFG;
}

QColor
BGButtonCommon::generateBackground(bool highlighted, bool hovered, bool checked,
                                   bool down) const {
    bool flat = QQmlProperty::read(m_control, "flat").toBool();

    if (flat || (m_isDelegate && !hovered && !highlighted))
        return m_BGControls->background();
    else
        return m_palette->mixColor(
            "button", m_BGControls, true, highlighted || checked, hovered, down,
            BGPalette::PALETTE_UNSET, m_BGControls->background());
}

QColor
BGButtonCommon::background() const {
    bool highlighted = QQmlProperty::read(m_control, "highlighted").toBool();
    bool hovered = QQmlProperty::read(m_control, "hovered").toBool();
    bool checked = QQmlProperty::read(m_control, "checked").toBool();
    bool down = QQmlProperty::read(m_control, "down").toBool();

    return generateBackground(highlighted, hovered, checked, down);
}

bool
BGButtonCommon::isDelegate() const {
    return m_isDelegate;
}

void
BGButtonCommon::setIsDelegate(bool delegate) {
    if (m_isDelegate == delegate) return;
    m_isDelegate = delegate;
    emit isDelegateChanged();
}

Qt::Alignment
BGButtonCommon::labelAlignment() const {
    return m_labelAlignment;
}

void
BGButtonCommon::setLabelAlignment(const Qt::Alignment& labelAlignment) {
    if (m_labelAlignment == labelAlignment) return;
    m_labelAlignment = labelAlignment;
    emit labelAlignmentChanged();
}
