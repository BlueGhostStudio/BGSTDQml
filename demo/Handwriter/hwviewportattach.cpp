#include "hwviewportattach.h"

HWViewportAttach::HWViewportAttach(QObject *parent) : QObject{ parent } {}

HWViewportAttach*
HWViewportAttach::qmlAttachedProperties(QObject* obj) {
    HWViewportAttach* viewport = new HWViewportAttach(obj);

    return viewport;
}

QObject*
HWViewportAttach::viewport() const {
    qDebug() << "=====" << parent() << parent()->parent();

    return parent();
}
