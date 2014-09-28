#ifndef AVATARPROVIDER_H
#define AVATARPROVIDER_H

#include <QPixmap>
#include <QHash>
#include <QSize>
#include <QString>
#include <QtQuick/QQuickImageProvider>

class AvatarProvider : public QQuickImageProvider
{
public:
    AvatarProvider(const QHash<QString, QPixmap>&& avatar);
    ~AvatarProvider() override = default;

    QPixmap requestPixmap(const QString& id, QSize*, const QSize&) override;

private:
    QHash<QString, QPixmap> m_avatar_;
};

#endif // AVATARPROVIDER_H
