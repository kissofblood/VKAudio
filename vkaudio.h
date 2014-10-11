#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "propertymodelaudio.h"
#include "propertymodelfriend.h"
#include "observer.h"
#include "modelaudio.h"
#include <QWidget>
#include <QVector>
#include <QString>
#include <QBuffer>
#include <QList>
#include <QObject>
#include <QPair>
#include <QUrl>
#include <QUrlQuery>
#include <QHBoxLayout>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QVariant>
#include <tuple>
#include <algorithm>
#include <functional>
#include <QtWebKitWidgets/QWebView>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtQml/QQmlContext>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtMultimedia/QMediaPlayer>


#include <QtWidgets>

class VkAudio : public QWidget, public Observer::AbstractObserver
{
    Q_OBJECT
public:
    explicit VkAudio(QWidget* parent = nullptr);
    ~VkAudio() override;

    void updateListFriend(const QVector<std::tuple<IdUser, QString, QUrl>>& listFriend) override;
    void updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration, IdUser>>& infoTrack) override;

signals:
    void progressDownloadTrack(qint64 value = 0);
    void mediaPositionChanged(qint64 position, const QString& duration);
    void mediaDurationChanged(qint64 duration);
    void idTrackChanged(bool value, const QString& id);
    void nextTrackDefault();

private slots:
    void checkUrl(const QUrl& url);
    void urlTrack(const QString& id);
    void setPositionPlayer(int position);
    void setNextTrack(const QString& id);
    void setPrevTrack(const QString& id);
    void setLoopTrack(bool value);
    void setRandomTrack(bool value);
    void mediaStatus(QMediaPlayer::MediaStatus status);
    void downloadTrack(const QString& name);
    void filterTrack(const QString& text);
    void pushRemoveTrack(const QString& trackId, const QString& userId, bool remove);
    void deleteAllTrack();
    void openFileForUpload();
    void getPlaylistMyModel();
    void getRecommendedModel(const QString& idUser);
    void getPopularModel(const QString& id);
    void getGlobalSearchAudioModel(const QString& artist);
    void getPlaylistFriendModel(const QString& idUser);

protected:
    Q_INVOKABLE QUrl getUrlAvatarMy() const;
    Q_INVOKABLE QString getIdMy() const;
    Q_INVOKABLE QString getNameAvatarMy() const;

private:
    QWebView        *m_authorization            = new QWebView;
    QQuickView      *m_quickView                = new QQuickView;
    QWidget         *m_quickWidget              = nullptr;
    ModelAudio      *m_modelAudio               = new ModelAudio(this);
    QBuffer         *m_bufferTrack              = new QBuffer(this);
    QMediaPlayer    *m_player                   = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    QNetworkAccessManager   *m_loadTrack        = nullptr;
    QList<QObject*> m_propertyModelAudio_;
    QList<QObject*> m_propertyModelFriend_;
    QHash<QString, QString> m_deleteTrack_;
    bool m_isRandomTrack    = false;
    bool m_isLoopTrack      = false;
    bool m_addAndCancel     = true;
};

#endif // VKAUDIO_H
