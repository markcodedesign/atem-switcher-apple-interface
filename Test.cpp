//
//  Test.cpp
//  The Switcher Project
//
//  Created by Lemark on 11/14/14.
//  Copyright (c) 2014 Mr. McByte. All rights reserved.
//

#include "Test.h"
#include <dlfcn.h>
#define EXPORT __attribute__((visibility("default")))



javaCallbackFunction javaCallback=nullptr;
int event=0;

void SwitcherCallback::switchCall()
{
    javaCallback();
}

EXPORT
void addJavaCallback(int watch_for_event,javaCallbackFunction sigfunc)
{
    event = watch_for_event;
    javaCallback = sigfunc;
}


EXPORT
void raiseEvent(int raise_event)
{
    SwitcherCallback *switchCall = new SwitcherCallback();
    if(event == raise_event)
        switchCall->switchCall();
    
    delete switchCall;
}

EXPORT
void testPointerParameter(int* value)
{
    int testArray[] = {'11','22','33','44','55','aa','bb','cc','dd','ee',END_OF_ARRAY};
    int x=countArrayElements(testArray);
    
    int give_value = x;
    
    *value = give_value;
}

EXPORT
int testSampleErrorCode()
{
    return 'AAA';
}

EXPORT
bool testSystem()
{
    return true;
}

EXPORT
int testParameterString(const char* any_string)
{
    const char* my_string = any_string;
    
    return 0;
}

EXPORT
const char* testParameterReturnString(const char* any_string)
{
    const char* my_string = any_string;
    
    return my_string;
}

CFStringRef test_switchOneName;
char* test_switchOneName2=nullptr;
CFStringRef test_switchOneName3;

void test_switchOneNameFunction(CFStringRef* pointer_to_name)
{
    int size = sizeof("This is my String ");
   
    const char* name = (const char*)malloc(size);
    memset((void*)name, NULL, size);
    memcpy((void*)name,"This is my String ", size);
    *pointer_to_name = CFStringCreateWithCString(kCFAllocatorDefault,name, kCFStringEncodingUTF8);
    
  
}

EXPORT
const char* testReturnCStringFromCFString()
{
    test_switchOneNameFunction(&test_switchOneName);
    int size = sizeof("This is my String ");
    test_switchOneName2 = (char*)malloc(size);
    memset((void*)test_switchOneName2, NULL, size);
    const char* name = CFStringGetCStringPtr(test_switchOneName, NULL);
    return  name;
   
}

#define NUMBER_OF_INPUTS 10
long long* test_masterInputIdArray;

EXPORT
void test_enumerateInputArrayGetSize(int* out_array_size)
{
    *out_array_size = NUMBER_OF_INPUTS;
}

EXPORT
void test_enumerateInputArray(const char* out_input_name_array[],long long out_input_id_array[],const char* out_input_port_type_array[])
{
    const char* inputName = "Hello";
    const char* portName = "(port)";
    CFStringRef inputNameString[NUMBER_OF_INPUTS];
    long long inputID[NUMBER_OF_INPUTS];
    
    for(int x=0;x<NUMBER_OF_INPUTS;x++)
    {
        inputNameString[x] = NULL;
        inputID[x] = NULL;
    }
    
    for(int x=0;x<NUMBER_OF_INPUTS;x++)
    {
        inputNameString[x] = CFStringCreateWithCString(kCFAllocatorDefault,"(Input) ",kCFStringEncodingUTF8);
        if(inputNameString[x] == NULL)
            inputName = "It is NULL!";
        else
        inputName = CFStringGetCStringPtr(inputNameString[x], NULL);
        
        out_input_port_type_array[x] = portName;
        out_input_id_array[x] = inputID[x] = x;
        out_input_name_array[x] = inputName;
        
    }
    test_masterInputIdArray = out_input_id_array;
    
        for(int y=0;y<NUMBER_OF_INPUTS;y++)
        test_masterInputIdArray[y] += out_input_id_array[y];
    
    out_input_id_array[0] = test_masterInputIdArray[NUMBER_OF_INPUTS-1];

}

