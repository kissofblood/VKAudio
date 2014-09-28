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
    //m_authorization->setVisible(false);
    m_modelAudio->registerObserver(this);
    m_quickView->rootContext()->setContextProperty("connectVkAudio", this);

    QHBoxLayout* box = new QHBoxLayout;
    box->addWidget(m_authorization);
    box->addWidget(m_quickWidget);
    box->setMargin(0);
    this->setLayout(box);

    this->connect(m_modelAudio, &ModelAudio::loadTrue, m_modelAudio, &ModelAudio::getPlaylistMy);
    this->connect(m_authorization, &QWebView::urlChanged, this, &VkAudio::checkUrl);
    this->connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &VkAudio::mediaStatus);
    this->connect(m_player, &QMediaPlayer::positionChanged, this, [this](quint64 position)
    {
        QTime displayTime(0, (position / 60000) % 60, (position / 1000) % 60);
        emit mediaPositionChanged(position / 1000, displayTime.toString("mm:ss"));
    });
    this->connect(m_player, &QMediaPlayer::durationChanged, this, [this](quint64 duration)
    { emit mediaDurationChanged(duration / 1000); });

    QQuickItem* item = m_quickView->rootObject();
    this->connect(item, SIGNAL(selectIdTrack(QString)),     SLOT(urlTrack(QString)));
    this->connect(item, SIGNAL(positionTrackChange(int)),   SLOT(setPositionPlayer(int)));
    this->connect(item, SIGNAL(selectPlayTrack()),          m_player, SLOT(play()));
    this->connect(item, SIGNAL(selectPauseTrack()),         m_player, SLOT(pause()));
    this->connect(item, SIGNAL(volumeTrackChange(int)),     m_player, SLOT(setVolume(int)));
    this->connect(item, SIGNAL(selectNextTrack(QString)),   SLOT(setNextTrack(QString)));
    this->connect(item, SIGNAL(selectPrevTrack(QString)),   SLOT(setPrevTrack(QString)));
    this->connect(item, SIGNAL(selectLoopTrack(bool)),      SLOT(setLoopTrack(bool)));
    this->connect(item, SIGNAL(selectRandomTrack(bool)), SLOT(setRandomTrack(bool)));
    this->connect(item, SIGNAL(clickedDownloadTrack(QString)),    SLOT(downloadTrack(QString)));
    this->connect(item, SIGNAL(returnPressedSearch(QString)),     SLOT(filterTrack(QString)));
}

VkAudio::~VkAudio()
{
    delete m_quickView;
    delete m_avatar;
}

void VkAudio::updateListFriend(const QVector<std::tuple<IdUser, QString, QPixmap>>& listFriend)
{
    std::for_each(m_propertyModelFriend_.begin(), m_propertyModelFriend_.end(), std::bind(&QObject::deleteLater, std::placeholders::_1));
    m_propertyModelFriend_.clear();
    delete m_avatar;
    m_avatar = nullptr;

    QHash<QString, QPixmap> avatart;
    for(auto& info : listFriend)
    {
        IdUser id;
        QString name;
        QPixmap pix;
        std::tie(id, name, pix) = info;
        m_propertyModelFriend_.push_back(new PropertyModelFriend(name, id, this));
        avatart.insert(id, pix);
    }

    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkFriendModel", QVariant::fromValue(m_propertyModelFriend_));

    m_avatar = new AvatarProvider(qMove(avatart));
    m_quickView->engine()->addImageProvider("avatarFriend", m_avatar);
}

void VkAudio::updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration>>& infoTrack)
{
    std::for_each(m_propertyModelAudio_.begin(), m_propertyModelAudio_.end(), std::bind(&QObject::deleteLater, std::placeholders::_1));
    m_propertyModelAudio_.clear();
    for(auto& track : infoTrack)
    {
        QString id;
        Artist artist;
        Title title;
        Duration duration;
        std::tie(id, artist, title, duration) = track;
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        m_propertyModelAudio_.push_back(new PropertyModelAudio(artist, title, durationTime.toString("mm:ss"), id, this));
    }
    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(m_propertyModelAudio_));
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
    m_loadTrack->deleteLater();
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

void VkAudio::setPositionPlayer(int position)
{ m_player->setPosition(static_cast<qint64>(position) * 1000); }

void VkAudio::setNextTrack(const QString& id)
{
    QString nextId = m_modelAudio->getNextIdTrack(id);
    urlTrack(nextId);
    emit idTrackChanged(true, nextId);
}

void VkAudio::setPrevTrack(const QString& id)
{
    QString prevId = m_modelAudio->getPrevIdTrack(id);
    urlTrack(prevId);
    emit idTrackChanged(false, prevId);
}

void VkAudio::setLoopTrack(bool value)
{ m_isLoopTrack = value; }

void VkAudio::setRandomTrack(bool value)
{ m_isRandomTrack = value; }

void VkAudio::mediaStatus(QMediaPlayer::MediaStatus status)
{
     if(status == QMediaPlayer::EndOfMedia)
     {
         if(m_isLoopTrack)
             m_player->play();
         else if(m_isRandomTrack)
         {
            QString randId = m_modelAudio->getRandomIdTrack();
            urlTrack(randId);
            emit idTrackChanged(true, randId);
         }
         else emit nextTrackDefault();
     }
}

void VkAudio::downloadTrack(const QString& name)
{
    if(!m_bufferTrack->isOpen())
        return;
    QString pathFile = QFileDialog::getSaveFileName(this, "Save File", name, "*.mp3");
    QFile fileSave(pathFile + ".mp3");
    if(fileSave.open(QIODevice::WriteOnly))
        fileSave.write(m_bufferTrack->buffer());
    fileSave.close();
}

void VkAudio::filterTrack(const QString& text)
{
    QList<QObject*> result;
    for(QObject* objectModel : m_propertyModelAudio_)
    {
        PropertyModelAudio* propertyModel = qobject_cast<PropertyModelAudio*>(objectModel);
        if(propertyModel->artist().contains(text, Qt::CaseInsensitive)
           || propertyModel->title().contains(text, Qt::CaseInsensitive))
        {
            m_modelAudio->setHideTrack(propertyModel->idTrack(), false);
            result.push_back(propertyModel);
        }
        else
            m_modelAudio->setHideTrack(propertyModel->idTrack(), true);
    }
    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(result));
}
