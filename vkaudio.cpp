#include "vkaudio.h"

VkAudio::VkAudio(QWidget* parent) : QWidget(parent)
{
    m_quickView->setSource(QUrl("qrc:/qmlInterface/Interface.qml"));
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_authorization->load(QUrl("https://oauth.vk.com/authorize?"
                               "client_id=4290375"
                               "&scope=audio,friends,photos"
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

    this->connect(m_authorization, &QWebView::urlChanged, this, &VkAudio::checkUrl);
    this->connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &VkAudio::mediaStatus);
    this->connect(m_player, &QMediaPlayer::positionChanged, this, [this](quint64 position)
    {
        QTime displayTime(0, (position / 60000) % 60, (position / 1000) % 60);
        emit mediaPositionChanged(position / 1000, displayTime.toString("mm:ss"));
    });
    this->connect(m_player, &QMediaPlayer::durationChanged, std::bind(&VkAudio::mediaDurationChanged, this,
        std::bind(std::divides<qint64>(), std::placeholders::_1, 1000)));

    QQuickItem* item = m_quickView->rootObject();
    this->connect(item, SIGNAL(selectIdTrack(QString)),     SLOT(urlTrack(QString)));
    this->connect(item, SIGNAL(positionTrackChange(int)),   SLOT(setPositionPlayer(int)));
    this->connect(item, SIGNAL(selectPlayTrack()),          m_player, SLOT(play()));
    this->connect(item, SIGNAL(selectPauseTrack()),         m_player, SLOT(pause()));
    this->connect(item, SIGNAL(volumeTrackChange(int)),     m_player, SLOT(setVolume(int)));
    this->connect(item, SIGNAL(selectNextTrack(QString)),   SLOT(setNextTrack(QString)));
    this->connect(item, SIGNAL(selectPrevTrack(QString)),   SLOT(setPrevTrack(QString)));
    this->connect(item, SIGNAL(selectLoopTrack(bool)),      SLOT(setLoopTrack(bool)));
    this->connect(item, SIGNAL(selectRandomTrack(bool)),    SLOT(setRandomTrack(bool)));
    this->connect(item, SIGNAL(clickedDownloadTrack(QString)),      SLOT(downloadTrack(QString)));
    this->connect(item, SIGNAL(returnPressedSearchTrack(QString)),  SLOT(filterTrack(QString)));
    this->connect(item, SIGNAL(selectPlaylistFriend(QString)),      SLOT(getPlaylistFriendModel(QString)));
    this->connect(item, SIGNAL(selectPlaylistMy()), SLOT(getPlaylistMyModel()));
    this->connect(item, SIGNAL(addTrack(QString, QString)), m_modelAudio, SLOT(addTrack(QString, QString)));
    this->connect(item, SIGNAL(returnPressedGlobalSearchTrack(QString)), SLOT(getGlobalSearchAudioModel(QString)));
    this->connect(item, SIGNAL(removeTrack(QString, QString, bool)), SLOT(pushRemoveTrack(QString, QString, bool)));
    this->connect(item, SIGNAL(deleteTrack()), SLOT(deleteAllTrack()));
    this->connect(item, SIGNAL(uploadTrack()), SLOT(openFileForUpload()));
    this->connect(item, SIGNAL(selectPlaylistRecommended(QString)), SLOT(getRecommendedModel(QString)));
    this->connect(item, SIGNAL(selectPlaylistPopular(QString)),     SLOT(getPopularModel(QString)));


    this->connect(m_modelAudio, &ModelAudio::progressDownload, this, [this](qint64 value)
    {
        qDebug()<<value;
        //emit progressDownloadTrack(value);
    });
}

VkAudio::~VkAudio()
{ delete m_quickView; }

void VkAudio::updateListFriend(const QVector<std::tuple<IdUser, QString, QUrl>>& listFriend)
{
    std::for_each(m_propertyModelFriend_.begin(), m_propertyModelFriend_.end(), std::bind(&QObject::deleteLater, std::placeholders::_1));
    m_propertyModelFriend_.clear();

    IdUser id;
    QString name;
    QUrl url;
    for(auto& info : listFriend)
    {
        std::tie(id, name, url) = info;
        m_propertyModelFriend_.push_back(new PropertyModelFriend(name, id, url, this));
    }
    if(m_propertyModelFriend_.size() % 2 != 0)
    {
        id = "indefinite";
        name = "indefinite";
        url = QUrl();
        m_propertyModelFriend_.push_back(new PropertyModelFriend(name, id, url, this));
    }

    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkFriendModel", QVariant::fromValue(m_propertyModelFriend_));
}

void VkAudio::updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration, IdUser>>& infoTrack)
{
    std::for_each(m_propertyModelAudio_.begin(), m_propertyModelAudio_.end(), std::bind(&QObject::deleteLater, std::placeholders::_1));
    m_propertyModelAudio_.clear();
    for(auto& track : infoTrack)
    {
        QString idTrack;
        Artist artist;
        Title title;
        Duration duration;
        IdUser idUser;
        std::tie(idTrack, artist, title, duration, idUser) = track;
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        m_propertyModelAudio_.push_back(new PropertyModelAudio(artist, title, durationTime.toString("mm:ss"), idTrack, idUser, m_addAndCancel, this));
    }
    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(m_propertyModelAudio_));
}

