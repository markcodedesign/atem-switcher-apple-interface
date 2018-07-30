//
//  Test.h
//  The Switcher Project
//
//  Created by Lemark on 11/14/14.
//  Copyright (c) 2014 Mr. McByte. All rights reserved.
//

#ifndef The_Switcher_Project_Test_h
#define The_Switcher_Project_Test_h
#include <iostream>
#include "BMDSwitcherAPI.h"
#include "lemsibutil.h"

typedef void (*javaCallbackFunction)(void);


extern "C" int testSampleErrorCode();
extern "C" void testPointerParameter(int* value);
extern "C" bool testSystem();
extern "C" int testParameterString(const char* any_string);
extern "C" const char* testParameterReturnString(const char* any_string);
extern "C" const char* testReturnCStringFromCFString();

extern "C" void test_enumerateInputArrayGetSize(int* out_array_size);
extern "C" void test_enumerateInputArray(const char* out_input_name_array[],long long out_input_id_array[],const char* out_input_port_type_array[]);


extern "C" void addJavaCallback(int watch_for_event,javaCallbackFunction sigfunc);
extern "C" void raiseEvent(int raise_event);

class SwitcherCallback {
    
public:
    void switchCall();
};

#endif
