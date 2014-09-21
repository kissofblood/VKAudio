#include "vkaudio.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VkAudio vk;
    vk.resize(800, 600);
    vk.show();

    return a.exec();
}
