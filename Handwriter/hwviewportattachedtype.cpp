#include "hwviewportattachedtype.h"

#include <QMetaClassInfo>

#include "hwviewport.h"

HWViewportAttachedType::HWViewportAttachedType(QObject* parent)
    : QObject{ parent } {
    if (parent->isQuickItemType())
        QObject::connect(qobject_cast<QQuickItem*>(parent),
                         &QQuickItem::parentChanged, this,
                         [=]() { emit viewportChanged(); });
}

HWViewport*
HWViewportAttachedType::viewport() const {
    /*QObject* vp = parent()->parent();
    QString HWVPClassName("HWViewport");
    while (vp && vp->metaObject()->className() != HWVPClassName) {
        vp = vp->parent();
    }

    return qobject_cast<HWViewport*>(vp);*/

    auto getParentObj = [](QObject* obj) -> QObject* {
        QObject* parent = obj->parent();
        if (!parent) {
            QQuickItem* item = qobject_cast<QQuickItem*>(obj);
            if (item) parent = item->parentItem();
        }

        return parent;
    };

    auto checkViewport = [](QObject* obj) -> bool {
        if (!obj)
            return false;
        else {
            int cii =
                obj->metaObject()->indexOfClassInfo(  // cii: classInfoIndex
                    "HWQmlBaseType");
            return cii != -1 && obj->metaObject()->classInfo(cii).value() ==
                                    QString("HWViewport");
        }
    };

    QObject* vp = getParentObj(parent());

    while (vp && !checkViewport(vp)) {
        vp = getParentObj(vp);
    }

    return qobject_cast<HWViewport*>(vp);
}
