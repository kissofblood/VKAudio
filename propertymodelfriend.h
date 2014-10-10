#ifndef PROPERTYMODELFRIEND_H
#define PROPERTYMODELFRIEND_H

#include <QObject>
#include <QString>
#include <QUrl>

class PropertyModelFriend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nameFriend READ nameFriend WRITE setNameFriend NOTIFY nameFriendChanged)
    Q_PROPERTY(QString idFriend READ idFriend WRITE setIdFriend NOTIFY idFriendChanged)
    Q_PROPERTY(QUrl urlAvatar READ urlAvatar WRITE setUrlAvatar NOTIFY urlAvatarChanged)
public:
    explicit PropertyModelFriend(QObject* parent = nullptr);
    PropertyModelFriend(const QString& name, const QString& id, const QUrl& url, QObject* parent = nullptr);
    ~PropertyModelFriend() override = default;

    QString nameFriend() const;
    void setNameFriend(const QString& name);
    QString idFriend() const;
    void setIdFriend(const QString& id);
    QUrl urlAvatar() const;
    void setUrlAvatar(const QUrl& url);

signals:
    void nameFriendChanged();
    void idFriendChanged();
    void urlAvatarChanged();

private:
    QString m_name;
    QString m_id;
    QUrl    m_urlAvatar;
};

#endif // PROPERTYMODELFRIEND_H
