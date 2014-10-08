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

bool PropertyModelAudio::visibleColorAdd() const
{ return m_visibleColorAdd; }

void PropertyModelAudio::setVisibleColorAdd(bool value)
{
    if(value != m_visibleColorAdd)
    {
        m_visibleColorAdd = value;
        emit visibleColorAddChanged();
    }
}

bool PropertyModelAudio::visibleColorRemove() const
{ return m_visibleColorRemove; }

void PropertyModelAudio::setVisibleColorRemove(bool value)
{
    if(value != m_visibleColorRemove)
    {
        m_visibleColorRemove = value;
        emit visibleColorRemoveChanged();
    }
}

bool PropertyModelAudio::visibleColorItem() const
{ return m_visibleColorItem; }

void PropertyModelAudio::setVisibleColorItem(bool value)
{
    if(value != m_visibleColorItem)
    {
        m_visibleColorItem = value;
        emit visibleColorItemChanged();
    }
}

bool PropertyModelAudio::visibleImageAdd() const
{ return m_visibleImageAdd; }

void PropertyModelAudio::setVisibleImageAdd(bool value)
{
    if(value != m_visibleImageAdd)
    {
        m_visibleImageAdd = value;
        emit visibleImageAddChanged();
    }
}

bool PropertyModelAudio::visibleImageCancel() const
{ return m_visibleImageCancel; }

void PropertyModelAudio::setVisibleImageCancel(bool value)
{
    if(value != m_visibleImageCancel)
    {
        m_visibleImageCancel = value;
        emit visibleImageCancelChanged();
    }
}

bool PropertyModelAudio::enableMouse() const
{ return m_enableMouse; }

void PropertyModelAudio::setEnableMouse(bool value)
{
    if(value != m_enableMouse)
    {
        m_enableMouse = value;
        emit enableMouseChanged();
    }
}
