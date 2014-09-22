#include "propertymodelaudio.h"

PropertyModelAudio::PropertyModelAudio(QObject* parent) : QObject(parent)
{ }

PropertyModelAudio::PropertyModelAudio(const QString& artist, const QString& title, const QString& duration, const QString& id, QObject* parent) : QObject(parent)
    , m_artist(artist)
    , m_title(title)
    , m_duration(duration)
    , m_idTrack(id)
{ }

QString PropertyModelAudio::artist() const
{ return m_artist; }

void PropertyModelAudio::setArtist(const QString& artist)
{
    if(artist != m_artist)
    {
        m_artist = artist;
        emit artistChanged();
    }
}

QString PropertyModelAudio::title() const
{ return m_title; }

void PropertyModelAudio::setTitle(const QString& title)
{
    if(title != m_title)
    {
        m_title = title;
        emit titleChanged();
    }
}

QString PropertyModelAudio::duration() const
{ return m_duration; }

void PropertyModelAudio::setDuration(const QString& duration)
{
    if(duration != m_duration)
    {
        m_duration = duration;
        emit durationChanged();
    }
}

QString PropertyModelAudio::idTrack() const
{ return m_idTrack; }

void PropertyModelAudio::setIdTrack(const QString& idTrack)
{
    if(idTrack != m_idTrack)
    {
        m_idTrack = idTrack;
        emit idTrackChanged();
    }
}
