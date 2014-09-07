#include "controller.h"

Controller::Controller(QObject* parent) : QObject(parent)
{
    m_view = qobject_cast<View*>(parent);
    m_view->setVisibleWidgets(false);
}

QUrl Controller::getUrlTrack(int id)
{ return qMove(m_model->findUrlTrack(id)); }

QString Controller::getNameTrack(int id)
{ return qMove(m_model->findNameTrack(id)); }

void Controller::checkUrl(const QUrl& url)
{
    QUrlQuery query(url.toString().replace('#', '&'));
    QString token = query.queryItemValue("access_token");
    if(token.length() == 0)
        return;
    m_view->setVisibleWidgets(true);
    m_model->setUrlAudio(token);
}


