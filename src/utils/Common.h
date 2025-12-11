#ifndef _COMMON_H
#define _COMMON_H

#include "Logger.h"

#define MAX_SIZE_OF_TYPES(...) \
([]{ \
    size_t max_size = 0; \
    size_t sizes[] = { __VA_ARGS__ }; \
    size_t num_types = sizeof(sizes) / sizeof(sizes[0]); \
    for (size_t i = 0; i < num_types; ++i) { \
        max_size = std::max(max_size, sizes[i]); \
    } \
    return max_size; \
})()

class Common{
    public:

        Common();
        static void printArray(int arr[],int size);
        static void calculateFrequency(int arr[],int freq[],int size);
        static int mean(const int arr[],int size);
        static void convertToUpper(char *sPtr);
        static void printCharacters(const char *sPtr);
        static void displayBits(int value);
    private:

        
};

#endif