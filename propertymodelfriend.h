#ifndef PROPERTYMODELFRIEND_H
#define PROPERTYMODELFRIEND_H

#include <QObject>
#include <QString>

class PropertyModelFriend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nameFriend READ nameFriend WRITE setNameFriend NOTIFY nameFriendChanged)
    Q_PROPERTY(QString idFriend READ idFriend WRITE setIdFriend NOTIFY idFriendChanged)
public:
    explicit PropertyModelFriend(QObject* parent = nullptr);
    PropertyModelFriend(const QString& name, const QString& id, QObject* parent = nullptr);
    ~PropertyModelFriend() override = default;

    QString nameFriend() const;
    void setNameFriend(const QString& name);
    QString idFriend() const;
    void setIdFriend(const QString& id);
    QImage avatarFriend() const;
    void setAvatarFriend(const QImage& avatar);

signals:
    void nameFriendChanged();
    void idFriendChanged();
    void avatarFriendChanged();

private:
    QString m_name;
    QString m_id;
};

#endif // PROPERTYMODELFRIEND_H
