#include "modelaudio.h"

ModelAudio::ModelAudio(QObject* parent) : QObject(parent)
{ }

void ModelAudio::parserAudio(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode nodeAudio = doc.documentElement().firstChild();
    m_vecInfoTrack_.reserve(nodeAudio.toElement().text().toInt());
    nodeAudio = doc.documentElement().firstChild().nextSibling().firstChild();
    while(!nodeAudio.isNull())
    {
        QDomNode nodeInfoTrack = nodeAudio.toElement().firstChild();
        IdTrack id;
        Artist artist;
        Title title;
        Duration duration;
        QUrl url;
        while(!nodeInfoTrack.isNull())
        {
            QDomElement element = nodeInfoTrack.toElement();
            if(element.tagName() == "id")
                id = element.text();
            else if(element.tagName() == "artist")
                artist = element.text();
            else if(element.tagName() == "title")
                title = element.text();
            else if(element.tagName() == "duration")
                duration = element.text().toInt();
            else if(element.tagName() == "url")
                url = QUrl(element.text());
            nodeInfoTrack = nodeInfoTrack.nextSibling();
        }

        m_vecInfoTrack_.push_back(qMakePair(false, std::make_tuple(artist, title, duration, url)));
        m_hashInfoTrack_.insert(id, m_vecInfoTrack_.end() - 1);

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
    QString resultMy = queryUserMy.toString();
    for(int i = 0; i < resultMy.length(); i++)
        if(resultMy[i] == '&')
        {
            resultMy.replace(i, 1, '?');
            break;
        }

    QNetworkAccessManager* loadUserMy = new QNetworkAccessManager(this);

    QEventLoop loopMy;
    QNetworkReply* replyMy = loadUserMy->get(QNetworkRequest(resultMy));
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

    if(m_countFriend == m_infoFriend_.size())
        notifyFriendObservers();
}

QPair<IdUser, QPair<QString, QPixmap>> ModelAudio::getResultParserUser(const QByteArray& array)
{
    QNetworkAccessManager* loadAvatar = new QNetworkAccessManager(this);
    QDomDocument doc;
    doc.setContent(array);
    QDomNode nodeUser = doc.documentElement().firstChild().toElement().firstChild();
    IdUser id;
    QString name;
    QUrl photo;
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
            photo = QUrl(element.text());
        nodeUser = nodeUser.nextSibling();
    }

    QEventLoop loop;
    QPixmap pix;
    QNetworkReply* reply = loadAvatar->get(QNetworkRequest(photo));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   // this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
      //  std::bind(std::divides<quint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadAvatar, &QNetworkAccessManager::finished, [&pix](QNetworkReply* reply)
    { pix.loadFromData(reply->readAll(), "jpg"); });
    this->connect(reply, &QNetworkReply::finished, loadAvatar, &QNetworkAccessManager::deleteLater);
    loop.exec();
    return qMakePair(id, qMakePair(name, pix));
}

QUrl ModelAudio::findUrlTrack(const QString& id)
{ return std::get<3>(m_hashInfoTrack_[id]->second); }

QString ModelAudio::getNextIdTrack(const QString& id)
{
    auto iter = m_hashInfoTrack_[id];
    auto iterEnd = iter;
    for(auto i = iter + 1; i != m_vecInfoTrack_.end(); i++)
        if(i->first != true)
            return m_hashInfoTrack_.key(i);

    for(auto i = m_vecInfoTrack_.begin(); i != iterEnd; i++)
        if(i->first != true)
            return m_hashInfoTrack_.key(i);
    return id;
}

QString ModelAudio::getPrevIdTrack(const QString& id)
{
    auto iter = m_hashInfoTrack_[id];
    auto iterBegin = iter;
    for(auto i = iter - 1; i >= m_vecInfoTrack_.begin(); i--)
        if(i->first != true)
            return m_hashInfoTrack_.key(i);

    for(auto i = m_vecInfoTrack_.end() - 1; i >= iterBegin; i--)
        if(i->first != true)
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
        if(m_vecInfoTrack_[currentIndex].first != true)
                return idRandom;
    }
    return QString();
}

std::tuple<IdUser, QString, QPixmap> ModelAudio::getInfoMy() const
{ return std::make_tuple(m_infoMy.first, m_infoMy.second.first, m_infoMy.second.second); }

void ModelAudio::setHideTrack(const QString& id, bool value)
{ m_hashInfoTrack_[id]->first = value; }

