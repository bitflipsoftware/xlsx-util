//
//  main.cpp
//  ValParseTest
//
//  Created by mjb on 6/17/18.
//  Copyright © 2018 mjb. All rights reserved.
//

#include <iostream>
#include "Val.h"

int main(int argc, const char * argv[])
{
    xlsx::Val v;
    std::string sci = "1E-3";
    v.setParse( sci );
    std::cout << v.getDouble() << std::endl;
    return 0;
}
