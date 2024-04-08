#include <qqmlextensionplugin.h>

class BGStudio_ControlsPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)
public:
    void initializeEngine(QQmlEngine *engine, const char *uri) override
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine)
    }

};

#include "bgcontrolsplugin.moc"
