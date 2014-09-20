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

    QHBoxLayout* box = new QHBoxLayout;
    box->addWidget(m_authorization);
    box->addWidget(m_quickWidget);
    box->setMargin(0);
    this->setLayout(box);

    this->connect(m_modelAudio, &ModelAudio::loadTrue, m_modelAudio, &ModelAudio::getPlaylistMy);
    this->connect(m_authorization, &QWebView::urlChanged, this, &VkAudio::checkUrl);
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
    QList<QObject*> dataList;
    for(auto& track : infoTrack)
    {
        QString id;
        Artist artist;
        Title title;
        Duration duration;
        std::tie(id, artist, title, duration) = track;
        int durationMsec = duration * 1000;
        QTime durationTime(0, (durationMsec / 60000) % 60, (durationMsec / 1000) % 60);
        auto rrr = new DataObject(artist, title, durationTime.toString("mm:ss"), id, this);
        dataList.push_back(rrr);
    }

    QQmlContext* context = m_quickView->rootContext();
    context->setContextProperty("vkAudioModel", QVariant::fromValue(dataList));
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
