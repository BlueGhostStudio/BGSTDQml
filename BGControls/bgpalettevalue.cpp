#include "bgpalettevalue.h"

#include <QDebug>
#include <QQmlContext>

BGPaletteValue::BGPaletteValue(QObject* parent) : QObject{ parent } {}

void
BGPaletteValue::classBegin() {
    m_qmlEngine = qmlEngine(this);
    m_refreshConnection = m_qmlEngine->singletonInstance<BGRefreshConnection*>(
        "BGStudio.Controls", "BGRefreshConnection");

    m_palette = m_qmlEngine->singletonInstance<BGPalette*>("BGStudio.Controls",
                                                           "BGPalette");
}

void
BGPaletteValue::componentComplete() {
    // if (!m_control) m_control = qobject_cast<QQuickItem*>(parent());
    if (!m_control) m_control = parent();

    m_BGControls = qobject_cast<BGControls*>(
        qmlAttachedPropertiesObject<BGControls>(m_control, true));

    QObject::connect(m_BGControls, &BGControls::optionsChanged, this,
                     &BGPaletteValue::updateProperty);
    QObject::connect(m_BGControls, &BGControls::forceRefresh, this,
                     &BGPaletteValue::updateProperty);

    QQmlProperty(m_control, "highlighted")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "hovered")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "checked")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "down")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "pressed")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "activeFocus")
        .connectNotifySignal(this, SLOT(updateProperty()));
    QQmlProperty(m_control, "enabled")
        .connectNotifySignal(this, SLOT(updateProperty()));

    if (m_refreshConnection)
        QObject::connect(m_refreshConnection,
                         &BGRefreshConnection::refreshStarted, this,
                         &BGPaletteValue::updateProperty);

    QObject::connect(this, &BGPaletteValue::stateMaskChanged, this,
                     &BGPaletteValue::updateProperty);
    QObject::connect(this, &BGPaletteValue::sectionChanged, this,
                     &BGPaletteValue::updateProperty);
    QObject::connect(this, &BGPaletteValue::baseColorChanged, this,
                     &BGPaletteValue::updateProperty);
    QObject::connect(this, &BGPaletteValue::palettePropertyChanged, this,
                     &BGPaletteValue::updateProperty);
    QObject::connect(this, &BGPaletteValue::callbackChanged, this,
                     &BGPaletteValue::updateProperty);
}

void
BGPaletteValue::setTarget(const QQmlProperty& prop) {
    QObject* obj = prop.object();

    QByteArray propID = "palette_" + prop.name().toLatin1();

    QVariant varValueSource = obj->property(propID);

    if (!obj->property(propID).isValid())
        obj->setProperty(propID, QVariant::fromValue(this));
    else
        varValueSource.value<BGPaletteValue*>()->setEnabled(false);

    m_targetProperty = prop;
}

QObject*
BGPaletteValue::targetControl() const {
    return m_control;
}

void
BGPaletteValue::setTargetControl(QObject* targetControl) {
    if (m_control) return;
    m_control = targetControl;
    emit targetControlChanged();
}

QString
BGPaletteValue::paletteProperty() const {
    return m_paletteProperty;
}

void
BGPaletteValue::setPaletteProperty(const QString& paletteProperty) {
    if (m_paletteProperty == paletteProperty) return;
    m_paletteProperty = paletteProperty;
    emit palettePropertyChanged();
}

BGPaletteValue::StateMasks
BGPaletteValue::stateMask() const {
    return m_stateMask;
}

void
BGPaletteValue::setStateMask(const StateMasks& newStateMask) {
    if (m_stateMask == newStateMask) return;
    m_stateMask = newStateMask;
    emit stateMaskChanged();
}

QString
BGPaletteValue::section() const {
    return m_section;
}

void
BGPaletteValue::setSection(const QString& newSection) {
    if (m_section == newSection) return;
    m_section = newSection;
    emit sectionChanged();
}

QColor
BGPaletteValue::baseColor() const {
    return m_baseColor;
}

void
BGPaletteValue::setBaseColor(const QColor& baseColor) {
    if (m_baseColor == baseColor) return;
    m_baseColor = baseColor;
    emit baseColorChanged();
}

QJSValue
BGPaletteValue::callback() const {
    return m_callback;
}

void
BGPaletteValue::setCallback(const QJSValue& newCallback) {
    m_callback = newCallback;
    emit callbackChanged();
}

QColor
BGPaletteValue::colorAdjustment(const QString& cmp, const QString& rule,
                                const QColor& color, const QColor& baseColor) {
    QColor background = m_BGControls->background();
    return m_palette->colorAdjustment(
        m_BGControls, m_palette->selectRule(cmp, rule),
        color.isValid() ? color : background,
        baseColor.isValid() ? baseColor : background);
}

void
BGPaletteValue::updateProperty() {
    if (!m_enabled) return;

    if (m_paletteProperty == "color") {
        bool background = m_stateMask & STM_BACKGROUND;
        bool highlight =
            (m_stateMask & STM_FORCE_HIGHLIGHT) ||
            ((m_stateMask & STM_HIGHLIGHT) &&
             (QQmlProperty::read(m_control, "highlighted").toBool() ||
              QQmlProperty::read(m_control, "checked").toBool()));

        bool hover = (m_stateMask & STM_FORCE_HOVERED) ||
                     ((m_stateMask & STM_HOVERED) &&
                      QQmlProperty::read(m_control, "hovered").toBool());
        bool shadow = (m_stateMask & STM_FORCE_SHADOW) ||
                      ((m_stateMask & STM_SHADOW) &&
                       (QQmlProperty::read(m_control, "down").toBool() ||
                        QQmlProperty::read(m_control, "pressed").toBool()));
        BGPalette::Focus focus = BGPalette::PALETTE_UNSET;
        if (m_stateMask & STM_FORCE_FOCUS)
            focus = BGPalette::PALETTE_FOCUS;
        else if (m_stateMask & STM_FORCE_UNFOCUS)
            focus = BGPalette::PALETTE_UNFOCUS;
        else if (m_stateMask & STM_FOCUS) {
            if (QQmlProperty::read(m_control, "activeFocus").toBool())
                focus = BGPalette::PALETTE_FOCUS;
            else
                focus = BGPalette::PALETTE_UNFOCUS;
        }

        m_targetProperty.write(m_palette->mixColor(
            m_section, m_BGControls, background, highlight, hover, shadow,
            focus, m_baseColor, m_BGControls->background()));
    } else if (m_paletteProperty == "enable")
        m_targetProperty.write(
            m_palette->enabledOpacity(m_section, m_BGControls));
    else if (m_paletteProperty == "custom") {
        if (m_callback.isCallable()) {
            QJsonObject rule = m_palette->selectCmpRule(m_section);
            QJSValue result = m_callback.call(
                QJSValueList{ m_qmlEngine->toScriptValue(rule),
                              m_qmlEngine->toScriptValue(m_control) });

            m_targetProperty.write(result.toVariant());
        }
    }
}

bool
BGPaletteValue::enabled() const {
    return m_enabled;
}

void
BGPaletteValue::setEnabled(bool enabled) {
    if (m_enabled == enabled) return;
    m_enabled = enabled;
    emit enabledChanged();
}
