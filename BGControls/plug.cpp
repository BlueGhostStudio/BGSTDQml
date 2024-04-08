#include <QtQml/qqmlextensionplugin.h>

extern void qml_register_types_BGStudio_Controls();
Q_GHS_KEEP_REFERENCE(qml_register_types_BGStudio_Controls)

class BGStudio_ControlsPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)

public:
    BGStudio_ControlsPlugin(QObject *parent = nullptr) : QQmlEngineExtensionPlugin(parent)
    {
        volatile auto registration = &qml_register_types_BGStudio_Controls;
        Q_UNUSED(registration)
    }
};
