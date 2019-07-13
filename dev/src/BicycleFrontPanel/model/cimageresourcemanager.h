#ifndef CIMAGERESOURCEMANAGER_H
#define CIMAGERESOURCEMANAGER_H
#include <QString>
#include <QPixmap>
#include "model/cimageresource.h"

class CImageResourceManager
{
public:
    CImageResourceManager();
    ~CImageResourceManager() {}

public://Getter/Setter
    QString getImageResourcePath(int lightImageIndex, int brakeImageIndex);
    QString getImageResourcePath(CImageResource &imageResource);
    QPixmap getImageResource(CImageResource &imageResource);

protected:
    QString mImageResourcePath[4][4];   //Brake state and light state.
};

#endif // CIMAGERESOURCEMANAGER_H
