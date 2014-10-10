#include "modelaudio.h"

ModelAudio::ModelAudio(QObject* parent) : QObject(parent)
{ }

void ModelAudio::parserAudio(QNetworkReply* reply)
{
    m_vecInfoTrack_.clear();
    m_hashInfoTrack_.clear();

    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode nodeAudio = doc.documentElement().firstChild();
    m_vecInfoTrack_.reserve(nodeAudio.toElement().text().toInt());
    nodeAudio = doc.documentElement().firstChild().nextSibling().firstChild();
    while(!nodeAudio.isNull())
    {
        QDomNode nodeInfoTrack = nodeAudio.toElement().firstChild();
        InfoTrack infoTrack;
        IdTrack idTrack;
        while(!nodeInfoTrack.isNull())
        {
            QDomElement element = nodeInfoTrack.toElement();
            if(element.tagName() == "id")
                idTrack = element.text();
            else if(element.tagName() == "artist")
                infoTrack.artist = element.text();
            else if(element.tagName() == "title")
                infoTrack.title = element.text();
            else if(element.tagName() == "duration")
                infoTrack.duration = element.text().toInt();
            else if(element.tagName() == "url")
                infoTrack.url = QUrl(element.text());
            else if(element.tagName() == "owner_id")
                infoTrack.ownerId = element.text();
            nodeInfoTrack = nodeInfoTrack.nextSibling();
        }

        m_vecInfoTrack_.push_back(qMakePair(StateTrack::Show, infoTrack));
        m_hashInfoTrack_.insert(idTrack, m_vecInfoTrack_.end() - 1);

        nodeAudio = nodeAudio.nextSibling();
    }

    notifyAudioObservers();
}

void ModelAudio::parserFriend(QNetworkReply* reply)
{
    m_countFriend = 0;
    QStringList idFriend;
    QStringList resutlFriend;
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode myFriend = doc.documentElement().firstChild().nextSibling().firstChild();
    while(!myFriend.isNull())
    {
        idFriend.push_back(myFriend.toElement().text());
        myFriend = myFriend.nextSibling();
    }

    QUrlQuery queryUserMy("https://api.vk.com/method/users.get.xml");
    queryUserMy.addQueryItem("fields", "photo_100");
    queryUserMy.addQueryItem("v", "5.24");
    queryUserMy.addQueryItem("access_token", m_token);

    QNetworkAccessManager* loadUserMy = new QNetworkAccessManager(this);

    QEventLoop loopMy;
    QNetworkReply* replyMy = loadUserMy->get(QNetworkRequest(makeWorkUrl(queryUserMy.toString())));
    this->connect(replyMy, &QNetworkReply::finished, &loopMy, &QEventLoop::quit);
    //this->connect(replyMy, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
      //  std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadUserMy, &QNetworkAccessManager::finished, [this](QNetworkReply* reply)
    { m_infoMy = getResultParserUser(reply->readAll()); });
    this->connect(replyMy, &QNetworkReply::finished, loadUserMy, &QNetworkAccessManager::deleteLater);
    loopMy.exec();

    for(QString& id : idFriend)
    {
        QUrlQuery queryUserFriend("https://api.vk.com/method/users.get.xml");
        queryUserFriend.addQueryItem("user_ids", id);
        queryUserFriend.addQueryItem("fields", "photo_100");
        queryUserFriend.addQueryItem("v", "5.24");
        queryUserFriend.addQueryItem("access_token", m_token);
        resutlFriend.push_back(queryUserFriend.toString());
    }

    for(QString& url : resutlFriend)
        for(int i = 0; i < url.length(); i++)
            if(url[i] == '&')
            {
                url.replace(i, 1, '?');
                break;
            }

    m_countFriend = resutlFriend.size();
    for(QString& url : resutlFriend)
    {
        QEventLoop sleep;
        QTimer::singleShot(220, &sleep, SLOT(quit()));
        sleep.exec();

        QNetworkAccessManager* loadUserFriend = new QNetworkAccessManager(this);

        QEventLoop loopFriend;
        QNetworkReply* replyFriend = loadUserFriend->get(QNetworkRequest(url));
        this->connect(replyFriend, &QNetworkReply::finished, &loopFriend, &QEventLoop::quit);
        //this->connect(replyFriend, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
          //  std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
        this->connect(loadUserFriend, &QNetworkAccessManager::finished, this, &ModelAudio::parserUser);
        this->connect(replyFriend, &QNetworkReply::finished, loadUserFriend, &QNetworkAccessManager::deleteLater);
        loopFriend.exec();
    }
}

void ModelAudio::parserUser(QNetworkReply* reply)
{
    auto result = getResultParserUser(reply->readAll());
    m_infoFriend_.insert(result.first, qMakePair(result.second.first, result.second.second));

    qDebug()<<m_infoFriend_.size()<<"\t"<<m_countFriend<<"asdf";
    if(m_countFriend == m_infoFriend_.size())
        notifyFriendObservers();
}

