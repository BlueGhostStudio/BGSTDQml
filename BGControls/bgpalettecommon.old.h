#ifndef BGPALETTECOMMON_H
#define BGPALETTECOMMON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>

#include "bgcommonbase.h"

/*#include "bgcontrols.h"
#include "bgrefreshconnection.h"*/

class BGPaletteCommon : public BGCommonBase {
    Q_OBJECT
    QML_NAMED_ELEMENT(BGPalette)
    QML_ATTACHED(BGPaletteCommon)

    Q_PROPERTY(bool isDark READ isDark NOTIFY isDarkChanged)
    Q_PROPERTY(QColor btnBaseColor READ btnBaseColor NOTIFY btnBaseColorChanged)
    Q_PROPERTY(QColor btnHighlightedColor READ btnHighlightedColor NOTIFY
                   btnHighlightedColorChanged)
    Q_PROPERTY(QColor btnCheckedColor READ btnCheckedColor NOTIFY
                   btnCheckedColorChanged)
    Q_PROPERTY(
        qreal enabledOpacity READ enabledOpacity NOTIFY enabledOpacityChanged)
    Q_PROPERTY(QColor focusColor READ focusColor NOTIFY focusColorChanged)
    Q_PROPERTY(QColor unfocusColor READ unfocusColor NOTIFY unfocusColorChanged)

    Q_PROPERTY(StateMasks stateMask READ stateMask WRITE setStateMask NOTIFY
                   stateMaskChanged)
    Q_PROPERTY(
        QString section READ section WRITE setSection NOTIFY sectionChanged)
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY
                   baseColorChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)

public:
    explicit BGPaletteCommon(QObject* parent = nullptr);

    enum Focus { UNSET, UNFOCUS, FOCUS };
    Q_ENUM(Focus)

    enum StateMask {
        STM_NONE = 0,
        STM_BACKGROUND = 0B00000001,
        STM_HIGHLIGHT = 0B00000010,
        STM_HOVERED = 0B00000100,
        STM_SHADOW = 0B00001000,
        STM_FOCUS = 0B00010000,
        STM_ALL = STM_BACKGROUND | STM_HIGHLIGHT | STM_HOVERED | STM_SHADOW |
                  STM_FOCUS,
        STM_ALL_EXCEPT_FOCUS = STM_ALL & ~STM_FOCUS
    };
    Q_ENUM(StateMask)
    Q_DECLARE_FLAGS(StateMasks, StateMask)
    Q_FLAG(StateMasks)

    static BGPaletteCommon* qmlAttachedProperties(QObject* obj);

    bool isDark() const;

    QColor btnBaseColor() const;
    QColor btnHighlightedColor() const;
    QColor btnCheckedColor() const;

    qreal enabledOpacity() const;
    QColor focusColor() const;

    QColor unfocusColor() const;

    StateMasks stateMask() const;
    void setStateMask(const StateMasks& stateMask);

    QString section() const;
    void setSection(const QString& section);

    QColor baseColor() const;
    void setBaseColor(const QColor& baseColor);

    QColor color() const;

    Q_INVOKABLE QColor btnColor(const QColor& baseColor, bool highlighted,
                                bool hovered, bool checked, bool down);
    Q_INVOKABLE QColor btnFGColor(bool flat, const QColor& bgColor,
                                  bool highlighted, bool hovered, bool checked,
                                  bool down);

    Q_INVOKABLE QColor mixColor(const QString& cmp, bool background = false,
                                bool highlight = false, bool hover = false,
                                bool shadow = false, Focus focus = Focus::UNSET,
                                const QColor& baseColor = QColor(),
                                const QColor& refBGColor = QColor()) const;

signals:
    void isDarkChanged();
    void btnBaseColorChanged();
    void btnHighlightedColorChanged();
    void btnCheckedColorChanged();
    void enabledOpacityChanged();
    void focusColorChanged();
    void unfocusColorChanged();

    void stateMaskChanged();
    void sectionChanged();
    void baseColorChanged();

    void colorChanged();

private:
    QJsonObject selectCmpRule(const QString& cmp) const;

    QColor colorAdjustment(const QJsonObject& rule, const QColor& color,
                           const QColor& baseColor = QColor()) const;

private:
    QJsonDocument m_paletteRules;
    QJsonObject m_defaultRule;

    StateMasks m_stateMask = STM_ALL;
    QString m_section = "default";
    QColor m_baseColor;
};

#endif  // BGPALETTECOMMON_H
