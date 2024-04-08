#ifndef BGCOLORSCHEMEBASE_H
#define BGCOLORSCHEMEBASE_H

#include <QObject>
#include <QQuickItem>

class BGControls;
class BGRefreshConnection;

class BGColorSchemeBase : public QObject {
    Q_OBJECT
public:
    explicit BGColorSchemeBase(BGControls* parent = nullptr);

signals:

protected:
    //QQmlEngine* m_qmlEngine = nullptr;
    BGRefreshConnection* m_refreshConnection = nullptr;

    BGControls* m_BGControls = nullptr;
    QQuickItem* m_control = nullptr;
};

#endif  // BGCOLORSCHEMEBASE_H
