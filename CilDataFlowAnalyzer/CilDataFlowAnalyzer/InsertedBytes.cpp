#include "cor.h"
#include "InsertedBytes.h"


void InsertedBytes::Add(int location, int byteCount)
{
    this->insert(std::map<int, int>::value_type(location, byteCount));
}

int InsertedBytes::GetCountOfBytesInsertedBefore(const DWORD location, const bool isReplaceLocation)
{
    auto retVal = 0;
    for (const auto insertion : *this)
    {
        if ((isReplaceLocation && insertion.first < location) || (!isReplaceLocation && insertion.first <= location))
        {
            retVal += insertion.second;
        }
    }

    return retVal;
}

int InsertedBytes::GetCountOfBytesInsertedBetween(const DWORD startLocation, const DWORD endLocation, const bool isReplaceLocation)
{
    const auto lowerBound = startLocation <= endLocation ? startLocation : endLocation;
    auto upperBound = endLocation >= startLocation ? endLocation : startLocation;

    // This is to prevent code inserted during a 'replace' mutation ('Call' location) from being included
    // in the corner case where a jump target is the site of the replace.
    // In other words, you still want the jump to target the opcode at the site of the replace,
    // not to be incorrectly revectored to an opcode after the inserted code.
    if (endLocation >= startLocation && isReplaceLocation)
    {
        upperBound--;
    }

    auto retVal = 0;
    for (const auto insertion : *this)
    {
        if (((isReplaceLocation && insertion.first >= lowerBound) || (insertion.first > lowerBound)) && insertion.first <= upperBound)
        {
            retVal += insertion.second;
        }
    }

    return retVal;
}
