#include "avatarprovider.h"

AvatarProvider::AvatarProvider(const QHash<QString, QPixmap>& avatar) : QQuickImageProvider(QQuickImageProvider::Pixmap)
    , m_avatar_(avatar)
{ }

QPixmap AvatarProvider::requestPixmap(const QString& id, QSize*, const QSize&)
{ return m_avatar_[id]; }
