#ifndef BGBUTTONCOLORSCHEME_H
#define BGBUTTONCOLORSCHEME_H

#include <QColor>
#include <QObject>
#include <QQmlEngine>

#include "bgcolorschemebase.h"

class BGButtonColorScheme : public BGColorSchemeBase {
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(QColor base READ base NOTIFY baseChanged)
    Q_PROPERTY(QColor highlighted READ highlighted NOTIFY highlightedChanged)
    Q_PROPERTY(QColor checked READ checked NOTIFY checkedChanged)

public:
    explicit BGButtonColorScheme(BGControls* parent = nullptr);

    QColor base() const;
    QColor highlighted() const;
    QColor checked() const;

    Q_INVOKABLE QColor background() const;
    Q_INVOKABLE QColor foreground(bool flat) const;

signals:
    void baseChanged();
    void highlightedChanged();
    void checkedChanged();
};

#endif  // BGBUTTONCOLORSCHEME_H
