#include "bgcolorschemebase.h"

#include <QQmlEngine>

#include "bgcontrols.h"
#include "bgrefreshconnection.h"

BGColorSchemeBase::BGColorSchemeBase(BGControls* parent) : QObject{ parent } {
    m_BGControls = parent;
    m_control = qobject_cast<QQuickItem*>(m_BGControls->parent());

    QQmlEngine* theQmlEngine = qmlEngine(m_control);
    m_refreshConnection = theQmlEngine->singletonInstance<BGRefreshConnection*>(
        "BGStudio.Controls", "BGRefreshConnection");
}
