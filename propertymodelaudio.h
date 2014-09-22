#ifndef PROPERTYMODELAUDIO_H
#define PROPERTYMODELAUDIO_H

#include <QObject>
#include <QString>

class PropertyModelAudio : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString idTrack READ idTrack WRITE setIdTrack NOTIFY idTrackChanged)
public:
    explicit PropertyModelAudio(QObject* parent = nullptr);
    PropertyModelAudio(const QString& artist, const QString& title, const QString& duration, const QString& id, QObject* parent = nullptr);
    ~PropertyModelAudio() override = default;

    QString artist() const;
    void setArtist(const QString& artist);
    QString title() const;
    void setTitle(const QString& title);
    QString duration() const;
    void setDuration(const QString& duration);
    QString idTrack() const;
    void setIdTrack(const QString& idTrack);

signals:
    void artistChanged();
    void titleChanged();
    void durationChanged();
    void idTrackChanged();

private:
    QString m_artist;
    QString m_title;
    QString m_duration;
    QString m_idTrack;
};

#endif // PROPERTYMODELAUDIO_H
