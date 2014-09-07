#include "model.h"

Model* Model::m_singleton = nullptr;

Model::~Model()
{
    delete m_manager;
    delete m_singleton;
}

Model* Model::getInstance()
{
    if(m_singleton == nullptr)
        m_singleton = new Model;
    return m_singleton;
}

QUrl Model::findUrlTrack(int id)
{
    auto iter = std::find_if(m_infoTrack_.begin(), m_infoTrack_.end(), [id](const std::tuple<Id, Artist, Title, Duration, QUrl>& val)
    { return std::get<0>(val) == id; });
    if(iter != m_infoTrack_.end())
        return std::get<4>(*iter);
    return QUrl();
}

QString Model::findNameTrack(int id)
{
    auto iter = std::find_if(m_infoTrack_.begin(), m_infoTrack_.end(), [id](const std::tuple<Id, Artist, Title, Duration, QUrl>& val)
    { return std::get<0>(val) == id; });
    if(iter != m_infoTrack_.end())
        return std::get<1>(*iter) + " - " + std::get<2>(*iter);
    return QString();
}

void Model::setUrlAudio(const QString& token)
{
    QUrlQuery queryAudio("https://api.vk.com/method/audio.get.xml");
    queryAudio.addQueryItem("v", "5.24");
    queryAudio.addQueryItem("access_token", token);

    QString result = queryAudio.toString();
    for(int i = 0; i < result.length(); i++)
        if(result[i] == '&')
        {
            result.replace(i, 1, '?');
            break;
        }

    m_manager->get(QNetworkRequest(QUrl(result)));

    QObject::connect(m_manager, &QNetworkAccessManager::finished, std::bind(&Model::parserXml, this, std::placeholders::_1));
}

void Model::registerObserver(Observer::AbstractObserver* observer)
{ m_observer_.push_back(observer); }

void Model::removeObserver(Observer::AbstractObserver* observer)
{
    auto iterRemove = qFind(m_observer_.begin(), m_observer_.end(), observer);
    if(iterRemove != m_observer_.end())
        m_observer_.erase(iterRemove);
}

void Model::notifyObservers()
{
    QVector<std::tuple<Id, Artist, Title, Duration>> infoTrack;
    for(auto& info : m_infoTrack_)
    {
        Id id;
        Artist artist;
        Title title;
        Duration duration;
        std::tie(id, artist, title, duration, std::ignore) = info;
        infoTrack.push_back(std::make_tuple(id, artist, title, duration));
    }

    std::for_each(m_observer_.begin(), m_observer_.end(), std::bind(
                      &Observer::AbstractObserver::updateListTrack, std::placeholders::_1, infoTrack));
}

void Model::parserXml(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());
    QDomNode nodeAudio = doc.documentElement().firstChild().nextSibling().firstChild();
    while(!nodeAudio.isNull())
    {
        QDomNode nodeInfoTrack = nodeAudio.toElement().firstChild();
        Id id;
        Artist artist;
        Title title;
        Duration duration;
        QUrl url;
        while(!nodeInfoTrack.isNull())
        {
            QDomElement element = nodeInfoTrack.toElement();
            if(element.tagName() == "id")
                id = element.text().toInt();
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

        m_infoTrack_.push_back(std::make_tuple(id, artist, title, duration, url));
        nodeAudio = nodeAudio.nextSibling();
    }

    notifyObservers();
}
