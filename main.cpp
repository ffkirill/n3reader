#include "n3readerapplication.h"
#include <iostream>

int main(int argc, char *argv[])
{
    N3ReaderApplication a(argc, argv);
    std::cout.setf(std::ios::unitbuf);
    return a.exec();
}