QUrl VkAudio::getUrlAvatarMy() const
{ return std::get<2>(m_modelAudio->getInfoMy()); }

QString VkAudio::getIdMy() const
{ return std::get<0>(m_modelAudio->getInfoMy()); }

QString VkAudio::getNameAvatarMy() const
{ return std::get<1>(m_modelAudio->getInfoMy()); }

void VkAudio::checkUrl(const QUrl& url)
{
    QUrlQuery query(url.toString().replace('#', '&'));
    QString token = query.queryItemValue("access_token");
    if(token.length() == 0)
        return;
    m_authorization->setVisible(false);
    m_quickWidget->setVisible(true);
    m_modelAudio->findPlaylist(token);
    m_modelAudio->getPlaylistMy();
}

void VkAudio::urlTrack(const QString& id)
{
    m_loadTrack->deleteLater();
    m_loadTrack = new QNetworkAccessManager(this);
    QNetworkReply* reply = m_loadTrack->get(QNetworkRequest(m_modelAudio->findUrlTrack(id)));

    this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&VkAudio::progressDownloadTrack, this,
        std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(m_loadTrack, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply)
    {
        m_bufferTrack->close();
        m_bufferTrack->setData(reply->readAll());
        m_bufferTrack->open(QIODevice::ReadOnly);
        m_player->setMedia(nullptr, m_bufferTrack);
        m_player->play();
        QTimer::singleShot(100, this, SIGNAL(progressDownloadTrack()));
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
            m_modelAudio->setStateTrack(propertyModel->idTrack(), ModelAudio::StateTrack::Show);
            result.push_back(objectModel);
        }
        else
            m_modelAudio->setStateTrack(propertyModel->idTrack(), ModelAudio::StateTrack::Hide);
    }
    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(result));
}

void VkAudio::pushRemoveTrack(const QString& trackId, const QString& userId, bool remove)
{
    if(remove)
    {
        m_deleteTrack_.remove(trackId);
        m_modelAudio->setStateTrack(trackId, ModelAudio::StateTrack::Show);
    }
    else
    {
        m_deleteTrack_.insert(trackId, userId);
        m_modelAudio->setStateTrack(trackId, ModelAudio::StateTrack::Remove);
    }
}

void VkAudio::deleteAllTrack()
{
    for(auto i = m_deleteTrack_.begin(); i != m_deleteTrack_.end(); i++)
        m_modelAudio->deleteTrack(i.key(), i.value());
    m_deleteTrack_.clear();
}

void VkAudio::openFileForUpload()
{
    QString pathFile = QFileDialog::getOpenFileName(this, "Open File", QString(), "*.mp3");
    QFile fileOpen(pathFile);
    if(fileOpen.open(QIODevice::ReadOnly))
        m_modelAudio->uploadServerTrack(fileOpen.readAll());
    fileOpen.close();
}

void VkAudio::getPlaylistMyModel()
{
    m_addAndCancel = true;
    m_modelAudio->getPlaylistMy();
}

void VkAudio::getRecommendedModel(const QString& idUser)
{
    m_addAndCancel = false;
    m_modelAudio->getRecommended(idUser);
}

void VkAudio::getPopularModel(const QString& id)
{
    m_addAndCancel = false;
    m_modelAudio->getPopular(id);
}

void VkAudio::getGlobalSearchAudioModel(const QString& artist)
{
    m_addAndCancel = false;
    m_modelAudio->globalSearchAudio(artist);
}

void VkAudio::getPlaylistFriendModel(const QString& idUser)
{
    m_addAndCancel = false;
    m_modelAudio->getPlaylistFriend(idUser);
}
