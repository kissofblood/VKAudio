#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "dataobject.h"
#include "observer.h"
#include "modelaudio.h"
#include <QWidget>
#include <QtQuick/QQuickView>
#include <QtWebKitWidgets/QWebView>
#include <QtQml/QQmlContext>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QTime>
#include <QVariant>
#include <QList>

#include <QtWidgets>

class VkAudio : public QWidget, public Observer::AbstractObserver
{
    Q_OBJECT
public:
    explicit VkAudio(QWidget* parent = nullptr);
    ~VkAudio() override;

    void updateListFriend(const QVector<std::tuple<IdUser, QString, QIcon>>& listFriend) override;
    void updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration>>& infoTrack) override;

private slots:
    void checkUrl(const QUrl& url);

private:
    QWebView        *m_authorization    = new QWebView;
    QQuickView      *m_quickView        = new QQuickView;
    QWidget         *m_quickWidget      = nullptr;
    ModelAudio      *m_modelAudio       = ModelAudio::getInstance();
};

#endif // VKAUDIO_H
