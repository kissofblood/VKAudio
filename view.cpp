#include "view.h"

View::View(QWidget* parent) : QWidget(parent),
    m_controller(new Controller(this))
{
    QHBoxLayout* btnPlayer = new QHBoxLayout;
    btnPlayer->addWidget(m_start);
    btnPlayer->addWidget(m_pause);
    btnPlayer->addWidget(m_stop);

    QHBoxLayout* hBoxPlayer = new QHBoxLayout;
    hBoxPlayer->addItem(btnPlayer);
    hBoxPlayer->addWidget(m_repeat);
    hBoxPlayer->addWidget(m_streak);
    hBoxPlayer->addWidget(m_random);
    hBoxPlayer->addStretch(1);
    hBoxPlayer->addWidget(m_volume);

    QHBoxLayout* durationPlayer = new QHBoxLayout;
    durationPlayer->addWidget(m_position, 3);
    durationPlayer->addWidget(m_duration);

    QVBoxLayout* vBox = new QVBoxLayout;
    vBox->addWidget(m_download);
    vBox->addWidget(m_tableAudio);
    vBox->addItem(durationPlayer);
    vBox->addWidget(m_nameTrack);
    vBox->addItem(hBoxPlayer);

    QHBoxLayout* hBox = new QHBoxLayout;
    hBox->addWidget(m_authentication);
    hBox->addItem(vBox);
    this->setLayout(hBox);
    this->resize(800, 500);

    m_authentication->load(QUrl("https://oauth.vk.com/authorize?"
                               "client_id=4290375"
                               "&scope=audio, ofline"
                               "&redirect_uri=https://oauth.vk.com/blank.html"
                               "&display=mobile"
                               "&v=5.24"
                               "&response_type=token"));
    m_tableAudio->setColumnCount(4);
    m_tableAudio->setHorizontalHeaderLabels({ "artist", "title", "duration" });
    m_tableAudio->setColumnHidden(3, true);
    m_tableAudio->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableAudio->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_volume->setRange(0, 100);
    m_volume->setValue(100);
    m_duration->display(QTime(0, 0, 0).toString("mm:ss"));
    m_duration->setSegmentStyle(QLCDNumber::Flat);
    m_model->registerObserver(dynamic_cast<Observer::AbstractObserver*>(this));

    this->connect(m_authentication, &QWebView::urlChanged, m_controller, &Controller::checkUrl);
    this->connect(m_tableAudio, &QTableWidget::cellDoubleClicked, this, &View::playTrack);
    this->connect(m_start,      &QPushButton::clicked, m_player, &QMediaPlayer::play);
    this->connect(m_pause,      &QPushButton::clicked, m_player, &QMediaPlayer::pause);
    this->connect(m_stop,       &QPushButton::clicked, m_player, &QMediaPlayer::stop);
    this->connect(m_download,   &QPushButton::clicked, this, &View::downloadTrack);
    this->connect(m_volume,     &QSlider::sliderMoved, m_player, &QMediaPlayer::setVolume);
    this->connect(m_position,   &QSlider::sliderMoved, this, [this](int msec)
    {
        m_player->setPosition(static_cast<qint64>(msec) * 1000);

        QTime displayTime(0, (msec / 60000) % 60, (msec / 1000) % 60);
        m_duration->display(displayTime.toString("mm:ss"));
    });

    this->connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 msec)
    {
        m_position->setValue(static_cast<int>(msec) / 1000);

        QTime displayTime(0, (msec / 60000) % 60, (msec / 1000) % 60);
        m_duration->display(displayTime.toString("mm:ss"));

    });
    this->connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 msec)
    { m_position->setRange(0, static_cast<int>(msec) / 1000); });
}

void View::setVisibleWidgets(bool value)
{
    m_download->setVisible(value);
    m_tableAudio->setVisible(value);
    m_position->setVisible(value);
    m_duration->setVisible(value);
    m_start->setVisible(value);
    m_pause->setVisible(value);
    m_stop->setVisible(value);
    m_volume->setVisible(value);
    m_nameTrack->setVisible(value);
    m_repeat->setVisible(value);
    m_random->setVisible(value);
    m_streak->setVisible(value);
    m_authentication->setVisible(!value);
}

void View::updateListTrack(const QVector<std::tuple<Id, Artist, Title, Duration>>& infoTrack)
{
    int currentRow = 0;
    for(auto& track : infoTrack)
    {
        int id;
        QString artist;
        QString title;
        int duration;
        std::tie(id, artist, title, duration) = track;
        m_tableAudio->setRowCount(m_tableAudio->rowCount() + 1);
        QTableWidgetItem* artistItem = new QTableWidgetItem(artist);
        QTableWidgetItem* titleItem = new QTableWidgetItem(title);
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        QTableWidgetItem* durationItem = new QTableWidgetItem(durationTime.toString("mm:ss"));
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(id));
        m_tableAudio->setItem(currentRow, 0, artistItem);
        m_tableAudio->setItem(currentRow, 1, titleItem);
        m_tableAudio->setItem(currentRow, 2, durationItem);
        m_tableAudio->setItem(currentRow, 3, idItem);
        currentRow += 1;
    }
}

void View::playTrack(int row, int)
{
    QUrl urlTrack = m_controller->getUrlTrack(m_tableAudio->item(row, 3)->text().toInt());
    QString nameTrack = m_controller->getNameTrack(m_tableAudio->item(row, 3)->text().toInt());
    if(urlTrack.isEmpty() || nameTrack.isEmpty())
        return;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    manager->get(QNetworkRequest(urlTrack));

    this->connect(manager, &QNetworkAccessManager::finished, [this, nameTrack, urlTrack](QNetworkReply* reply)
    {
        m_trackTmp->close();
        m_trackTmp->setData(reply->readAll());
        m_trackTmp->open(QIODevice::ReadOnly);

        m_player->setMedia(QMediaContent(urlTrack), m_trackTmp);
        m_player->play();

        m_nameTrack->setText(nameTrack);
    });
}

void View::downloadTrack()
{
    if(!m_trackTmp->isOpen())
        return;

    QString pathFile = QFileDialog::getSaveFileName(this, "Save File", QString(), ".mp3");
    QFile fileSave(pathFile + ".mp3");
    if(fileSave.open(QIODevice::WriteOnly))
        fileSave.write(m_trackTmp->buffer());
    fileSave.close();
}

