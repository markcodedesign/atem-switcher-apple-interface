//
//  lemsibatem.cpp
//  The Switcher Project
//
//  Created by Lemark on 11/26/14.
//  Copyright (c) 2014 Mr. McByte. All rights reserved.
//

#include "lemsibatem.h"

#define EXPORT __attribute__((visibility("default")))

#define NUMBER_OF_INPUTS 30

static IBMDSwitcherDiscovery* switcherDiscovery = NULL;

static IBMDSwitcher* switchOne=NULL;
CFStringRef switchOneName=NULL;

IBMDSwitcher* switchTwo=NULL;
CFStringRef* switchTwoName=NULL;

IBMDSwitcher* switchThree=NULL;
CFStringRef* switchThreeName=NULL;

IBMDSwitcherMixEffectBlock *mixEffectBlock = NULL;

long long masterInputIdArray[NUMBER_OF_INPUTS];
long long* masterInputPortTypesArray=NULL;

int* getSwitcherInputPropertyIdArray(int* out_array_size)
{
    int static inputPropertyId[] = {
        bmdSwitcherInputPropertyIdPortType,
        bmdSwitcherInputPropertyIdInputAvailability,
        bmdSwitcherInputPropertyIdShortName,
        bmdSwitcherInputPropertyIdLongName,
        bmdSwitcherInputPropertyIdIsProgramTallied,
        bmdSwitcherInputPropertyIdIsPreviewTallied,
        bmdSwitcherInputPropertyIdAvailableExternalPortTypes,
        bmdSwitcherInputPropertyIdCurrentExternalPortType,
        END_OF_ARRAY
    };
    
    *out_array_size = countArrayElements(inputPropertyId);
    
    return inputPropertyId;
}

EXPORT
int* getSwitcherConnectToFailureCodesArray(int* out_array_size)
{
    int static connectToFailureCodes[] = {
        bmdSwitcherConnectToFailureCorruptData,
        bmdSwitcherConnectToFailureIncompatibleFirmware,
        bmdSwitcherConnectToFailureNoResponse,
        bmdSwitcherConnectToFailureStateSync,
        bmdSwitcherConnectToFailureStateSyncTimedOut,
        END_OF_ARRAY
    };
    
    *out_array_size = countArrayElements(connectToFailureCodes);
    
    return connectToFailureCodes;
}

EXPORT
bool initSwitcher()
{
    
    if(switcherDiscovery == NULL)
        switcherDiscovery =  CreateBMDSwitcherDiscoveryInstance();
    
    if(switcherDiscovery != NULL)
        return true;
    else
        return false;
}

EXPORT
void exitSwitcher()
{
    if(switchOne)
        switchOne->Release();
    if(switchTwo)
        switchTwo->Release();
    if(switchThree)
        switchThree->Release();
    
    if(mixEffectBlock)
        mixEffectBlock->Release();
}

EXPORT
int connectToSwitcher(int switcherNumber,const char* ipAddress)
{
    if(switcherNumber == 0)
        return 0;
    
    int switchCaseNumber = switcherNumber;
    CFStringRef ipAddressString = CFStringCreateWithCString(kCFAllocatorDefault,ipAddress, kCFStringEncodingASCII);
    BMDSwitcherConnectToFailure reasonForFailure = 0;
    
    switch(switchCaseNumber)
    {
        case 1:
        {
            if(switchOne == NULL){
                if(switcherDiscovery != NULL)
                {
                    HRESULT result;
                    result = switcherDiscovery->ConnectTo(ipAddressString, &switchOne, &reasonForFailure);
                    if(result == S_OK)
                    {
                        switchOne->GetProductName(&switchOneName);
                        IBMDSwitcherMixEffectBlockIterator *mixEffectBlockIterator = NULL;
                        
                        switchOne->CreateIterator(IID_IBMDSwitcherMixEffectBlockIterator,(void**)&mixEffectBlockIterator);
                        
                        if(mixEffectBlockIterator)
                          if(mixEffectBlockIterator->Next(&mixEffectBlock) != S_OK)
                              return -2;
                        
                        return -1;
                    }
                }
                
            }
            
        }break;
            
        case 2:break;
        case 3:break;
            
    }
    
    return reasonForFailure;
}

EXPORT
const char* getSwitcherName(int switch_number)
{
    switch(switch_number)
    {
        case 1:
        {
            const char* name = CFStringGetCStringPtr(switchOneName,NULL);
            return name;
        }
        case 2:break;
        case 3:break;
        default:break;
    }
    
    return NULL;
}

EXPORT void enumerateInputArrayGetSize(int* out_array_size)
{
    *out_array_size = NUMBER_OF_INPUTS;
}

CFStringRef inputNameString[NUMBER_OF_INPUTS];
BMDSwitcherInputId inputID[NUMBER_OF_INPUTS];
int64_t inputPortType[NUMBER_OF_INPUTS];

