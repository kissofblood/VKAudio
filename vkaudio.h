#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "propertymodelaudio.h"
#include "propertymodelfriend.h"
#include "avatarprovider.h"
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
#include <QIcon>
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

    void updateListFriend(const QVector<std::tuple<IdUser, QString, QPixmap>>& listFriend) override;
    void updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration>>& infoTrack) override;

signals:
    void progressDownloadValue(int value = 0);
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

private:
    QWebView        *m_authorization            = new QWebView;
    QQuickView      *m_quickView                = new QQuickView;
    QWidget         *m_quickWidget              = nullptr;
    ModelAudio      *m_modelAudio               = new ModelAudio(this);
    QBuffer         *m_bufferTrack              = new QBuffer(this);
    QMediaPlayer    *m_player                   = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    AvatarProvider  *m_avatar                   = nullptr;
    QNetworkAccessManager       *m_loadTrack    = nullptr;
    QList<QObject*> m_propertyModelAudio_;
    QList<QObject*> m_propertyModelFriend_;
    bool m_isRandomTrack = false;
    bool m_isLoopTrack   = false;
};

#endif // VKAUDIO_H
