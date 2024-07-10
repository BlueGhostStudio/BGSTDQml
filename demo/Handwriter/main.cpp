#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>

int
main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QGuiApplication::setFont(QFont("Noto Sans"));

    if (app.platformName() != "android") {
        QSurfaceFormat format;
        format.setSamples(4);  // 设置抗锯齿级别，例如4x
        QSurfaceFormat::setDefaultFormat(format);
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DemoHandwriter/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    qDebug() << "sceneGraphBackend" << QQuickWindow::sceneGraphBackend();

    return app.exec();
}
