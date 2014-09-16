#include <QApplication>
#include <QtQml/QQmlApplicationEngine>

#include <QtWidgets>
#include <QtQuick>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/mainInterface.qml"));
    QObject* topLevel = engine.rootObjects().front();
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);
    window->show();

    return a.exec();
}
