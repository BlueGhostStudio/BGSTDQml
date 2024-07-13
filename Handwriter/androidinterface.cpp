#include "androidinterface.h"

#include <QCoreApplication>
#include <QJniObject>

AndroidInterface::AndroidInterface(QObject* parent) : QObject{ parent } {}

int
AndroidInterface::screenOrientation() const {
    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    if (activity.isValid())
        return activity.callMethod<int>("getRequestedOrientation");
    else
        return -1;
}

void
AndroidInterface::setScreenOrientation(int orientation) {
    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    if (activity.isValid()) {
        activity.callMethod<void>("setRequestedOrientation", "(I)V",
                                  orientation);

        emit screenOrientationChanged();
    }
}

void
AndroidInterface::setFullSreen() {
    /*if (productVersion() >= 9)
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]() {
            QJniObject activity =
                QNativeInterface::QAndroidApplication::context();
            QJniObject win = activity.callObjectMethod(
                "getWindow", "()Landroid/view/Window;");

            QJniObject wm_lp = win.callObjectMethod(
                "getAttributes", "()Landroid/view/WindowManager$LayoutParams;");
            wm_lp.setField("layoutInDisplayCutoutMode", 1);

            // win.callMethod<void>("setDecorFitsSystemWindows", "(Z)V", false);
            win.callMethod<void>("addFlags", "(I)V", 0x00000200);

            QJniObject windowInsetsController =
                win.callObjectMethod("getDecorView", "()Landroid/view/View;")
                    .callObjectMethod(
                        "getWindowInsetsController",
                        "()Landroid/view/WindowInsetsController;");
            windowInsetsController.callMethod<void>("hide", "(I)V", 0x03);
            windowInsetsController.callMethod<void>("setSystemBarsBehavior",
                                                    "(I)V", 0x02);
        }).waitForFinished();*/
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([=]() {
        QJniObject activity =
            QNativeInterface::QAndroidApplication::context();
        QJniObject win = activity.callObjectMethod(
            "getWindow", "()Landroid/view/Window;");
        QJniObject decorView = win.callObjectMethod("getDecorView", "()Landroid/view/View;");

        if (productVersion() >= 10) {
            QJniObject windowInsetsController = decorView.callObjectMethod(
                "getWindowInsetsController",
                "()Landroid/view/WindowInsetsController;");
            windowInsetsController.callMethod<void>("hide", "(I)V", 0x03);
            windowInsetsController.callMethod<void>("setSystemBarsBehavior",
                                                    "(I)V", 0x02);
        } else {
            // win.callMethod<void>("addFlags", "(I)V", 0x00000200);
            decorView.callMethod<void>(
                "setSystemUiVisibility", "(I)V",
                0x00000002        // View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | 0x00000004  // View.SYSTEM_UI_FLAG_FULLSCREEN
                    | 0x00001000  // View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            );
        }

        if (productVersion() >= 9) {
            QJniObject wm_lp = win.callObjectMethod(
                "getAttributes", "()Landroid/view/WindowManager$LayoutParams;");
            wm_lp.setField("layoutInDisplayCutoutMode", 1);
            // win.callMethod<void>("setDecorFitsSystemWindows", "(Z)V", false);
        }

    }).waitForFinished();
}

QRect
AndroidInterface::cutout_boundingRectTop() const {
    return cutout_boundingRect("getBoundingRectTop");
}

QRect
AndroidInterface::cutout_boundingRectBottom() const {
    return cutout_boundingRect("getBoundingRectBottom");
}

QRect
AndroidInterface::cutout_boundingRectLeft() const {
    return cutout_boundingRect("getBoundingRectLeft");
}

QRect
AndroidInterface::cutout_boundingRectRight() const {
    return cutout_boundingRect("getBoundingRectRight");
}

QRect
AndroidInterface::cutout_boundingRect(const char* method) const {
    qDebug() << "~~ ~~" << productVersion();
    if (productVersion() >= 9) {
        QJniObject activity = QNativeInterface::QAndroidApplication::context();

        if (activity.isValid()) {
            QJniObject cutout =
                activity
                    .callObjectMethod("getWindow", "()Landroid/view/Window;")
                    .callObjectMethod("getDecorView", "()Landroid/view/View;")
                    .callObjectMethod("getRootWindowInsets",
                                      "()Landroid/view/WindowInsets;")
                    .callObjectMethod("getDisplayCutout",
                                      "()Landroid/view/DisplayCutout;");

            QJniObject rect =
                cutout.callObjectMethod(method, "()Landroid/graphics/Rect;");

            int x = rect.getField<int>("left");
            int width = rect.getField<int>("right") - x;
            int y = rect.getField<int>("top");
            int height = rect.getField<int>("bottom");

            return QRect(x, y, width, height);
        } else
            return QRect();
    } else
        return QRect();
}

qreal
AndroidInterface::productVersion() const {
    return QSysInfo::productVersion().toDouble();
}

/*void
AndroidInterface::testCutout() {
    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    if (activity.isValid()) {
        QJniObject cutout =
            activity.callObjectMethod("getWindow", "()Landroid/view/Window;")
                .callObjectMethod("getDecorView", "()Landroid/view/View;")
                .callObjectMethod("getRootWindowInsets",
                                  "()Landroid/view/WindowInsets;")
                .callObjectMethod("getDisplayCutout",
                                  "()Landroid/view/DisplayCutout;");
        qDebug() << "in test>>>>"
                 << cutout.callMethod<int>("getSafeInsetTop", "()I")
                 << cutout.callMethod<int>("getSafeInsetLeft", "()I")
                 << cutout.callMethod<int>("getSafeInsetRight", "()I");
    }
}*/
