#include "vkaudio.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VkAudio vk;
    vk.show();

    vk.resize(800, 400);

    return a.exec();
}
