#include "QDebug"
#include "cimageresourcemanager.h"

/**
 * @brief CImageResourceManager::CImageResourceManager  Default constructor.
 */
CImageResourceManager::CImageResourceManager()
{
#define GET_IMAGE_RESOURCE_PATH_QSTRING(QStringImageFilePath)                   \
    (QString(":/resources/images/") + QStringImageFilePath)

#define GET_IMAGE_RESOURCE_PATH(ImageFilePath)                                  \
    (GET_IMAGE_RESOURCE_PATH_QSTRING(QString(ImageFilePath)))

    //Setup brake state with light off in auto mode.
    mImageResourcePath[0][0] = GET_IMAGE_RESOURCE_PATH("bicycle_normal.png");
    mImageResourcePath[0][1] = GET_IMAGE_RESOURCE_PATH("bicycle_brake_front.png");
    mImageResourcePath[0][2] = GET_IMAGE_RESOURCE_PATH("bicycle_brake_rear.png");
    mImageResourcePath[0][3] = GET_IMAGE_RESOURCE_PATH("bicycle_brake_front_rear.png");

    //Setup brake state with light on in auto mode.
    mImageResourcePath[1][0] = GET_IMAGE_RESOURCE_PATH("bicycle_auto_light_on.png");
    mImageResourcePath[1][1] = GET_IMAGE_RESOURCE_PATH("bicycle_auto_light_on_brake_front.png");
    mImageResourcePath[1][2] = GET_IMAGE_RESOURCE_PATH("bicycle_auto_light_on_brake_rear.png");
    mImageResourcePath[1][3] = GET_IMAGE_RESOURCE_PATH("bicycle_auto_light_on_brake_rear_brake.png");

    //Setup brake state with light off in manual mode, in other words, light off in request mode.
    mImageResourcePath[2][0] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_off.png");
    mImageResourcePath[2][1] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_off_brake_front.png");
    mImageResourcePath[2][2] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_off_brake_rear.png");
    mImageResourcePath[2][3] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_off_brake_rear_front.png");

    //Setup brake state with light on in manual mode, in other words, light on in request mode.
    mImageResourcePath[3][0] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_on.png");
    mImageResourcePath[3][1] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_on_brake_front.png");
    mImageResourcePath[3][2] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_on_brake_rear.png");
    mImageResourcePath[3][3] = GET_IMAGE_RESOURCE_PATH("bicycle_manual_light_on_brake_rear_front.png");
}

QString CImageResourceManager::getImageResourcePath(int lightImageIndex, int brakeImageIndex)
{
    return this->mImageResourcePath[lightImageIndex][brakeImageIndex];
}

/**
 * @brief CImageResourceManager::getImageResourcePath   Get path of image resource.
 * @param imageResource Data used to select, decide image file path.
 * @return  Path of image file.
 */
QString CImageResourceManager::getImageResourcePath(CImageResource &imageResource)
{
    return this->mImageResourcePath[imageResource.getLightState()][imageResource.getBrakeState()];
}

/**
 * @brief CImageResourceManager::getImageResource   Get image data in QPix format.
 * @param imageResource Data used to select, decide image file path.
 * @return Image data shows the state of bicycle.
 */
QPixmap CImageResourceManager::getImageResource(CImageResource &imageResource)
{
    QString imageFilePath = this->getImageResourcePath(imageResource);

    return QPixmap(imageFilePath);
}
