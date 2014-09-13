#ifndef MODELAUDIO_H
#define MODELAUDIO_H

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

class ModelAudio : public QObject, public Observer::AbstractObservable
{
    Q_OBJECT

    using InfoTrack = QPair<bool, std::tuple<Artist, Title, Duration, QUrl>>;
public:
    ModelAudio(const ModelAudio&) = delete;
    ~ModelAudio() override;
    ModelAudio& operator =(const ModelAudio&) = delete;

    static ModelAudio* getInstance();
    QUrl findUrlTrack(const QString& id);
    int getNextIdTrack(int id);
    int getRandomIdTrack(int id);
    void findPlaylist(const QString& token);
    void registerObserver(Observer::AbstractObserver* observer) override;
    void removeObserver(Observer::AbstractObserver* observer) override;
    void notifyAudioObservers() override;
    void notifyFriendObservers() override;

signals:
    void loadTrue();

public slots:
    void getPlaylistMy();
    void getPlaylistFriend(const QString& id);

private slots:
    void parserAudio(QNetworkReply* reply);
    void parserFriend(QNetworkReply* reply);
    void parserUser(QNetworkReply* reply);

private:
    static ModelAudio                       *m_singleton;
    QNetworkAccessManager                   *m_loadFriend   = new QNetworkAccessManager;
    QNetworkAccessManager                   *m_loadAudio    = new QNetworkAccessManager;
    QVector<QNetworkAccessManager*>         m_loadUser_;
    QVector<QNetworkAccessManager*>         m_loadIcon_;
    QVector<Observer::AbstractObserver*>    m_observer_;
    QVector<InfoTrack>                              m_vecInfoTrack_;
    QHash<IdTrack, QVector<InfoTrack>::iterator>    m_hashInfoTrack_;
    QHash<IdUser, QPair<QString, QIcon>> m_infoFriend_;
    QPair<IdUser, QPair<QString, QIcon>> m_infoMy;
    QString m_token;
    int m_countFriend = 0;

    ModelAudio();

    QPair<IdUser, QPair<QString, QIcon>> getResultParserUser(const QByteArray& array);
};

#endif // MODELAUDIO_H
