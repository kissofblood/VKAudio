#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"
#include "model.h"
#include <QObject>

#include <QtWidgets>
#include <QtXml>

#include <QtNetwork>

class View;

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject* parent = nullptr);
    ~Controller() override = default;

    QUrl getUrlTrack(int id);
    QString getNameTrack(int id);

public slots:
    void checkUrl(const QUrl& url);

private:
    View            *m_view     = nullptr;
    Model           *m_model    = Model::getInstance();
};

#endif // CONTROLLER_H
