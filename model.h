#ifndef MODEL_H
#define MODEL_H

#include "observer.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QUrlQuery>
#include <QUrl>
#include <functional>
#include <algorithm>
#include <tuple>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

#include <QtWidgets>
#include <QtNetwork>
#include <QtXml>

class Model : public Observer::AbstractObservable
{
    using Id        = int;
    using Artist    = QString;
    using Title     = QString;
    using Duration  = int;
public:
    Model(const Model&) = delete;
    ~Model() override;
    Model& operator =(const Model&) = delete;

    static Model* getInstance();
    QUrl findUrlTrack(int id);
    QString findNameTrack(int id);
    int getNextIdTrack(int id);
    int getRandomIdTrack(int id);
    void setUrlAudio(const QString& token);
    void registerObserver(Observer::AbstractObserver* observer) override;
    void removeObserver(Observer::AbstractObserver* observer) override;
    void notifyObservers() override;

private:
    static Model                            *m_singleton;
    QNetworkAccessManager                   *m_manager = new QNetworkAccessManager;
    QVector<Observer::AbstractObserver*>    m_observer_;
    QVector<std::tuple<Id, Artist, Title, Duration, QUrl>> m_infoTrack_;

    Model() = default;

    void parserXml(QNetworkReply* reply);
};

#endif // MODEL_H
