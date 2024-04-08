#include "bgcontrols.h"

#include <QDebug>
#include <QJSValueIterator>
#include <QQmlContext>
#include <QQmlEngine>

#include "bgcoloroperation.h"

QColor defaultPrimary = QColor::fromString("#1985A1");
QColor defaultAccent = QColor::fromString("#f89b19");
QColor defaultBase(Qt::white);

BGControls::BGControls(QObject* parent) : QObject(parent) {
    m_qmlEngine = qmlEngine(parent);
    m_refreshConnection = m_qmlEngine->singletonInstance<BGRefreshConnection*>(
        "BGStudio.Controls", "BGRefreshConnection");

    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGControls::backgroundChanged);
    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGControls::darkChanged);
    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGControls::foregroundChanged);
    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGControls::accentChanged);
    QObject::connect(m_refreshConnection, &BGRefreshConnection::refreshStarted,
                     this, &BGControls::primaryChanged);
}

BGControls::~BGControls() {}

BGControls*
BGControls::qmlAttachedProperties(QObject* obj) {
    BGControls* controls = new BGControls(obj);

    return controls;
}

QObject*
BGControls::parentContainer(QObject* obj) const {
    auto getParentObj = [](QObject* obj) -> QObject* {
        QObject* parent = obj->parent();
        if (!parent) {
            QQuickItem* item = qobject_cast<QQuickItem*>(obj);
            if (item) parent = item->parentItem();
        }

        return parent;
        /*QQuickItem* item = qobject_cast<QQuickItem*>(obj);
        QObject* parent = nullptr;
        if (item) parent = item->parentItem();

        return parent ? parent : obj->parent();*/
    };

    QObject* item = obj ? getParentObj(obj) : getParentObj(parent());

    while (item) {
        if (item->property("isContainer").toBool()) break;

        item = getParentObj(item);
    }

    return item;
}

QObject*
BGControls::backtrackContainer(const QJSValue& cb) const {
    if (cb.isCallable()) {
        return backtrackContainer(
            [&](QObject* obj, BGControls* control) -> bool {
                return cb
                    .call(QJSValueList{ m_qmlEngine->toScriptValue(obj),
                                        m_qmlEngine->toScriptValue(control) })
                    .toBool();
            });
    } else
        return nullptr;
}

bool
BGControls::isContainer() const {
    return parent()->property("isContainer").toBool();
}

void
BGControls::setIsContainer(bool is) {
    parent()->setProperty("isContainer", is);
    emit isContainerChanged();
}

QQuickItem*
BGControls::contentItem() const {
    return m_contentItem;
}

void
BGControls::setContentItem(QQuickItem* contentItem) {
    if (!isContainer()) {
        m_contentItem = nullptr;
        return;
    } else {
        QQuickItem* origContentItem =
            parent()->property("contentItem").value<QQuickItem*>();

        QQuickItem* parentItem = contentItem;
        QQuickItem* subContentItem = nullptr;
        while ((subContentItem =
                    parentItem->property("contentItem").value<QQuickItem*>())) {
            parentItem = subContentItem;
        }

        foreach (QQuickItem* item, origContentItem->childItems()) {
            item->setParentItem(parentItem);
        }
        parent()->setProperty("contentItem",
                              QVariant::fromValue<QQuickItem*>(contentItem));

        if (m_contentItem) m_contentItem->deleteLater();

        m_contentItem = contentItem;

        emit contentItemChanged();
    }
}

QJSValue
BGControls::options() const {
    return m_options;
}

void
BGControls::setOptions(const QJSValue& data) {
    if (m_options.isObject() || m_options.isQObject()) {
        QJSValueIterator it(data);
        while (it.hasNext()) {
            it.next();
            m_options.setProperty(it.name(), it.value());
        }
    } else
        m_options = data;
    // m_data = data;
    emit optionsChanged();
}

BGControls::Theme
BGControls::theme() const {
    Theme _theme = Theme::NORMAL;
    backtrackContainer([&](QObject*, BGControls* control) -> bool {
        if (control && control->m_theme != Theme::NOTHEME) {
            _theme = control->m_theme;
            return true;
        } else
            return false;
    });

    return _theme;
}

void
BGControls::setTheme(Theme theme) {
    if (m_theme == theme) return;
    m_theme = theme;
    emit themeChanged();
    emit m_refreshConnection->refreshStarted();
}

bool
BGControls::dark() const {
    /*BGControls::Theme theTheme = theme();
    float baseLightness = base().lightnessF();
    return (theTheme == Theme::NORMAL && baseLightness < 0.5) ||
           (theTheme == Theme::BRIGHTNESS_INVERSION && baseLightness >= 0.5);*/
    return background().lightnessF() < 0.5;
}

void
BGControls::setDark(bool dark) {
    bool isDark = BGControls::dark();
    if (isDark != dark)
        m_theme = Theme::BRIGHTNESS_INVERSION;
    else
        m_theme = Theme::NORMAL;

    emit darkChanged();
    emit m_refreshConnection->refreshStarted();
}

QColor
BGControls::primary() const {
    return backtrackColor(ColorRoles::PRIMARY);
    // return m_primary;
}

void
BGControls::setPrimary(const QColor& color) {
    if (m_primary == color) return;
    m_primary = color;

    emit primaryChanged();
    emit m_refreshConnection->refreshStarted();
}

QColor
BGControls::accent() const {
    return backtrackColor(ColorRoles::ACCENT);
}

