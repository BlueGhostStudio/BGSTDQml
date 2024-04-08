#include "bgswitchcommon.h"

#include <QQmlProperty>

#include "bgcoloroperation.h"

BGSwitchCommon::BGSwitchCommon(QObject* parent) : BGCommonBase{ parent } {
    m_background = m_BGControls->background();
    m_foreground = m_BGControls->foreground();

    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, [=]() {
                         m_background = m_BGControls->background();
                         m_foreground = m_BGControls->foreground();
                         emit stateColorChanged();
                     });

    QQmlProperty(m_control, "hovered")
        .connectNotifySignal(this, SIGNAL(stateColorChanged()));
    QQmlProperty(m_control, "checked")
        .connectNotifySignal(this, SIGNAL(stateColorChanged()));
    QQmlProperty(m_control, "down")
        .connectNotifySignal(this, SIGNAL(stateColorChanged()));
}

BGSwitchCommon*
BGSwitchCommon::qmlAttachedProperties(QObject* obj) {
    return new BGSwitchCommon(obj);
}

QColor
BGSwitchCommon::stateColor() const {
    QColor bgColor = m_BGControls->background();
    QColor fgColor = BGColor_contrast(bgColor);

    return m_palette->mixColor(
        "button/VAR_FLAT", m_BGControls, false,
        QQmlProperty::read(m_control, "checked").toBool(),
        QQmlProperty::read(m_control, "hovered").toBool(),
        QQmlProperty::read(m_control, "down").toBool(),
        BGPalette::PALETTE_UNSET, fgColor, bgColor);
}
