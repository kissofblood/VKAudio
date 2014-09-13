#include "modelaudio.h"

ModelAudio* ModelAudio::m_singleton = nullptr;

ModelAudio::ModelAudio() : QObject(nullptr)
{ }

void ModelAudio::parserAudio(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode nodeAudio = doc.documentElement().firstChild().nextSibling().firstChild();
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

        m_vecInfoTrack_.push_back(qMakePair(true, std::make_tuple(artist, title, duration, url)));
        m_hashInfoTrack_.insert(id, m_vecInfoTrack_.end() - 1);
        nodeAudio = nodeAudio.nextSibling();
    }

    notifyAudioObservers();
}

void ModelAudio::parserFriend(QNetworkReply* reply)
{
    std::for_each(m_loadIcon_.begin(), m_loadIcon_.end(), std::bind(&QNetworkAccessManager::deleteLater, std::placeholders::_1));
    m_loadIcon_.clear();
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
    queryUserMy.addQueryItem("v", "5.24");
    queryUserMy.addQueryItem("fields", "photo_50");
    queryUserMy.addQueryItem("access_token", m_token);
    QString resultMy = queryUserMy.toString();
    for(int i = 0; i < resultMy.length(); i++)
        if(resultMy[i] == '&')
        {
            resultMy.replace(i, 1, '?');
            break;
        }
    m_loadUser_.push_back(new QNetworkAccessManager);
    m_loadUser_.back()->get(QNetworkRequest(resultMy));
    this->connect(m_loadUser_.back(), &QNetworkAccessManager::finished, [this](QNetworkReply* reply)
    {
         m_infoMy = getResultParserUser(reply->readAll());
         emit loadTrue();
    });

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
        m_loadUser_.push_back(new QNetworkAccessManager);
        m_loadUser_.back()->get(QNetworkRequest(url));
        this->connect(m_loadUser_.back(), &QNetworkAccessManager::finished, this, &ModelAudio::parserUser);
    }
}

void ModelAudio::parserUser(QNetworkReply* reply)
{
    auto result = getResultParserUser(reply->readAll());
    m_infoFriend_.insert(result.first, qMakePair(result.second.first, result.second.second));

    if(m_countFriend == m_infoFriend_.size())
        notifyFriendObservers();
}

QPair<IdUser, QPair<QString, QIcon>> ModelAudio::getResultParserUser(const QByteArray& array)
{
    m_loadIcon_.push_back(new QNetworkAccessManager);
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
    m_loadIcon_.back()->get(QNetworkRequest(photo));

    this->connect(m_loadIcon_.back(), &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    this->connect(m_loadIcon_.back(), &QNetworkAccessManager::finished, [&pix](QNetworkReply* reply)
    { pix.loadFromData(QByteArray(reply->readAll()), "jpg"); });
    loop.exec();
    return qMakePair(id, qMakePair(name, QIcon(pix)));
}

ModelAudio::~ModelAudio()
{
    std::for_each(m_loadUser_.begin(), m_loadUser_.end(), std::bind(&QNetworkAccessManager::deleteLater, std::placeholders::_1));
    std::for_each(m_loadIcon_.begin(), m_loadIcon_.end(), std::bind(&QNetworkAccessManager::deleteLater, std::placeholders::_1));
    delete m_loadAudio;
    delete m_loadFriend;
    delete m_singleton;
}

ModelAudio* ModelAudio::getInstance()
{
    if(m_singleton == nullptr)
        m_singleton = new ModelAudio;
    return m_singleton;
}

QUrl ModelAudio::findUrlTrack(const QString& id)
{

}

QString ModelAudio::findNameTrack(int id)
{

}

int ModelAudio::getNextIdTrack(int id)
{

}

int ModelAudio::getRandomIdTrack(int id)
{

}

void ModelAudio::findPlaylist(const QString& token)
{
    m_token = token;
    std::for_each(m_loadUser_.begin(), m_loadUser_.end(), std::bind(&QNetworkAccessManager::deleteLater, std::placeholders::_1));
    m_loadUser_.clear();

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

    m_loadFriend->get(QNetworkRequest(QUrl(result)));

    this->connect(m_loadFriend, &QNetworkAccessManager::finished, this, &ModelAudio::parserFriend);
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
    QVector<std::tuple<IdUser, QString, QIcon>> listFriend;
    for(auto iter = m_infoFriend_.begin(); iter != m_infoFriend_.end(); iter++)
        listFriend.push_back(std::make_tuple(iter.key(), iter.value().first, iter.value().second));
    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(&Observer::AbstractObserver::updateListFriend, std::placeholders::_1, listFriend));
}

void ModelAudio::getPlaylistMy()
{ getPlaylistFriend(m_infoMy.first); }

void ModelAudio::getPlaylistFriend(const QString& id)
{
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
    m_loadAudio->get(QNetworkRequest(QUrl(result)));
    this->connect(m_loadAudio, &QNetworkAccessManager::finished, this, &ModelAudio::parserAudio);
}