#ifndef BGPALETTEVALUE_H
#define BGPALETTEVALUE_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlParserStatus>
#include <QQmlProperty>
#include <QQmlPropertyValueSource>
// #include <QQuickItem>

#include <QJSValue>

#include "bgcontrols.h"
#include "bgpalette.h"
#include "bgrefreshconnection.h"

class BGPaletteValue : public QObject,
                       public QQmlParserStatus,
                       public QQmlPropertyValueSource {
    Q_OBJECT
    QML_ELEMENT
    Q_INTERFACES(QQmlParserStatus QQmlPropertyValueSource)

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QObject* targetControl READ targetControl WRITE setTargetControl
                   NOTIFY targetControlChanged)
    Q_PROPERTY(QString paletteProperty READ paletteProperty WRITE
                   setPaletteProperty NOTIFY palettePropertyChanged)
    Q_PROPERTY(StateMasks stateMask READ stateMask WRITE setStateMask NOTIFY
                   stateMaskChanged)
    Q_PROPERTY(
        QString section READ section WRITE setSection NOTIFY sectionChanged)
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY
                   baseColorChanged)
    Q_PROPERTY(QJSValue callback READ callback WRITE setCallback NOTIFY
                   callbackChanged)

public:
    explicit BGPaletteValue(QObject* parent = nullptr);

    // clang-format off
    enum StateMask {
        STM_NONE = 0,
             STM_BACKGROUND = 0B0000000001,
              STM_HIGHLIGHT = 0B0000000010,
        STM_FORCE_HIGHLIGHT = 0B0000000100,
                STM_HOVERED = 0B0000001000,
          STM_FORCE_HOVERED = 0B0000010000,
                 STM_SHADOW = 0B0000100000,
           STM_FORCE_SHADOW = 0B0001000000,
                  STM_FOCUS = 0B0010000000,
            STM_FORCE_FOCUS = 0B0100000000,
          STM_FORCE_UNFOCUS = 0B1000000000,
        STM_ALL = STM_BACKGROUND | STM_HIGHLIGHT | STM_HOVERED | STM_SHADOW |
                  STM_FOCUS,
        STM_ALL_EXCEPT_FOCUS = STM_ALL & ~STM_FOCUS
    };
    // clang-format on

    Q_ENUM(StateMask)
    Q_DECLARE_FLAGS(StateMasks, StateMask)
    Q_FLAG(StateMasks)

    void classBegin() override;
    void componentComplete() override;
    void setTarget(const QQmlProperty& prop) override;

    bool enabled() const;
    void setEnabled(bool enabled);

    QObject* targetControl() const;
    void setTargetControl(QObject* targetControl);

    QString paletteProperty() const;
    void setPaletteProperty(const QString& paletteProperty);

    StateMasks stateMask() const;
    void setStateMask(const StateMasks& newStateMask);

    QString section() const;
    void setSection(const QString& newSection);

    QColor baseColor() const;
    void setBaseColor(const QColor& baseColor);

    QJSValue callback() const;
    void setCallback(const QJSValue& newCallback);

    Q_INVOKABLE QColor colorAdjustment(const QString& cmp, const QString& rule,
                                       const QColor& color = QColor(),
                                       const QColor& baseColor = QColor());

signals:
    void enabledChanged();
    void stateMaskChanged();
    void sectionChanged();
    void baseColorChanged();
    void targetControlChanged();
    void palettePropertyChanged();
    void callbackChanged();

private slots:
    void updateProperty();

private:
    QQmlProperty m_targetProperty;

    QString m_paletteProperty = "color";
    StateMasks m_stateMask = STM_ALL;
    QString m_section = "default";
    QColor m_baseColor;
    BGPalette* m_palette;

    QQmlEngine* m_qmlEngine = nullptr;

    bool m_enabled = true;
    QObject* m_control = nullptr;
    BGControls* m_BGControls = nullptr;
    BGRefreshConnection* m_refreshConnection = nullptr;
    QJSValue m_callback;
};

#endif  // BGPALETTEVALUE_H