EXPORT
void enumerateInputArray(const char* out_input_name_array[],long long out_input_id_array[],const char* out_input_port_type_array[])
{

    for(int x=0;x<NUMBER_OF_INPUTS;x++)
    {
        inputNameString[x] = NULL;
        inputID[x] = 0;
        inputPortType[x] = 0;
        
        masterInputIdArray[x] = 0;
    }
    
    if(switchOne)
    {
        IBMDSwitcherInput* input = NULL;
        IBMDSwitcherInputIterator* inputIterator = NULL;
       
        HRESULT result;
        
        result = switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator);
        if(result != S_OK)
            throw (1);
        
        int index=0;
        while(S_OK == inputIterator->Next(&input))
        {
            BMDSwitcherInputId id;
            CFStringRef name;
            const char* inputName = NULL;

            
            result = input->GetInputId(&id);
            if(result != S_OK)
                throw (2);
            
            inputID[index] = id;
            out_input_id_array[index] = id;
            masterInputIdArray[index] = id;
            
            result = input->GetInt(bmdSwitcherInputPropertyIdPortType,&inputPortType[index]);
            if(result != S_OK)
                throw(3);
            
            out_input_port_type_array[index] = getInputPortTypeName((BMDSwitcherPortType)inputPortType[index]);
         
            result = input->GetString(bmdSwitcherInputPropertyIdLongName, &name);
            if(result != S_OK)
                throw(4);
            
            inputNameString[index] = name;
            inputName = CFStringGetCStringPtr(name,NULL);
            if(inputName == NULL)
                throw(5);
            
            out_input_name_array[index] = inputName;

            input->Release();
            index++;
        }
   
        //masterInputIdArray = out_input_id_array;
        masterInputPortTypesArray = inputPortType;
        
        inputIterator->Release();
        
    }
    
}

const char* getInputPortTypeName(BMDSwitcherPortType port_type)
{
    switch(port_type)
    {
        case bmdSwitcherPortTypeExternal: return "External";
        case bmdSwitcherPortTypeBlack: return "Black";
        case bmdSwitcherPortTypeColorBars: return "Color Bars";
        case bmdSwitcherPortTypeColorGenerator: return "Color Generator";
        case bmdSwitcherPortTypeMediaPlayerFill: return "Media Player Fill";
        case bmdSwitcherPortTypeMediaPlayerCut: return "Media Player Cut";
        case bmdSwitcherPortTypeSuperSource: return "Type Super Source";
        case bmdSwitcherPortTypeMixEffectBlockOutput: return "Mix Effect Block Output";
        case bmdSwitcherPortTypeAuxOutput: return "Aux Output";
        case bmdSwitcherPortTypeKeyCutOutput: return "Key Cut Output";
        default: break;
    }
    
    return "unknown";
}

EXPORT
void performCut(long long source_input_id,long long destination_output_id)
{
    mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, destination_output_id);
    mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, source_input_id);
    mixEffectBlock->PerformCut();
}

EXPORT
void performCutByIndex(int src_index,int dest_index)
{
    mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, masterInputIdArray[src_index]);
    mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, masterInputIdArray[dest_index]);
    mixEffectBlock->PerformCut();
}

EXPORT
void performAuxOutputSourceChange(long long source_input_id,long long aux_output_id)
{
    IBMDSwitcherInputAux* inputAux = NULL;
    IBMDSwitcherInput* input = NULL;
    IBMDSwitcherInputIterator* iterator = NULL;
    BMDSwitcherInputId id=0;
    BMDSwitcherInputId aux_id = aux_output_id;
    
    HRESULT result;
    
    result = switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&iterator);
    
    if(result != S_OK)
        throw(100);
    
    while(S_OK == iterator->Next(&input))
    {
        result = input->GetInputId(&id);
        if(result != S_OK)
            throw(200);
        
        if(id == aux_id)
        {
            result = input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&inputAux);
            if(result != S_OK)
                throw(300);
            
            result = inputAux->SetInputSource(source_input_id);
            if(result != S_OK)
                throw(400);
            
            goto done;
        }
        
        input->Release();
    }
    
done:
    {
        if(inputAux)
            inputAux->Release();
        if(input)
            input->Release();
        if(iterator)
            iterator->Release();
    }
}

EXPORT
void performAuxOutputSourceChangeByIndex(int src_index,int dest_index)
{
    IBMDSwitcherInputAux* inputAux = NULL;
    IBMDSwitcherInput* input = NULL;
    IBMDSwitcherInputIterator* iterator = NULL;
    BMDSwitcherInputId id;
    long long dest = masterInputIdArray[dest_index];
    long long src = masterInputIdArray[src_index];
    
    switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&iterator);
    
    while(iterator->Next(&input))
    {
        input->GetInputId(&id);
        if(id == dest)
        {
            input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&inputAux);
            inputAux->SetInputSource(src);
            goto done;
        }
    }
    
done:
    {
        inputAux->Release();
        input->Release();
        iterator->Release();
    }
}