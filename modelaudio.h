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

class ModelAudio : public QObject, public Observer::AbstractObservable
{
    Q_OBJECT

    using InfoTrack = QPair<bool, std::tuple<Artist, Title, Duration, QUrl>>;
public:
    explicit ModelAudio(QObject* parent = nullptr);
    ~ModelAudio() override = default;

    QUrl findUrlTrack(const QString& id);
    QString getNextIdTrack(const QString& id);
    QString getPrevIdTrack(const QString& id);
    QString getRandomIdTrack();
    std::tuple<IdUser, QString, QPixmap> getInfoMy() const;
    void setHideTrack(const QString& id, bool value);
    void findPlaylist(const QString& token);
    void globalSearchAudio(const QString& artist);
    void registerObserver(Observer::AbstractObserver* observer) override;
    void removeObserver(Observer::AbstractObserver* observer) override;
    void notifyAudioObservers() override;
    void notifyFriendObservers() override;

signals:
    void progressDownload(int value);

public slots:
    void getPlaylistMy();
    void getPlaylistFriend(const QString& id);

private slots:
    void parserAudio(QNetworkReply* reply);
    void parserFriend(QNetworkReply* reply);
    void parserUser(QNetworkReply* reply);

private:
    QNetworkAccessManager                   *m_loadFriend       = nullptr;
    QNetworkAccessManager                   *m_loadAudio        = nullptr;
    QNetworkAccessManager                   *m_loadGlobalAudio  = nullptr;
    QVector<QNetworkAccessManager*>         m_loadUser_;
    QVector<QNetworkAccessManager*>         m_loadAvatar_;
    QVector<Observer::AbstractObserver*>    m_observer_;
    QVector<InfoTrack>                              m_vecInfoTrack_;
    QHash<IdTrack, QVector<InfoTrack>::iterator>    m_hashInfoTrack_;
    QHash<IdUser, QPair<QString, QPixmap>> m_infoFriend_;
    QPair<IdUser, QPair<QString, QPixmap>> m_infoMy;
    QString m_token;
    int     m_countFriend = 0;

    QPair<IdUser, QPair<QString, QPixmap>> getResultParserUser(const QByteArray& array);
};

#endif // MODELAUDIO_H
