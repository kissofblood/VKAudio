#ifndef VIEW_H
#define VIEW_H

#include "observer.h"
#include "controller.h"
#include "model.h"
#include <QWidget>
#include <tuple>

#include <QtWidgets>

#include <QtWebKitWidgets>

#include <QtMultimedia>
#include <QtNetwork>
#include <QtMultimedia/QMediaContent>
#include <QtMultimedia/QMediaPlayer>


class Controller;

class View : public QWidget, public Observer::AbstractObserver
{
    Q_OBJECT
public:
    explicit View(QWidget* parent = nullptr);
    ~View() override = default;

    void setVisibleWidgets(bool value);
    void updateListTrack(const QVector<std::tuple<Id, Artist, Title, Duration>>& infoTrack) override;

private slots:
    void playTrack(int row, int);
    void downloadTrack();

private:
    QWebView        *m_authentication   = new QWebView;
    QPushButton     *m_download         = new QPushButton("download");
    QPushButton     *m_start            = new QPushButton("start");
    QPushButton     *m_pause            = new QPushButton("pause");
    QPushButton     *m_stop             = new QPushButton("stop");
    QSlider         *m_position         = new QSlider(Qt::Horizontal);
    QSlider         *m_volume           = new QSlider(Qt::Horizontal);
    QLCDNumber      *m_duration         = new QLCDNumber;
    QTableWidget    *m_tableAudio       = new QTableWidget;
    QMediaPlayer    *m_player           = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    QBuffer         *m_trackTmp         = new QBuffer(this);
    QLabel          *m_nameTrack        = new QLabel("artist - title");
    QRadioButton    *m_repeat           = new QRadioButton("repeat");
    QRadioButton    *m_random           = new QRadioButton("random");
    QRadioButton    *m_streak           = new QRadioButton("streak");
    Model           *m_model            = Model::getInstance();
    Controller      *m_controller       = nullptr;
};

#endif // VIEW_H
