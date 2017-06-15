#include "boorupixmapdownloader.h"

BooruPixmapDownloader::BooruPixmapDownloader(BooruPicture* pic, bool overwrite, QObject *parent) : QObject(parent)
{
    this->url = QString(pic->getThumbnailUrl().c_str());
    this->path = QString(pic->getThumbnailPath().c_str());
    this->id = pic->getID();
    this->overwrite = overwrite;
}

BooruPixmapDownloader::BooruPixmapDownloader(QString url, QString path, bool overwrite, QObject *parent) : QObject(parent)
{
    this->url = url;
    this->path = path;
    id = 0;
    this->overwrite = overwrite;
}

void BooruPixmapDownloader::process()
{
    ConnectionManager* c = new ConnectionManager();
    c->downloadFile(this->url,this->path,this->overwrite);
    QPixmap p(this->path);
    delete c;

    emit pixmapLoaded(p);
    emit finished();
}
