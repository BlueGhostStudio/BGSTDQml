#include "hwviewportattachedtype.h"

#include "hwviewport.h"

HWViewportAttachedType::HWViewportAttachedType(QObject* parent)
    : QObject{ parent } {}

HWViewport*
HWViewportAttachedType::viewport() const {
    QObject* vp = parent()->parent();
    QString HWVPClassName("HWViewport");
    while (vp && vp->metaObject()->className() != HWVPClassName) {
        vp = vp->parent();
    }

    return qobject_cast<HWViewport*>(vp);
}