void ModelAudio::findPlaylist(const QString& token)
{
    QNetworkAccessManager* loadFriend = new QNetworkAccessManager(this);
    m_token = token;

    QUrlQuery queryFriend("https://api.vk.com/method/friends.get.xml");
    queryFriend.addQueryItem("v", "5.24");
    queryFriend.addQueryItem("access_token", m_token);

    QString result = queryFriend.toString();
    for(int i = 0; i < result.length(); i++)
        if(result[i] == '&')
        {
            result.replace(i, 1, '?');
            break;
        }

    QEventLoop loop;
    QNetworkReply* reply = loadFriend->get(QNetworkRequest(QUrl(result)));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
       // std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadFriend, &QNetworkAccessManager::finished, this, &ModelAudio::parserFriend);
    this->connect(reply, &QNetworkReply::finished, loadFriend, &QNetworkAccessManager::deleteLater);
    loop.exec();
}

void ModelAudio::globalSearchAudio(const QString& artist)
{
    QNetworkAccessManager* loadGlobalAudio = new QNetworkAccessManager(this);
    m_vecInfoTrack_.clear();
    m_hashInfoTrack_.clear();
    QUrlQuery query("https://api.vk.com/method/audio.search.xml");
    query.addQueryItem("q", artist);
    query.addQueryItem("performer_only", "1");
    query.addQueryItem("count", "300");
    query.addQueryItem("v", "5.24");
    query.addQueryItem("access_token", m_token);

    QString result = query.toString();
    for(int i = 0; i < result.length(); i++)
        if(result[i] == '&')
        {
            result.replace(i, 1, '?');
            break;
        }

    QEventLoop loop;
    QNetworkReply* reply = loadGlobalAudio->get(QNetworkRequest(QUrl(result)));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
      //  std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadGlobalAudio, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
    this->connect(reply, &QNetworkReply::finished, loadGlobalAudio, &QNetworkAccessManager::deleteLater);
    loop.exec();
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
    QVector<std::tuple<IdTrack, Artist, Title, Duration>> infoTrack;
    for(auto i = m_vecInfoTrack_.begin(); i != m_vecInfoTrack_.end(); i++)
    {
        IdTrack id = m_hashInfoTrack_.key(i);
        Artist artist;
        Title title;
        Duration duration;
        std::tie(artist, title, duration, std::ignore) = (*i).second;
        infoTrack.push_back(std::make_tuple(id, artist, title, duration));
    }
    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(&Observer::AbstractObserver::updatePlaylist, std::placeholders::_1, infoTrack));
}

void ModelAudio::notifyFriendObservers()
{
    QVector<std::tuple<IdUser, QString, QPixmap>> listFriend;
    for(auto iter = m_infoFriend_.begin(); iter != m_infoFriend_.end(); iter++)
        listFriend.push_back(std::make_tuple(iter.key(), iter.value().first, iter.value().second));
    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(&Observer::AbstractObserver::updateListFriend, std::placeholders::_1, listFriend));
}

void ModelAudio::getPlaylistMy()
{ getPlaylistFriend(m_infoMy.first); }

void ModelAudio::getPlaylistFriend(const QString& id)
{
    QNetworkAccessManager* loadAudio = new QNetworkAccessManager(this);
    m_vecInfoTrack_.clear();
    m_hashInfoTrack_.clear();
    QUrlQuery queryAudio("https://api.vk.com/method/audio.get.xml");
    queryAudio.addQueryItem("owner_id", id);
    queryAudio.addQueryItem("v", "5.24");
    queryAudio.addQueryItem("access_token", m_token);

    QString result = queryAudio.toString();
    for(int i = 0; i < result.length(); i++)
        if(result[i] == '&')
        {
            result.replace(i, 1, '?');
            break;
        }

    QEventLoop loop;
    QNetworkReply* reply = loadAudio->get(QNetworkRequest(QUrl(result)));
    this->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //this->connect(reply, &QNetworkReply::downloadProgress, std::bind(&ModelAudio::progressDownload, this,
       // std::bind(std::divides<qint64>(), std::bind(std::multiplies<qint64>(), 100, std::placeholders::_1), std::placeholders::_2)));
    this->connect(loadAudio, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
    this->connect(reply, &QNetworkReply::finished, loadAudio, &QNetworkAccessManager::deleteLater);
    loop.exec();
}
