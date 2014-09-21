#include "vkaudio.h"

VkAudio::VkAudio(QWidget* parent) : QWidget(parent)
{
    m_quickView->setSource(QUrl("qrc:/mainInterface.qml"));
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_authorization->load(QUrl("https://oauth.vk.com/authorize?"
                               "client_id=4290375"
                               "&scope=ofline"
                               "&redirect_uri=https://oauth.vk.com/blank.html"
                               "&display=mobile"
                               "&v=5.24"
                               "&response_type=token"));
    m_quickWidget = QWidget::createWindowContainer(m_quickView);
    m_quickWidget->setVisible(false);
    m_modelAudio->registerObserver(this);
    m_quickView->rootContext()->setContextProperty("connectVkAudio", this);

    QHBoxLayout* box = new QHBoxLayout;
    box->addWidget(m_authorization);
    box->addWidget(m_quickWidget);
    box->setMargin(0);
    this->setLayout(box);

    this->connect(m_modelAudio, &ModelAudio::loadTrue, m_modelAudio, &ModelAudio::getPlaylistMy);
    this->connect(m_authorization, &QWebView::urlChanged, this, &VkAudio::checkUrl);
    this->connect(m_player, &QMediaPlayer::positionChanged, this, [this](quint64 position)
    {
        QTime displayTime(0, (position / 60000) % 60, (position / 1000) % 60);
        emit mediaPositionChanged(position / 1000, displayTime.toString("mm:ss"));
    });
    this->connect(m_player, &QMediaPlayer::durationChanged, this, [this](quint64 duration)
    { emit mediaDurationChanged(duration / 1000); });
    this->connect(m_quickView->rootObject(), SIGNAL(selectIdTrack(QString)), SLOT(urlTrack(QString)));
}

VkAudio::~VkAudio()
{ delete m_quickView; }

void VkAudio::updateListFriend(const QVector<std::tuple<IdUser, QString, QIcon>>& listFriend)
{
    for(auto& info : listFriend)
    {
        IdUser id;
        QString name;
        QIcon icon;
        std::tie(id, name, icon) = info;
        //qDebug()<<id<<name;
        //ui->cmbFriend->addItem(icon, name, QVariant(id));
    }
}

void VkAudio::updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration>>& infoTrack)
{
    std::for_each(m_dataList_.begin(), m_dataList_.end(), std::bind(&QObject::deleteLater, std::placeholders::_1));
    m_dataList_.clear();
    for(auto& track : infoTrack)
    {
        QString id;
        Artist artist;
        Title title;
        Duration duration;
        std::tie(id, artist, title, duration) = track;
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        m_dataList_.push_back(new DataObject(artist, title, durationTime.toString("mm:ss"), id, this));
    }
    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(m_dataList_));
}

void VkAudio::checkUrl(const QUrl& url)
{
    QUrlQuery query(url.toString().replace('#', '&'));
    QString token = query.queryItemValue("access_token");
    if(token.length() == 0)
        return;
    m_authorization->setVisible(false);
    m_quickWidget->setVisible(true);
    m_modelAudio->findPlaylist(token);
}

void VkAudio::urlTrack(const QString& id)
{
    delete m_loadTrack;
    m_loadTrack = nullptr;
    m_loadTrack = new QNetworkAccessManager(this);
    QNetworkReply* reply = m_loadTrack->get(QNetworkRequest(m_modelAudio->findUrlTrack(id)));

    this->connect(reply, &QNetworkReply::downloadProgress, this, [this](quint64 received, quint64 total)
    {
        if(static_cast<int>(total) < 0)
            return;
        emit progressDownloadValue(100 * received / total);
    });
    this->connect(m_loadTrack, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply)
    {
        m_bufferTrack->close();
        m_bufferTrack->setData(reply->readAll());
        m_bufferTrack->open(QIODevice::ReadOnly);
        m_player->setMedia(nullptr, m_bufferTrack);
        m_player->play();
        QTimer::singleShot(100, this, SIGNAL(progressDownloadValue()));
    });
}
