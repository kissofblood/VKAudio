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
    Q_PROPERTY(bool visibleColorItem READ visibleColorItem WRITE setVisibleColorItem NOTIFY visibleColorItemChanged)
    Q_PROPERTY(bool visibleColorAdd READ visibleColorAdd WRITE setVisibleColorAdd NOTIFY visibleColorAddChanged)
    Q_PROPERTY(bool visibleColorRemove READ visibleColorRemove WRITE setVisibleColorRemove NOTIFY visibleColorRemoveChanged)
    Q_PROPERTY(bool visibleImageAdd READ visibleImageAdd WRITE setVisibleImageAdd NOTIFY visibleImageAddChanged)
    Q_PROPERTY(bool visibleImageCancel READ visibleImageCancel WRITE setVisibleImageCancel NOTIFY visibleImageCancelChanged)
    Q_PROPERTY(bool enableMouse READ enableMouse WRITE setEnableMouse NOTIFY enableMouseChanged)
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
    bool visibleColorAdd() const;
    void setVisibleColorAdd(bool value);
    bool visibleColorRemove() const;
    void setVisibleColorRemove(bool value);
    bool visibleColorItem() const;
    void setVisibleColorItem(bool value);
    bool visibleImageAdd() const;
    void setVisibleImageAdd(bool value);
    bool visibleImageCancel() const;
    void setVisibleImageCancel(bool value);
    bool enableMouse() const;
    void setEnableMouse(bool value);

signals:
    void artistChanged();
    void titleChanged();
    void durationChanged();
    void idTrackChanged();
    void visibleColorAddChanged();
    void visibleColorRemoveChanged();
    void visibleColorItemChanged();
    void visibleImageAddChanged();
    void visibleImageCancelChanged();
    void enableMouseChanged();

private:
    QString m_artist;
    QString m_title;
    QString m_duration;
    QString m_idTrack;
    bool    m_visibleColorAdd       = false;
    bool    m_visibleColorRemove    = false;
    bool    m_visibleColorItem      = false;
    bool    m_visibleImageAdd       = false;
    bool    m_visibleImageCancel    = true;
    bool    m_enableMouse           = true;
};

#endif // PROPERTYMODELAUDIO_H
