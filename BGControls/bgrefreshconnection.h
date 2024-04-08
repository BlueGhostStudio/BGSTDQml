#ifndef BGREFRESHCONNECTION_H
#define BGREFRESHCONNECTION_H

#include <QObject>
#include <QQmlEngine>

class BGRefreshConnection : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(BGRefreshConnection)
    QML_SINGLETON

public:
    explicit BGRefreshConnection(QObject* parent = nullptr);

signals:
    void refreshStarted();
};

#endif  // BGREFRESHCONNECTION_H
