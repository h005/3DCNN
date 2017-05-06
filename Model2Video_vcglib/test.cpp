#include "test.h"

#include <iostream>

Test::Test(QString a1, QString a2)
{
    f1 = a1;
    f2 = a2;
}

void Test::show()
{
    std::cout << f1.toStdString() << std::endl;
    std::cout << f2.toStdString() << std::endl;
}


