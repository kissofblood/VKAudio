#ifndef OBSERVER_H
#define OBSERVER_H

#include <QVector>
#include <tuple>
#include <QString>

namespace Observer {

class AbstractObserver
{
public:
    using Id        = int;
    using Artist    = QString;
    using Title     = QString;
    using Duration  = int;

    AbstractObserver() = default;
    virtual ~AbstractObserver() = default;

    virtual void updateListTrack(const QVector<std::tuple<Id, Artist, Title, Duration>>& infoTrack) = 0;
};

class AbstractObservable
{
public:
    AbstractObservable() = default;
    virtual ~AbstractObservable() = default;

    virtual void registerObserver(AbstractObserver* observer) = 0;
    virtual void removeObserver(AbstractObserver* observer) = 0;
    virtual void notifyObservers() = 0;
};

}

#endif // OBSERVER_H
