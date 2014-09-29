#include "vkaudio.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VkAudio vk;
    vk.setMinimumSize(935, 76);
    vk.showMaximized();

    return a.exec();
}
