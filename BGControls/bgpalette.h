#ifndef BGPALETTE_H
#define BGPALETTE_H

#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>

#include "bgcontrols.h"
// #include "bgrefreshconnection.h"

class BGPalette : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(
        QJsonDocument rules READ rules WRITE setRules NOTIFY rulesChanged)
public:
    explicit BGPalette(QObject* parent);
    static BGPalette* create(QQmlEngine* qmlEngine, QJSEngine*);

    enum Focus { PALETTE_UNSET, PALETTE_UNFOCUS, PALETTE_FOCUS };
    Q_ENUM(Focus)

    Q_INVOKABLE QJsonObject selectCmpRule(const QString& cmp) const;
    Q_INVOKABLE QJsonObject selectRule(const QString& cmp,
                                      const QString& rule) const;

    Q_INVOKABLE QColor mixColor(const QString& cmp, BGControls* BGControls,
                                bool background = false, bool highlight = false,
                                bool hover = false, bool shadow = false,
                                Focus focus = Focus::PALETTE_UNSET,
                                const QColor& baseColor = QColor(),
                                const QColor& refBGColor = QColor()) const;

    Q_INVOKABLE QColor colorAdjustment(BGControls* BGControls,
                                       const QJsonObject& rule,
                                       const QColor& color,
                                       const QColor& baseColor = QColor());

    Q_INVOKABLE qreal enabledOpacity(const QString& cmp,
                                     BGControls* BGControls);

    QJsonDocument rules() const;
    void setRules(const QJsonDocument& rules);

signals:
    void rulesChanged();

private:
    QColor colorAdjustment(const QJsonObject& rule, bool brightnessInversion,
                           const QColor& color, const QColor& baseColor) const;

private:
    // BGRefreshConnection* m_refreshConnection;
    QJsonDocument m_paletteRules;
    QJsonObject m_defaultRule;
};

#endif  // BGPALETTE_H
