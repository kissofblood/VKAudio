#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QObject>
#include <QString>

class DataObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString idTrack READ idTrack WRITE setIdTrack NOTIFY idTrackChanged)
    Q_PROPERTY(bool hide READ hide WRITE setHide NOTIFY hideChanged)
public:
    explicit DataObject(QObject* parent = nullptr);
    DataObject(const QString& artist, const QString& title, const QString& duration, const QString& id, QObject* parent = nullptr);
    ~DataObject() override = default;

    QString artist() const;
    void setArtist(const QString& artist);
    QString title() const;
    void setTitle(const QString& title);
    QString duration() const;
    void setDuration(const QString& duration);
    QString idTrack() const;
    void setIdTrack(const QString& idTrack);
    bool hide() const;
    void setHide(bool hide);

signals:
    void artistChanged();
    void titleChanged();
    void durationChanged();
    void idTrackChanged();
    void hideChanged();

private:
    QString m_artist;
    QString m_title;
    QString m_duration;
    QString m_idTrack;
    bool    m_hide = false;
};

#endif // DATAOBJECT_H
