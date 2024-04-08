#include "bgpalette.h"

#include <QQmlProperty>

#include "bgcoloroperation.h"

/*BGPalette::BGPalette(QObject* parent) : QObject{ parent } {
    m_refreshConnection =
        qmlEngine(this)->singletonInstance<BGRefreshConnection*>(
            "BGStudio.Controls", "BGRefreshConnection");

    setRules(QJsonDocument::fromJson(R"JSON(
{
    "default": {
        "background": {},
        "highlight": {},
        "hover": { "brightnessInversion": 0.15 },
        "shadow": { "brightnessInversion": 0.15 },
        "focus": {},
        "unfocus": { "brightnessInversion": 0.25 },
        "disabled": { "opacity": 0.3 }
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
)JSON"));
}*/

BGPalette::BGPalette(QObject* parent) : QObject{ parent } {}

/*BGPalette*
BGPalette::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine) {
    BGPalette* thePalette = new BGPalette(nullptr);

    qDebug() << "in create" << thePalette << qmlEngine
             << qmlEngine->singletonInstance<BGRefreshConnection*>(
                    "BGStudio.Controls", "BGRefreshConnection");

    return thePalette;
}*/

BGPalette*
BGPalette::create(QQmlEngine* qmlEngine, QJSEngine* /*jsEngine*/) {
    BGPalette* thePalette = new BGPalette(nullptr);

    // role: 0-NOROLE, 1-BASE, 2-PRIMARY, 3-ACCENT
    thePalette->setRules(QJsonDocument::fromJson(R"JSON(
{
    "default": {
        "background": {},
        "highlight": {},
        "hover": { "brightnessInversion": 0.15 },
        "shadow": { "brightnessInversion": 0.15 },
        "focus": {},
        "unfocus": { "brightnessInversion": 0.25 },
        "disabled": { "opacity": 0.3 }
    },"button": {
        "background": { "brightnessInversion": 0.1 },
        "VAR_FLAT": {
            "highlight": { "contrast": { "delta": 0.5 } }
        }
    }, "input": {
        "PART_BACKGROUND": {
            "focus": { "lighter": 120 }
        }, "PART_BORDER": {}
    }, "panel": {
        "VAR_NOSHADE": {
            "background": { "shade": 0 }
        }, "VAR_SHADE": {
            "background": { "shade": 5 }
        }, "PART_TITLE": {
            "background": {
                "role": 3
            }, "VAR_NO_BACKGROUND": {
                "background": {
                    "role": 1
                }
            }
        }, "VAR_PRIMARY": {
            "PART_TITLE": {
                "background": {
                    "role": 2
                }
            }
        }, "VAR_POPUP": {
            "background": { "shade": 20 },
            "PART_OVERLAY": {
                "VAR_MODAL": {
                    "background": {
                        "opacity": 0.5,
                        "contrast": { "threshold": 0.5 }
                    }
                }, "VAR_MODELESS": {
                    "background": {
                        "opacity": 0.12,
                        "contrast": { "threshold": 0.5 }
                    }
                }
            }
        }, "VAR_SEPARATOR": {
            "background": {
                "contrast": { "threshold": 0.5 },
                "opacity": 0.25
            }
        }
    }
}
)JSON"));
    /*qDebug() << "in create" << thePalette << qmlEngine
             << qmlEngine->singletonInstance<BGRefreshConnection*>(
                    "BGStudio.Controls", "BGRefreshConnection");*/

    return thePalette;
}

QJsonObject
BGPalette::selectCmpRule(const QString& cmp) const {
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

QJsonObject
BGPalette::selectRule(const QString& cmp, const QString& rule) const {
    QJsonObject jsoCmpRule = m_paletteRules.object();
    QJsonObject jsoRule;

    foreach (const QString& item, cmp.split('/')) {
        auto it = jsoCmpRule.constFind(item);
        if (it != jsoCmpRule.constEnd()) {
            jsoCmpRule = it.value().toObject();
            if (jsoCmpRule.contains(rule)) jsoRule = jsoCmpRule[rule].toObject();
        } else
            break;
    }

    return !jsoRule.isEmpty() ? jsoRule : m_defaultRule[rule].toObject();
}

QColor
BGPalette::mixColor(const QString& cmp, BGControls* BGControls, bool background,
                    bool highlight, bool hover, bool shadow, Focus focus,
                    const QColor& baseColor, const QColor& refBGColor) const {
    const QColor theBaseColor(baseColor.isValid() ? baseColor
                                                  : BGControls->background());

    if (BGControls->isContainer()) {
        QJsonObject ruleObj = selectRule(cmp, "background");
        if (!ruleObj.isEmpty()) {
            if (ruleObj.contains("shade"))
                BGControls->setShade(ruleObj.value("shade").toInt(0));
            /*else
                BGControls->setShade(0);*/

            if (ruleObj.contains("role"))
                BGControls->setRole(
                    (BGControls::ColorRoles)ruleObj.value("role").toInt());
        }

        return BGControls->background();
    }

    QColor theAdjColor;
    bool briInv = BGControls->theme() == BGControls::BRIGHTNESS_INVERSION;

    if (highlight || focus == PALETTE_FOCUS)
        theAdjColor = BGControls->accent();
    else if (background)
        theAdjColor = colorAdjustment(selectRule(cmp, "background"), briInv,
                                      theBaseColor, theBaseColor);
    else
        theAdjColor = theBaseColor;

    if (focus == Focus::PALETTE_FOCUS)
        theAdjColor =
            colorAdjustment(selectRule(cmp, "focus"), briInv, theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    else if (focus == Focus::PALETTE_UNFOCUS)
        theAdjColor =
            colorAdjustment(selectRule(cmp, "unfocus"), briInv, theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);

    if (highlight)
        theAdjColor =
            colorAdjustment(selectRule(cmp, "highlight"), briInv, theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    if (hover)
        theAdjColor =
            colorAdjustment(selectRule(cmp, "hover"), briInv, theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);
    if (shadow)
        theAdjColor =
            colorAdjustment(selectRule(cmp, "shadow"), briInv, theAdjColor,
                            refBGColor.isValid() ? refBGColor : theBaseColor);

    return theAdjColor;
}

QColor
BGPalette::colorAdjustment(BGControls* BGControls, const QJsonObject& rule,
                           const QColor& color, const QColor& baseColor) {
    return colorAdjustment(
        rule, BGControls->theme() == BGControls::BRIGHTNESS_INVERSION, color,
        baseColor.isValid() ? baseColor : BGControls->background());
}

qreal
BGPalette::enabledOpacity(const QString& cmp, BGControls* BGControls) {
    QJsonObject rule = selectRule(cmp, "disabled");

    if (rule.isEmpty()) return 1;

    bool isEnabled =
        QQmlProperty::read(BGControls->parent(), "enabled").toBool();
    return isEnabled ? 1 : rule.value("opacity").toDouble(1);
}

QJsonDocument
BGPalette::rules() const {
    return m_paletteRules;
}

void
BGPalette::setRules(const QJsonDocument& rules) {
    if (m_paletteRules == rules) return;
    m_paletteRules = rules;
    m_defaultRule = m_paletteRules.object().value("default").toObject();

    // emit m_refreshConnection->refreshStarted();
    emit rulesChanged();
}

QColor
BGPalette::colorAdjustment(const QJsonObject& rule, bool brightnessInversion,
                           const QColor& color, const QColor& baseColor) const {
    QColor theAdjColor(std::move(color));

    if (rule.isEmpty()) return theAdjColor;

    QJsonObject::const_iterator it;
    for (it = rule.constBegin(); it != rule.constEnd(); ++it) {
        QJsonValue jsvAdjValue = it.value();

        if (it.key() == "spin")
            theAdjColor = BGColor_spin(theAdjColor, jsvAdjValue.toDouble());

        else if (it.key() == "shade")
            theAdjColor =
                BGColor_shade(brightnessInversion, theAdjColor, it.value().toDouble(0), 100);

        else if (it.key() == "brightnessInversion") {
            if (jsvAdjValue.isBool()) {
                if (jsvAdjValue.toBool())
                    theAdjColor = BGColor_brightnessInversion(theAdjColor);
            } else if (jsvAdjValue.isDouble())
                theAdjColor = BGColor_brightnessInversion(
                    theAdjColor, jsvAdjValue.toDouble());
        }

        else if (it.key() == "lighter")
            theAdjColor = theAdjColor.lighter(jsvAdjValue.toInt(100));

        else if (it.key() == "darker")
            theAdjColor = theAdjColor.darker(jsvAdjValue.toInt(100));

        else if (it.key() == "opacity")
            theAdjColor.setAlphaF(jsvAdjValue.toDouble(1));

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
