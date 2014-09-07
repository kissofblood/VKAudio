#ifndef VKAUDIO_H
#define VKAUDIO_H

#include "observer.h"
#include "model.h"
#include <QWidget>
#include <tuple>

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

    void updateListTrack(const QVector<std::tuple<Id, Artist, Title, Duration>>& infoTrack) override;

private slots:
    void checkUrl(const QUrl& url);
    void playTrack(int row, int);
    void downloadTrack();
    void mediaStatus(QMediaPlayer::MediaStatus status);

private:
    Ui::VkAudio     *ui                 = nullptr;
    Model           *m_model            = Model::getInstance();
    QMediaPlayer    *m_player           = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    QBuffer         *m_trackTmp         = new QBuffer(this);
    bool            m_flagRequest       = true;
    int             m_currentIdPlayer   = -1;

    void loadTrack(const QUrl& urlTrack, const QString& nameTrack, int currentId);

    void setVisibleWebView(bool value);
};

#endif // VKAUDIO_H
