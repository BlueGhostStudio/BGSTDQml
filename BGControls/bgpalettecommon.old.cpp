#include "bgpalettecommon.h"

#include <QQmlProperty>

#include "bgcoloroperation.h"

BGPaletteCommon::BGPaletteCommon(QObject* parent) : BGCommonBase{ parent } {
    m_paletteRules = QJsonDocument::fromJson(R"JSON(
{
    "default": {
        "background": {},
        "highlight": {},
        "hover": { "brightnessInversion": 0.15 },
        "shadow": { "brightnessInversion": 0.15 },
        "focus": {},
        "unfocus": { "brightnessInversion": 0.25 }
    },"button": {
        "background": { "brightnessInversion": 0.1 }
    }, "text": {
        "highlight": { "contrast": { "delta": 0.5 } }
    }, "input": {
        "background": { "lighter": 0.2 },
        "text": {
            "highlight": {}
        }
    }, "panel": {
        "background": {
            "shade": 5
        }, "title": {
            "background": { "opacity": 0.1 }
        }
    }, "popup": {
        "background": {
            "shade": 5
        }, "title": {
            "background": { "opacity": 0.1 }
        }
    }
}
)JSON");
    m_defaultRule = m_paletteRules.object().value("default").toObject();

    if (m_refreshConnection) {
        QObject::connect(m_refreshConnection,
                         &BGRefreshConnection::refreshStarted, this, [=]() {
                             emit isDarkChanged();
                             emit btnBaseColorChanged();
                             emit btnHighlightedColorChanged();
                             emit btnCheckedColorChanged();
                             emit focusColorChanged();
                             emit unfocusColorChanged();
                             emit colorChanged();
                         });
    }

    QQmlProperty(m_control, "highlighted")
        .connectNotifySignal(this, SIGNAL(colorChanged()));
    QQmlProperty(m_control, "hovered")
        .connectNotifySignal(this, SIGNAL(colorChanged()));
    QQmlProperty(m_control, "checked")
        .connectNotifySignal(this, SIGNAL(colorChanged()));
    QQmlProperty(m_control, "down")
        .connectNotifySignal(this, SIGNAL(colorChanged()));
    QQmlProperty(m_control, "pressed")
        .connectNotifySignal(this, SIGNAL(colorChanged()));

    QObject::connect(this, &BGPaletteCommon::stateMaskChanged, this,
                     &BGPaletteCommon::colorChanged);
    QObject::connect(this, &BGPaletteCommon::sectionChanged, this,
                     &BGPaletteCommon::colorChanged);

    QQuickItem* controlItem = qobject_cast<QQuickItem*>(m_control);

    if (controlItem) {
        QObject::connect(controlItem, &QQuickItem::enabledChanged, this,
                         &BGPaletteCommon::enabledOpacityChanged);
    }
}

BGPaletteCommon*
BGPaletteCommon::qmlAttachedProperties(QObject* obj) {
    return new BGPaletteCommon(obj);
}

bool
BGPaletteCommon::isDark() const {
    return m_BGControls ? m_BGControls->dark() : false;
}

QColor
BGPaletteCommon::btnBaseColor() const {
    return BGColor_reverseBrightnessFlipDelta(m_BGControls->background(), 0.1);
}

QColor
BGPaletteCommon::btnHighlightedColor() const {
    return m_BGControls->accent();
}

QColor
BGPaletteCommon::btnCheckedColor() const {
    return m_BGControls->accent();
}

qreal
BGPaletteCommon::enabledOpacity() const {
    QQuickItem* contentItem = qobject_cast<QQuickItem*>(m_control);
    return contentItem->isEnabled() ? 1 : 0.3;
}

QColor
BGPaletteCommon::focusColor() const {
    return m_BGControls->accent();
}

QColor
BGPaletteCommon::unfocusColor() const {
    return BGColor_reverseBrightnessFlipDelta(m_BGControls->background(), 0.25);
}

BGPaletteCommon::StateMasks
BGPaletteCommon::stateMask() const {
    return m_stateMask;
}

void
BGPaletteCommon::setStateMask(const StateMasks& stateMask) {
    if (m_stateMask == stateMask) return;
    m_stateMask = stateMask;
    emit stateMaskChanged();
}

QString
BGPaletteCommon::section() const {
    return m_section;
}

void
BGPaletteCommon::setSection(const QString& section) {
    if (m_section == section) return;
    m_section = section;
    emit sectionChanged();
}

QColor
BGPaletteCommon::baseColor() const {
    return m_baseColor;
}

void
BGPaletteCommon::setBaseColor(const QColor& baseColor) {
    if (m_baseColor == baseColor) return;
    m_baseColor = baseColor;
    emit baseColorChanged();
}

QColor
BGPaletteCommon::color() const {
    if (m_BGControls->isContainer()) return m_BGControls->background();

    bool background = m_stateMask & STM_BACKGROUND;
    bool highlight = (m_stateMask & STM_HIGHLIGHT) &&
                     (QQmlProperty::read(m_control, "highlighted").toBool() ||
                      QQmlProperty::read(m_control, "checked").toBool());

    bool hover = (m_stateMask & STM_HOVERED) &&
                 QQmlProperty::read(m_control, "hovered").toBool();
    bool shadow = (m_stateMask & STM_SHADOW) &&
                  (QQmlProperty::read(m_control, "down").toBool() ||
                   QQmlProperty::read(m_control, "pressed").toBool());
    Focus focus = UNSET;
    if (m_stateMask & STM_FOCUS) {
        if (QQmlProperty::read(m_control, "activeFocus").toBool())
            focus = FOCUS;
        else
            focus = UNFOCUS;
    }

    return mixColor(m_section, background, highlight, hover, shadow, focus,
                    m_baseColor, m_BGControls->background());
}

