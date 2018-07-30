//
//  lemsibutil.cpp
//  The Switcher Project
//
//  Created by Lemark on 11/25/14.
//  Copyright (c) 2014 Mr. McByte. All rights reserved.
//

#include "lemsibutil.h"

int countArrayElements(int sourceArray[])
{
    int number_of_elements=0;
    
    for(;sourceArray[number_of_elements] != END_OF_ARRAY;number_of_elements++);
    
    return number_of_elements;
}