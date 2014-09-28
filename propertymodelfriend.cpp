#include "propertymodelfriend.h"

PropertyModelFriend::PropertyModelFriend(QObject* parent) : QObject(parent)
{ }

PropertyModelFriend::PropertyModelFriend(const QString& name, const QString& id, QObject* parent) : QObject(parent)
    , m_name(name)
    , m_id(id)
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
