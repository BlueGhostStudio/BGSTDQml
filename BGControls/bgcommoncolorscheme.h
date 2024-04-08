#ifndef BGCOMMONCOLORSCHEME_H
#define BGCOMMONCOLORSCHEME_H

#include <QColor>
#include <QObject>
#include <QQmlEngine>

#include "bgcolorschemebase.h"

class BGCommonColorScheme : public BGColorSchemeBase {
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(bool isDark READ isDark NOTIFY isDarkChanged)
    Q_PROPERTY(QColor focus READ focus NOTIFY focusChanged)
    Q_PROPERTY(QColor unfocus READ unfocus NOTIFY unfocusChanged)
    Q_PROPERTY(qreal opacity READ opacity NOTIFY opacityChanged)

public:
    explicit BGCommonColorScheme(BGControls* parent = nullptr);

    bool isDark() const;
    QColor focus() const;
    QColor unfocus() const;
    qreal opacity() const;

signals:
    void isDarkChanged();
    void focusChanged();
    void unfocusChanged();
    void opacityChanged();
};

#endif  // BGCOMMONCOLORSCHEME_H
