#include "pch.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "backend.h"
#include "applistmodel.h"
#include "kheventfilter.h"

HWND g_hwnd;

int main(int argc, char* argv[]) 
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<BackEnd>("mf.components", 1, 0, "BackEnd", &BackEnd::qmlInstance);
    qmlRegisterType<AppListModel>("mf.components", 1, 0, "AppListModel");

    engine.addImportPath(":/imports");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if(engine.rootObjects().size() == 0)
        throw std::exception("Unable to cast window");

    auto obj = engine.rootObjects().at(0);
    QQuickWindow* wnd;
    if( !(wnd = qobject_cast<QQuickWindow*>(obj) ))
        throw std::exception("Unable to cast window");

    app.installNativeEventFilter(new KhEventFilter());
    g_hwnd = (HWND)wnd->winId();

    return app.exec();
}
