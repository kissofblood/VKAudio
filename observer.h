#ifndef OBSERVER_H
#define OBSERVER_H

#include <QVector>
#include <QString>
#include <QUrl>
#include <tuple>

using IdUser    = QString;
using IdTrack   = QString;
using Artist    = QString;
using Title     = QString;
using Duration  = int;

namespace Observer {

class AbstractObserver
{
public:
    AbstractObserver() = default;
    virtual ~AbstractObserver() = default;

    virtual void updateListFriend(const QVector<std::tuple<IdUser, QString, QUrl>>& listFriend) = 0;
    virtual void updatePlaylist(const QVector<std::tuple<IdTrack, Artist, Title, Duration, IdUser>>& infoTrack) = 0;
};

class AbstractObservable
{
public:
    AbstractObservable() = default;
    virtual ~AbstractObservable() = default;

    virtual void registerObserver(AbstractObserver* observer) = 0;
    virtual void removeObserver(AbstractObserver* observer) = 0;
    virtual void notifyAudioObservers() = 0;
    virtual void notifyFriendObservers() = 0;
};

}

#endif // OBSERVER_H
