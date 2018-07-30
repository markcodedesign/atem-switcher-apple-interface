//
//  lemsibatem.h
//  The Switcher Project
//
//  Created by Lemark on 11/26/14.
//  Copyright (c) 2014 Mr. McByte. All rights reserved.
//

#ifndef __The_Switcher_Project__lemsibatem__
#define __The_Switcher_Project__lemsibatem__

#include <iostream>
#include "BMDSwitcherAPI.h"
#include "lemsibutil.h"

extern "C" bool initSwitcher();
extern "C" void exitSwitcher();

extern "C" int connectToSwitcher(int switcherNumber,const char* ipAddress);
extern "C" int* getSwitcherConnectToFailureCodesArray(int* out_array_size);
extern "C" const char* getSwitcherName(int switch_number);

extern "C" void enumerateInputArrayGetSize(int* out_array_size);
extern "C" void enumerateInputArray(const char* out_input_name_array[],long long out_input_id_array[],const char* out_input_port_type_array[]);

extern "C" void performCut(long long source_input_id,long long destination_output_id);
extern "C" void performCutByIndex(int src_index,int dest_index);
extern "C" void performAuxOutputSourceChange(long long source_input_id,long long aux_output_id);
extern "C" void performAuxOutputSourceChangeByIndex(int src_index,int dest_index);

const char* getInputPortTypeName(BMDSwitcherPortType port_type);

#endif /* defined(__The_Switcher_Project__lemsibatem__) */