void
BGControls::setAccent(const QColor& color) {
    if (m_accent == color) return;
    m_accent = color;

    emit accentChanged();
    emit m_refreshConnection->refreshStarted();
}

QColor
BGControls::base() const {
    return backtrackColor(ColorRoles::BASE);
}

void
BGControls::setBase(const QColor& color) {
    if (m_base == color) return;
    m_base = color;

    emit baseChanged();
    emit m_refreshConnection->refreshStarted();
}

int
BGControls::shade() const {
    int nShade = 0;
    backtrackContainer([&](QObject*, BGControls* control) -> bool {
        if (control && control->m_shade != -1) {
            nShade = control->m_shade;

            return true;
        } else
            return false;
    });

    return nShade;
}

void
BGControls::setShade(int shade) {
    if (m_shade == shade) return;
    m_shade = shade;

    emit shadeChanged();
    emit m_refreshConnection->refreshStarted();
}

int
BGControls::hue() const {
    int nHue = 0;
    backtrackContainer([&](QObject*, BGControls* control) -> bool {
        if (control && control->m_hue != -1) {
            nHue = control->m_hue;

            return true;
        } else
            return false;
    });

    return nHue;
}

void
BGControls::setHue(int hue) {
    if (m_hue == hue) return;
    m_hue = hue;

    emit hueChanged();
    emit m_refreshConnection->refreshStarted();
}

bool
BGControls::covered() const {
    return m_covered;
}

void
BGControls::setCovered(bool covered) {
    if (m_covered == covered) return;
    m_covered = covered;

    emit coveredChanged();
    emit m_refreshConnection->refreshStarted();
}

QColor
BGControls::foreground() const {
    return BGColor_contrast(background());
}

QColor
BGControls::background() const {
    return backtrackColor(ColorRoles::NOROLE);
}

BGControls::ColorRoles
BGControls::role() const {
    ColorRoles _role = ColorRoles::BASE;
    backtrackContainer([&](QObject*, BGControls* control) -> bool {
        if (control && control->m_role != ColorRoles::NOROLE) {
            _role = control->m_role;
            return true;
        } else
            return false;
    });

    return _role;
}

void
BGControls::setRole(ColorRoles role) {
    if (m_role == role) return;
    m_role = role;

    emit roleChanged();
    emit m_refreshConnection->refreshStarted();
}

QObject*
BGControls::backtrackContainer(
    std::function<bool(QObject*, BGControls*)> callback) const {
    QObject* item = parent();  // parentContainer(parent());
    while (item) {
        if (callback(item,
                     qobject_cast<BGControls*>(
                         qmlAttachedPropertiesObject<BGControls>(item, false))))
            return item;

        item = parentContainer(item);
    }

    return nullptr;
}

QColor
BGControls::backtrackColor(ColorRoles target) const {
    int fb = 0;
    int shade = 0;
    int hue = 0;
    bool covered = false;
    Theme theme = Theme::NORMAL;
    ColorRoles role = ColorRoles::BASE;
    QColor qcPrimary(defaultPrimary);
    QColor qcAccent(defaultAccent);
    QColor qcBase(defaultBase);

    bool isFirst = true;
    backtrackContainer([&](QObject*, BGControls* control) -> bool {
        if (control) {
            if (isFirst && control->m_covered) covered = true;

            isFirst = false;

            if (!(fb & 0b0000001) && control->m_theme != Theme::NOTHEME) {
                theme = control->m_theme;
                fb |= 0b0000001;
            }
            if (!(fb & 0b0000010) && control->m_role != ColorRoles::NOROLE) {
                role = control->m_role;
                fb |= 0b0000010;
            }
            if (!(fb & 0b00100) && control->m_primary.isValid()) {
                qcPrimary = control->m_primary;
                fb |= 0b00100;
            }
            if (!(fb & 0b01000) && control->m_accent.isValid()) {
                qcAccent = control->m_accent;
                fb |= 0b01000;
            }
            if (!(fb & 0b10000) && control->m_base.isValid()) {
                qcBase = control->m_base;
                fb |= 0b10000;
            }

            if (control->m_shade != 0XFFFF) {
                // if (control->m_shade <= 10 && control->m_shade >= -10)
                shade = qBound(-100, shade + control->m_shade, 100);
            }
            if (control->m_hue != 0XFFFF) {
                //hue = qBound(-360, hue + control->m_hue, 360);
                hue = (hue + control->m_hue) % 360;
            }

            // return fb == check;
        } /*else
            return false;*/

        return false;
    });

    QColor targetColor;

    switch (target == ColorRoles::NOROLE ? role : target) {
    case ColorRoles::BASE:
        targetColor = qcBase;
        break;
    case ColorRoles::PRIMARY:
        targetColor = qcPrimary;
        break;
    case ColorRoles::ACCENT:
        targetColor = qcAccent;
        break;
    default:
        break;
    }

    if (theme == Theme::BRIGHTNESS_INVERSION)
        targetColor = BGColor_brightnessInversion(targetColor);

    if (shade != 0XFFFF)
        targetColor = BGColor_shade(theme == BRIGHTNESS_INVERSION, targetColor,
                                    shade, 100);
    if (hue != 0XFFFF)
        targetColor =
            BGColor_spin(targetColor, hue);  // hueColor(bgColor, hue);

    if (covered) targetColor = targetColor.darker(110);

    return targetColor;
}
