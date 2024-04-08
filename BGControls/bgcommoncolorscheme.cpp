#include "bgcommoncolorscheme.h"

#include "bgcoloroperation.h"
#include "bgcontrols.h"

BGCommonColorScheme::BGCommonColorScheme(BGControls* parent)
    : BGColorSchemeBase{ parent } {
    QObject::connect(m_BGControls, &BGControls::themeChanged, this,
                     &BGCommonColorScheme::isDarkChanged);

    QObject::connect(m_BGControls, &BGControls::accentChanged, this,
                     &BGCommonColorScheme::focusChanged);
    QObject::connect(m_BGControls, &BGControls::backgroundChanged, this,
                     &BGCommonColorScheme::unfocusChanged);

    if (m_control)
        QObject::connect(m_control, &QQuickItem::opacityChanged, this,
                         &BGCommonColorScheme::opacityChanged);
}

bool
BGCommonColorScheme::isDark() const {
    return m_BGControls->theme() == BGControls::DARK;
}

QColor
BGCommonColorScheme::focus() const {
    return m_BGControls->accent();
}

QColor
BGCommonColorScheme::unfocus() const {
    return BGColor_reverseBrightnessFlipDelta(m_BGControls->background(), 0.25);
}

qreal
BGCommonColorScheme::opacity() const {
    return qobject_cast<QQuickItem*>(m_control)->isEnabled() ? 1 : 0.3;
}