QPair<IdUser, QPair<QString, QUrl>> ModelAudio::getResultParserUser(const QByteArray& array)
{
    QDomDocument doc;
    doc.setContent(array);
    QDomNode nodeUser = doc.documentElement().firstChild().toElement().firstChild();
    IdUser id;
    QString name;
    QUrl photo;
    qDebug()<<doc.toString()<<"\n\nf432432";
    while(!nodeUser.isNull())
    {
        QDomElement element = nodeUser.toElement();
        if(element.tagName() == "id")
            id = element.text();
        else if(element.tagName() == "first_name")
            name = element.text() + " ";
        else if(element.tagName() == "last_name")
            name += element.text();
        else if(element.tagName() == "photo_100")
            photo = element.text();
        nodeUser = nodeUser.nextSibling();
    }
    return qMakePair(id, qMakePair(name, photo));
}

QUrl ModelAudio::makeWorkUrl(const QString& url)
{
    QString result = url;
    for(int i = 0; i < result.length(); i++)
        if(result[i] == '&')
        {
            result.replace(i, 1, '?');
            break;
        }
    return qMove(QUrl(result));
}

QUrl ModelAudio::findUrlTrack(const QString& id)
{ return m_hashInfoTrack_[id]->second.url; }

QString ModelAudio::getNextIdTrack(const QString& id)
{
    auto iter = m_hashInfoTrack_[id];
    auto iterEnd = iter;
    for(auto i = iter + 1; i != m_vecInfoTrack_.end(); i++)
        if(i->first == StateTrack::Show)
            return m_hashInfoTrack_.key(i);

    for(auto i = m_vecInfoTrack_.begin(); i != iterEnd; i++)
        if(i->first == StateTrack::Show)
            return m_hashInfoTrack_.key(i);
    return id;
}

QString ModelAudio::getPrevIdTrack(const QString& id)
{
    auto iter = m_hashInfoTrack_[id];
    auto iterBegin = iter;
    for(auto i = iter - 1; i >= m_vecInfoTrack_.begin(); i--)
        if(i->first == StateTrack::Show)
            return m_hashInfoTrack_.key(i);

    for(auto i = m_vecInfoTrack_.end() - 1; i >= iterBegin; i--)
        if(i->first == StateTrack::Show)
            return m_hashInfoTrack_.key(i);
    return id;
}

QString ModelAudio::getRandomIdTrack()
{
    if(m_vecInfoTrack_.size() == 1)
        return m_hashInfoTrack_.key(m_vecInfoTrack_.begin());

    forever
    {
        int currentIndex = qrand() % m_vecInfoTrack_.size();
        QString idRandom = m_hashInfoTrack_.key(m_vecInfoTrack_.begin() + currentIndex);
        if(m_vecInfoTrack_[currentIndex].first == StateTrack::Show)
            return idRandom;
    }
    return QString();
}

std::tuple<IdUser, QString, QUrl> ModelAudio::getInfoMy() const
{ return std::make_tuple(m_infoMy.first, m_infoMy.second.first, m_infoMy.second.second); }

void ModelAudio::setStateTrack(const QString& id, StateTrack state)
{ m_hashInfoTrack_[id]->first = state; }

void ModelAudio::findPlaylist(const QString& token)
{
    QNetworkAccessManager* loadFriend = new QNetworkAccessManager(this);
    m_token = token;

    QUrlQuery queryFriend("https://api.vk.com/method/friends.get.xml");
    queryFriend.addQueryItem("v", "5.24");
    queryFriend.addQueryItem("access_token", m_token);

    QEventLoop loop;
    QNetworkReply* reply = loadFriend->get(QNetworkRequest(makeWorkUrl(queryFriend.toString())));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
       // std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadFriend, &QNetworkAccessManager::finished, this, &ModelAudio::parserFriend);
    this->connect(reply, &QNetworkReply::finished, loadFriend, &QNetworkAccessManager::deleteLater);
    loop.exec();
}

void ModelAudio::globalSearchAudio(const QString& artist)
{
    m_vecInfoTrack_.clear();
    m_hashInfoTrack_.clear();
    QUrlQuery query("https://api.vk.com/method/audio.search.xml");
    query.addQueryItem("q", artist);
    query.addQueryItem("performer_only", "1");
    query.addQueryItem("count", "300");
    query.addQueryItem("v", "5.24");
    query.addQueryItem("access_token", m_token);

    QNetworkAccessManager* loadGlobalAudio = new QNetworkAccessManager(this);
    QNetworkReply* reply = loadGlobalAudio->get(QNetworkRequest(makeWorkUrl(query.toString())));
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
      //  std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadGlobalAudio, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
    this->connect(reply, &QNetworkReply::finished, loadGlobalAudio, &QNetworkAccessManager::deleteLater);
}

