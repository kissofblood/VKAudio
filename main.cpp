#include "vkaudio.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VkAudio vk;
    vk.show();

    return a.exec();
}