QColor
BGPaletteCommon::btnColor(const QColor& baseColor, bool highlighted,
                          bool hovered, bool checked, bool down) {
    QColor color;
    if (checked)
        color = highlighted ? BGColor_shade(isDark(), btnCheckedColor(), 5)
                            : btnCheckedColor();
    else if (highlighted)
        color = btnHighlightedColor();
    else
        color = baseColor;

    if (hovered) color = BGColor_reverseBrightnessFlipDelta(color, 0.15);

    if (down) color = BGColor_reverseBrightnessFlipDelta(color, 0.15);

    return color;
}

QColor
BGPaletteCommon::btnFGColor(bool flat, const QColor& bgColor, bool highlighted,
                            bool hovered, bool checked, bool down) {
    if (flat)
        return btnColor(m_BGControls->foreground(), highlighted, hovered,
                        checked, down);
    else
        return BGColor_contrast(bgColor);
}

QColor
BGPaletteCommon::mixColor(const QString& cmp, bool background, bool highlight,
                          bool hover, bool shadow, Focus focus,
                          const QColor& baseColor,
                          const QColor& refBGColor) const {
    if (m_BGControls->isContainer()) return m_BGControls->background();

    QJsonObject rule = selectCmpRule(cmp);

    const QColor theBaseColor(baseColor.isValid() ? baseColor
                                                  : m_BGControls->background());
    QColor theAdjColor;

    auto getRule = [&](const QString& aRule) -> QJsonObject {
        QJsonObject::const_iterator it = rule.constFind(aRule);
        return (it != rule.constEnd() ? it.value() : m_defaultRule.value(aRule))
            .toObject();
    };

    if (highlight || focus == FOCUS)
        theAdjColor = m_BGControls->accent();
    else if (background)
        theAdjColor =
            colorAdjustment(getRule("background"), theBaseColor, theBaseColor);
    else
        theAdjColor = theBaseColor;

    if (focus == Focus::FOCUS)
        theAdjColor =
            colorAdjustment(getRule("focus"), theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    else if (focus == Focus::UNFOCUS)
        theAdjColor =
            colorAdjustment(getRule("unfocus"), theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);

    if (highlight)
        theAdjColor =
            colorAdjustment(getRule("highlight"), theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    if (hover)
        theAdjColor =
            colorAdjustment(getRule("hover"), theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    if (shadow)
        theAdjColor =
            colorAdjustment(getRule("shadow"), theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);

    return theAdjColor;
}

QJsonObject
BGPaletteCommon::selectCmpRule(const QString& cmp) const {
    QJsonObject jsoRule = m_paletteRules.object();
    foreach (const QString& item, cmp.split('/')) {
        auto it = jsoRule.constFind(item);
        if (it != jsoRule.constEnd())
            jsoRule = it.value().toObject();
        else {
            jsoRule = m_defaultRule;
            break;
        }
    }

    return jsoRule;
}

QColor
BGPaletteCommon::colorAdjustment(const QJsonObject& rule, const QColor& color,
                                 const QColor& baseColor) const {
    QColor theAdjColor(std::move(color));

    if (rule.isEmpty()) return theAdjColor;

    QJsonObject::const_iterator it;
    for (it = rule.constBegin(); it != rule.constEnd(); ++it) {
        QJsonValue jsvAdjValue = it.value();

        if (it.key() == "spin")
            theAdjColor = BGColor_spin(theAdjColor, jsvAdjValue.toDouble());

        else if (it.key() == "shade")
            theAdjColor = BGColor_shade(
                m_BGControls->theme() == BGControls::BRIGHTNESS_INVERSION,
                it.value().toDouble(0), 100);

        else if (it.key() == "brightnessInversion") {
            if (jsvAdjValue.isBool()) {
                if (jsvAdjValue.toBool())
                    theAdjColor = BGColor_brightnessInversion(theAdjColor);
            } else if (jsvAdjValue.isDouble())
                theAdjColor = BGColor_reverseBrightnessFlipDelta(
                    theAdjColor, jsvAdjValue.toDouble());
        }

        else if (it.key() == "lighter")
            theAdjColor = theAdjColor.lighter(jsvAdjValue.toInt(100));

        else if (it.key() == "darker")
            theAdjColor = theAdjColor.darker(jsvAdjValue.toInt(100));

        else if (it.key() == "contrast") {
            if (jsvAdjValue.isBool()) {
                if (jsvAdjValue.toBool())
                    theAdjColor = BGColor_contrast(theAdjColor);
            } else if (jsvAdjValue.isObject()) {
                QJsonObject jsoContrast = jsvAdjValue.toObject();
                if (jsoContrast.contains("threshold"))
                    theAdjColor = BGColor_contrast(
                        theAdjColor, QColor(Qt::black), QColor(Qt::white),
                        jsoContrast.value("threshold").toDouble(0.5));
                else if (jsoContrast.contains("delta")) {
                    theAdjColor = BGColor_contrast2Color(
                        baseColor, theAdjColor,
                        jsoContrast.value("delta").toDouble(0.5));
                }
            }
        }
    }

    return theAdjColor;
}
