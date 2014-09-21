#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "dataobject.h"
#include "observer.h"
#include "modelaudio.h"
#include <QWidget>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtWebKitWidgets/QWebView>
#include <QtQml/QQmlContext>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QTime>
#include <QVariant>
#include <QList>


#include <QtWidgets>

#include <QtMultimedia>


class VkAudio : public QWidget, public Observer::AbstractObserver
{
    Q_OBJECT
public:
    explicit VkAudio(QWidget* parent = nullptr);
    ~VkAudio() override;

    void updateListFriend(const QVector<std::tuple<IdUser, QString, QIcon>>& listFriend) override;
    void updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration>>& infoTrack) override;

signals:
    void progressDownloadValue(int value = 0);
    void mediaPositionChanged(qint64 position, const QString& duration);
    void mediaDurationChanged(qint64 duration);

private slots:
    void checkUrl(const QUrl& url);
    void urlTrack(const QString& id);

private:
    QWebView        *m_authorization        = new QWebView;
    QQuickView      *m_quickView            = new QQuickView;
    QWidget         *m_quickWidget          = nullptr;
    ModelAudio      *m_modelAudio           = new ModelAudio(this);
    QBuffer         *m_bufferTrack          = new QBuffer(this);
    QMediaPlayer    *m_player               = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    QNetworkAccessManager   *m_loadTrack    = nullptr;

    QList<QObject*> m_dataList_;
};

#endif // VKAUDIO_H
