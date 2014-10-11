#ifndef MODELAUDIO_H
#define MODELAUDIO_H

#include "observer.h"
#include <QObject>
#include <QPair>
#include <QUrlQuery>
#include <QUrl>
#include <QString>
#include <QVector>
#include <QHash>
#include <QByteArray>
#include <QEventLoop>
#include <QPixmap>
#include <QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <tuple>
#include <functional>
#include <algorithm>


#include <QtWidgets>
#include <QtNetwork>

class ModelAudio : public QObject, public Observer::AbstractObservable
{
    Q_OBJECT

    struct InfoTrack
    {
        Artist      artist;
        Title       title;
        Duration    duration;
        QUrl        url;
        IdUser      ownerId;
    };
public:
    enum class StateTrack { Show, Hide, Remove };
    explicit ModelAudio(QObject* parent = nullptr);
    ~ModelAudio() override = default;

    QUrl findUrlTrack(const QString& id);
    QString getNextIdTrack(const QString& id);
    QString getPrevIdTrack(const QString& id);
    QString getRandomIdTrack();
    std::tuple<IdUser, QString, QUrl> getInfoMy() const;
    void setStateTrack(const QString& id, StateTrack state);
    void findPlaylist(const QString& token);
    void registerObserver(Observer::AbstractObserver* observer) override;
    void removeObserver(Observer::AbstractObserver* observer) override;
    void notifyAudioObservers() override;
    void notifyFriendObservers() override;
    void deleteTrack(const QString& trackId, const QString& userId);
    void uploadServerTrack(QFile* data);

signals:
    void progressDownload(qint64 value);

public slots:
    void getPlaylistMy();
    void getPlaylistFriend(const QString& id);
    void addTrack(const QString& trackId, const QString& userId);
    void globalSearchAudio(const QString& artist);
    void getRecommended(const QString& idUser);
    void getPopular(const QString& id);

private slots:
    void parserAudio(QNetworkReply* reply);
    void parserFriend(QNetworkReply* reply);
    void parserUser(QNetworkReply* reply);

private:
    QVector<Observer::AbstractObserver*>            m_observer_;
    QVector<QPair<StateTrack, InfoTrack>>           m_vecInfoTrack_;
    QHash<IdTrack, QVector<QPair<StateTrack, InfoTrack>>::iterator> m_hashInfoTrack_;
    QHash<IdUser, QPair<QString, QUrl>> m_infoFriend_;
    QPair<IdUser, QPair<QString, QUrl>> m_infoMy;
    QString m_token;
    int     m_countFriend = 0;

    QPair<IdUser, QPair<QString, QUrl>> getResultParserUser(const QByteArray& array);
    QUrl makeWorkUrl(const QString& url);
};

#endif // MODELAUDIO_H