void ModelAudio::getRecommended(const QString& idUser)
{
    QUrlQuery query("https://api.vk.com/method/audio.getRecommendations.xml");
    query.addQueryItem("user_id", idUser);
    query.addQueryItem("count", "1000");
    query.addQueryItem("v", "5.24");
    query.addQueryItem("access_token", m_token);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(makeWorkUrl(query.toString())));
    this->connect(manager, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
    this->connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);
}

void ModelAudio::registerObserver(Observer::AbstractObserver* observer)
{ m_observer_.push_back(observer); }

void ModelAudio::removeObserver(Observer::AbstractObserver* observer)
{
    auto iterRemove = qFind(m_observer_.begin(), m_observer_.end(), observer);
    if(iterRemove != m_observer_.end())
        m_observer_.erase(iterRemove);
}

void ModelAudio::notifyAudioObservers()
{
    QVector<std::tuple<IdTrack, Artist, Title, Duration, IdUser>> infoTrack;
    for(auto i = m_vecInfoTrack_.begin(); i != m_vecInfoTrack_.end(); i++)
    {
        IdTrack id = m_hashInfoTrack_.key(i);
        InfoTrack& track = i->second;
        infoTrack.push_back(std::make_tuple(id, track.artist, track.title, track.duration, track.ownerId));
    }
    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(&Observer::AbstractObserver::updatePlaylist, std::placeholders::_1, infoTrack));
}

void ModelAudio::notifyFriendObservers()
{
    QVector<std::tuple<IdUser, QString, QUrl>> listFriend;
    for(auto iter = m_infoFriend_.begin(); iter != m_infoFriend_.end(); iter++)
        listFriend.push_back(std::make_tuple(iter.key(), iter.value().first, iter.value().second));
    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(&Observer::AbstractObserver::updateListFriend, std::placeholders::_1, listFriend));
}

void ModelAudio::getPlaylistMy()
{ getPlaylistFriend(m_infoMy.first); }

void ModelAudio::getPlaylistFriend(const QString& id)
{
    QUrlQuery queryAudio("https://api.vk.com/method/audio.get.xml");
    queryAudio.addQueryItem("owner_id", id);
    queryAudio.addQueryItem("v", "5.24");
    queryAudio.addQueryItem("access_token", m_token);

    QEventLoop loop;
    QNetworkAccessManager* loadAudio = new QNetworkAccessManager(this);
    QNetworkReply* reply = loadAudio->get(QNetworkRequest(makeWorkUrl(queryAudio.toString())));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
       // std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadAudio, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
    this->connect(reply, &QNetworkReply::finished, loadAudio, &QNetworkAccessManager::deleteLater);
    loop.exec();
}

void ModelAudio::addTrack(const QString& trackId, const QString& userId)
{
    QUrlQuery queryAddTrack("https://api.vk.com/method/audio.add");
    queryAddTrack.addQueryItem("audio_id", trackId);
    queryAddTrack.addQueryItem("owner_id", userId);
    queryAddTrack.addQueryItem("v", "5.24");
    queryAddTrack.addQueryItem("access_token", m_token);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(makeWorkUrl(queryAddTrack.toString())));
    this->connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);
}

void ModelAudio::deleteTrack(const QString& trackId, const QString& userId)
{
    QUrlQuery queryDeleteTrack("https://api.vk.com/method/audio.delete");
    queryDeleteTrack.addQueryItem("audio_id", trackId);
    queryDeleteTrack.addQueryItem("owner_id", userId);
    queryDeleteTrack.addQueryItem("v", "5.24");
    queryDeleteTrack.addQueryItem("access_token", m_token);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(makeWorkUrl(queryDeleteTrack.toString())));
    this->connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);
}

void ModelAudio::uploadServerTrack(QFile* data)
{
    //?????????????????????????????????????????????????
    QUrlQuery queryUpload("https://api.vk.com/method/audio.getUploadServer.xml");
    queryUpload.addQueryItem("v", "5.24");
    queryUpload.addQueryItem("access_token", m_token);

 /*   auto multipart = new QHttpMultiPart (QHttpMultiPart::FormDataType);
    const auto& path = info.FilePath_;
    auto file = new QFile (path, multipart);
    file->open (QIODevice::ReadOnly);
    QHttpPart filePart;
    const auto& disp = QString ("form-data; name=\"file1\"; filename=\"%1\"")
    .arg (QFileInfo (path).fileName ());
    filePart.setHeader (QNetworkRequest::ContentDispositionHeader, disp);
    filePart.setBodyDevice (file);
    multipart->append (filePart);
*/

     /*QHttpMultiPart* multipart = new QHttpMultiPart (QHttpMultiPart::FormDataType);

         QHttpPart filePart;

         filePart.setHeader (QNetworkRequest::ContentDispositionHeader, "form-data; name=\"file1\"; filename=\"track1\"");
         filePart.setBodyDevice (data);
         multipart->append (filePart);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(makeWorkUrl(queryUpload.toString()));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    QNetworkReply* reply = manager->post(request, multipart);

    this->connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply)
    {
       qDebug()<<reply->readAll();
    });*/


}
