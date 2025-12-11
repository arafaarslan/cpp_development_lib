#include "Common.h"


Common::Common(){
}

void Common::printArray(int arr[],int size){
    std::string builder;
    for (int i = 0; i < size; i++){
        builder.append(std::to_string(arr[i]));
        builder.append("-");
    }
    Logger::Debug("Common::printArray : " + builder);
}

void Common::calculateFrequency(int arr[],int freq[],int size){
    for (int i = 0; i < size; i++){
        ++freq[arr[i]];   
    }
}

int Common::mean(const int arr[],int size){
    int sum = 0;
    for (int i = 0; i < size; i++){
          sum += arr[i];
    }
    return sum/size;
}


void Common::convertToUpper(char *sPtr){
    while(*sPtr != '\0'){
        if(islower(*sPtr)){
            *sPtr = toupper(*sPtr);
        }
        sPtr++;
    }
    printf("\n");
}

void Common::printCharacters(const char *sPtr){
    while(*sPtr != '\0'){
        printf("%c",*sPtr);
        sPtr++;
    }
    printf("\n");
}

void Common::displayBits(int value){
    
    unsigned int displayMask = 1 << 31;

    printf("value : %u\n",value);
    for(int i=1;i<=32;i++){
        putchar(value & displayMask? '1' : '0');
        value<<=1;
        if(i %8 == 0){
            putchar(' ');
        }

    }
    putchar('\n');
}