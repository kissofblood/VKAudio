#include "dataobject.h"

DataObject::DataObject(QObject* parent) : QObject(parent)
{ }

DataObject::DataObject(const QString& artist, const QString& title, const QString& duration, const QString& id, QObject* parent) : QObject(parent)
    , m_artist(artist)
    , m_title(title)
    , m_duration(duration)
    , m_idTrack(id)
{ }

QString DataObject::artist() const
{ return m_artist; }

void DataObject::setArtist(const QString& artist)
{
    if(artist != m_artist)
    {
        m_artist = artist;
        emit artistChanged();
    }
}

QString DataObject::title() const
{ return m_title; }

void DataObject::setTitle(const QString& title)
{
    if(title != m_title)
    {
        m_title = title;
        emit titleChanged();
    }
}

QString DataObject::duration() const
{ return m_duration; }

void DataObject::setDuration(const QString& duration)
{
    if(duration != m_duration)
    {
        m_duration = duration;
        emit durationChanged();
    }
}

QString DataObject::idTrack() const
{ return m_idTrack; }

void DataObject::setIdTrack(const QString& idTrack)
{
    if(idTrack != m_idTrack)
    {
        m_idTrack = idTrack;
        emit idTrackChanged();
    }
}

bool DataObject::hide() const
{ return m_hide; }

void DataObject::setHide(bool hide)
{
    if(hide != m_hide)
    {
        m_hide = hide;
        emit hideChanged();
    }
}


