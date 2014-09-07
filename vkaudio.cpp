#include "vkaudio.h"
#include "ui_vkaudio.h"

VkAudio::VkAudio(QWidget* parent) : QWidget(parent),
    ui(new Ui::VkAudio)
{
    ui->setupUi(this);
    this->resize(800, 500);

    ui->authentication->load(QUrl("https://oauth.vk.com/authorize?"
                               "client_id=4290375"
                               "&scope=audio, ofline"
                               "&redirect_uri=https://oauth.vk.com/blank.html"
                               "&display=mobile"
                               "&v=5.24"
                               "&response_type=token"));
    ui->tableAudio->setColumnCount(4);
    ui->tableAudio->setHorizontalHeaderLabels({ "artist", "title", "duration" });
    ui->tableAudio->setColumnHidden(3, true);
    ui->tableAudio->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableAudio->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableAudio->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableAudio->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->volume->setRange(0, 100);
    ui->volume->setValue(100);
    ui->duration->display(QTime(0, 0, 0).toString("mm:ss"));
    ui->duration->setSegmentStyle(QLCDNumber::Flat);
    ui->streak->setChecked(true);
    ui->progressDownload->setVisible(false);
    m_model->registerObserver(dynamic_cast<Observer::AbstractObserver*>(this));
    setVisibleWebView(false);

    this->connect(ui->authentication, &QWebView::urlChanged, this, &VkAudio::checkUrl);
    this->connect(ui->tableAudio, &QTableWidget::cellDoubleClicked, this, &VkAudio::playTrack);
    this->connect(ui->start,      &QPushButton::clicked, m_player, &QMediaPlayer::play);
    this->connect(ui->pause,      &QPushButton::clicked, m_player, &QMediaPlayer::pause);
    this->connect(ui->stop,       &QPushButton::clicked, m_player, &QMediaPlayer::stop);
    this->connect(ui->download,   &QPushButton::clicked, this, &VkAudio::downloadTrack);
    this->connect(ui->volume,     &QSlider::sliderMoved, m_player, &QMediaPlayer::setVolume);
    this->connect(ui->position,   &QSlider::sliderMoved, this, [this](int msec)
    {
        m_player->setPosition(static_cast<qint64>(msec) * 1000);

        QTime displayTime(0, (msec / 60000) % 60, (msec / 1000) % 60);
        ui->duration->display(displayTime.toString("mm:ss"));
    });
    this->connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 msec)
    {
        ui->position->setValue(static_cast<int>(msec) / 1000);

        QTime displayTime(0, (msec / 60000) % 60, (msec / 1000) % 60);
        ui->duration->display(displayTime.toString("mm:ss"));

    });
    this->connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &VkAudio::mediaStatus);
    this->connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 msec)
    { ui->position->setRange(0, static_cast<int>(msec) / 1000); });
}

VkAudio::~VkAudio()
{
    delete ui;
}

void VkAudio::updateListTrack(const QVector<std::tuple<Id, Artist, Title, Duration>>& infoTrack)
{
    int currentRow = 0;
    for(auto& track : infoTrack)
    {
        int id;
        QString artist;
        QString title;
        int duration;
        std::tie(id, artist, title, duration) = track;
        ui->tableAudio->setRowCount(ui->tableAudio->rowCount() + 1);
        QTableWidgetItem* artistItem = new QTableWidgetItem(artist);
        QTableWidgetItem* titleItem = new QTableWidgetItem(title);
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        QTableWidgetItem* durationItem = new QTableWidgetItem(durationTime.toString("mm:ss"));
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(id));
        ui->tableAudio->setItem(currentRow, 0, artistItem);
        ui->tableAudio->setItem(currentRow, 1, titleItem);
        ui->tableAudio->setItem(currentRow, 2, durationItem);
        ui->tableAudio->setItem(currentRow, 3, idItem);
        currentRow += 1;
    }
}

void VkAudio::checkUrl(const QUrl& url)
{
    QUrlQuery query(url.toString().replace('#', '&'));
    QString token = query.queryItemValue("access_token");
    if(token.length() == 0)
        return;
    setVisibleWebView(true);
    m_model->setUrlAudio(token);
}

void VkAudio::playTrack(int row, int)
{
    if(!m_flagRequest)
        return;
    int currentId = ui->tableAudio->item(row, 3)->text().toInt();
    QUrl urlTrack = m_model->findUrlTrack(currentId);
    QString nameTrack = m_model->findNameTrack(currentId);
    if(urlTrack.isEmpty() || nameTrack.isEmpty())
        return;
    loadTrack(urlTrack, nameTrack, currentId);
}

void VkAudio::downloadTrack()
{
    if(!m_trackTmp->isOpen())
        return;

    QString pathFile = QFileDialog::getSaveFileName(this, "Save File", ui->nameTrack->text(), ".mp3");
    QFile fileSave(pathFile + ".mp3");
    if(fileSave.open(QIODevice::WriteOnly))
        fileSave.write(m_trackTmp->buffer());
    fileSave.close();
}

void VkAudio::mediaStatus(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia)
    {
        int nextTrackId = -1;
        if(ui->repeat->isChecked())
        {
            m_player->play();
            return;
        }
        else if(ui->streak->isChecked())
            nextTrackId = m_model->getNextIdTrack(m_currentIdPlayer);
        else if(ui->random->isChecked())
            nextTrackId = m_model->getRandomIdTrack(m_currentIdPlayer);

        QUrl nextTrackUrl = m_model->findUrlTrack(nextTrackId);
        QString nextTrackName = m_model->findNameTrack(nextTrackId);
        for(int i = 0; i < ui->tableAudio->rowCount(); i++)
            if(ui->tableAudio->item(i, 3)->text().toInt() == nextTrackId)
            {
                ui->tableAudio->setCurrentCell(i, 0);
                break;
            }
        loadTrack(nextTrackUrl, nextTrackName, nextTrackId);
    }
}

void VkAudio::loadTrack(const QUrl& urlTrack, const QString& nameTrack, int currentId)
{
    m_currentIdPlayer = currentId;
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(urlTrack));

    this->connect(reply, &QNetworkReply::downloadProgress, this, [this](quint64 received, quint64 total)
    {
        if(static_cast<int>(total) < 0)
            return;
        m_flagRequest = false;
        ui->progressDownload->setVisible(true);
        ui->progressDownload->setValue(100 * received / total);
    });
    this->connect(manager, &QNetworkAccessManager::finished, [this, nameTrack, urlTrack](QNetworkReply* reply)
    {
        m_trackTmp->close();
        m_trackTmp->setData(reply->readAll());
        m_trackTmp->open(QIODevice::ReadOnly);

        m_player->setMedia(nullptr, m_trackTmp);
        m_player->play();

        ui->nameTrack->setText(nameTrack);
        ui->progressDownload->setVisible(false);
        m_flagRequest = true;
    });
}

void VkAudio::setVisibleWebView(bool value)
{
    ui->download->setVisible(value);
    ui->tableAudio->setVisible(value);
    ui->position->setVisible(value);
    ui->duration->setVisible(value);
    ui->start->setVisible(value);
    ui->pause->setVisible(value);
    ui->stop->setVisible(value);
    ui->volume->setVisible(value);
    ui->nameTrack->setVisible(value);
    ui->repeat->setVisible(value);
    ui->random->setVisible(value);
    ui->streak->setVisible(value);
    ui->authentication->setVisible(!value);
}
