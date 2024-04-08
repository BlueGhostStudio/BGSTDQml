#ifndef BGCOMMONBASE_H
#define BGCOMMONBASE_H

#include <QObject>
#include <QQmlEngine>

#include "bgcontrols.h"
#include "bgrefreshconnection.h"
#include "bgpalette.h"

class BGCommonBase : public QObject {
    Q_OBJECT
public:
    explicit BGCommonBase(QObject* parent = nullptr);

    template <typename T>
    T* otherAttach() {
        return qobject_cast<T*>(
            qmlAttachedPropertiesObject<T>(m_control, true));
    }

signals:

protected:
    QObject* m_control;
    BGControls* m_BGControls;
    BGRefreshConnection* m_refreshConnection;
    BGPalette* m_palette;

    QQmlEngine* m_qmlEngine = nullptr;
};

#endif  // BGCOMMONBASE_H
