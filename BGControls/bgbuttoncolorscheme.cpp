#include "bgbuttoncolorscheme.h"

#include "bgcoloroperation.h"
#include "bgcontrols.h"

BGButtonColorScheme::BGButtonColorScheme(BGControls* parent)
    : BGColorSchemeBase{ parent } {
    QObject::connect(m_BGControls, &BGControls::backgroundChanged, this,
                     &BGButtonColorScheme::baseChanged);
    QObject::connect(m_BGControls, &BGControls::accentChanged, this, [&]() {
        emit highlightedChanged();
        emit checkedChanged();
    });
}

QColor
BGButtonColorScheme::base() const {
    return BGColor_reverseBrightnessFlipDelta(m_BGControls->background(), 0.1);
}

QColor
BGButtonColorScheme::highlighted() const {
    return m_BGControls->accent();
}

QColor
BGButtonColorScheme::checked() const {
    return m_BGControls->accent();
}

QColor
BGButtonColorScheme::background() const {
    QColor color;

    QColor baseColor = base();

    bool highlighted = m_control->property("highlighted").toBool();

    if (m_control->property("checked").toBool()) {
        QColor checkedColor = this->checked();
        color = highlighted
                    ? BGColor_shade(m_BGControls->commonColorScheme()->isDark(),
                                    checkedColor, 5)
                    : checkedColor;
    } else if (highlighted)
        color = this->highlighted();
    else
        color = baseColor;

    if (m_control->property("hovered").toBool())
        color = BGColor_reverseBrightnessFlipDelta(color, 0.15);
    if (m_control->property("down").toBool())
        color = BGColor_reverseBrightnessFlipDelta(color, 0.15);

    return color;
}

QColor
BGButtonColorScheme::foreground(bool flat) const {
    if (flat)
        return background();
    else
        return BGColor_contrast(m_BGControls->background());
}
