#ifndef BGSWITCHCOMMON_H
#define BGSWITCHCOMMON_H

#include <QObject>
#include <QQmlEngine>

#include "bgcommonbase.h"

class BGSwitchCommon : public BGCommonBase {
    Q_OBJECT
    QML_NAMED_ELEMENT(BGSwitch)
    QML_ATTACHED(BGSwitchCommon)

    Q_PROPERTY(QColor stateColor READ stateColor NOTIFY stateColorChanged)

public:
    explicit BGSwitchCommon(QObject* parent = nullptr);

    static BGSwitchCommon* qmlAttachedProperties(QObject* obj);

    QColor stateColor() const;

signals:
    void stateColorChanged();

private:
    QColor m_background;
    QColor m_foreground;
};

#endif  // BGSWITCHCOMMON_H
