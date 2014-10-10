#include "propertymodelfriend.h"

PropertyModelFriend::PropertyModelFriend(QObject* parent) : QObject(parent)
{ }

PropertyModelFriend::PropertyModelFriend(const QString& name, const QString& id, const QUrl& url, QObject* parent) : QObject(parent)
    , m_name(name)
    , m_id(id)
    , m_urlAvatar(url)
{ }

QString PropertyModelFriend::nameFriend() const
{ return m_name; }

void PropertyModelFriend::setNameFriend(const QString& name)
{
    if(name != m_name)
    {
        m_name = name;
        emit nameFriend();
    }
}

QString PropertyModelFriend::idFriend() const
{ return m_id; }

void PropertyModelFriend::setIdFriend(const QString& id)
{
    if(id != m_id)
    {
        m_id = id;
        emit idFriendChanged();
    }
}

QUrl PropertyModelFriend::urlAvatar() const
{ return m_urlAvatar; }

void PropertyModelFriend::setUrlAvatar(const QUrl& url)
{
    if(url != m_urlAvatar)
    {
        m_urlAvatar = url;
        emit urlAvatarChanged();
    }
}
