#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "observer.h"
#include "modelaudio.h"
#include <QWidget>
#include <tuple>
#include <QCompleter>

#include <QtWidgets>
#include <QtMultimedia>

namespace Ui {
class VkAudio;
}

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
    void playTrack(int row, int);
    void downloadTrack();
    void mediaStatus(QMediaPlayer::MediaStatus status);
    void filterTableAudio();

private:
    Ui::VkAudio     *ui                 = nullptr;
    ModelAudio      *m_modelAudio       = ModelAudio::getInstance();
    QMediaPlayer    *m_player           = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    QBuffer         *m_trackTmp         = new QBuffer(this);
    QCompleter      *m_completer        = nullptr;
    bool            m_flagRequest       = true;
    int             m_currentIdPlayer   = -1;

    void loadTrack(const QUrl& urlTrack, const QString& nameTrack, int currentId);
    void setVisibleWebView(bool value);
};

#endif // VKAUDIO_H
