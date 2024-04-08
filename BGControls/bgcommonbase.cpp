#include "bgcommonbase.h"

BGCommonBase::BGCommonBase(QObject* parent) : QObject{ parent },
                                              m_control(parent){
    m_qmlEngine = qmlEngine(m_control);

    m_BGControls = qobject_cast<BGControls*>(
        qmlAttachedPropertiesObject<BGControls>(m_control, true));

    m_refreshConnection = m_qmlEngine->singletonInstance<BGRefreshConnection*>(
        "BGStudio.Controls", "BGRefreshConnection");

    m_palette = m_qmlEngine->singletonInstance<BGPalette*>(
        "BGStudio.Controls", "BGPalette");
}
