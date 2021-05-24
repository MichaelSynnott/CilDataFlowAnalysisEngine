#pragma once
#include <map>

class InsertedBytes : public std::map<int, int>
{
public:
    void Add(int location, int byteCount);
    int GetCountOfBytesInsertedBefore(const DWORD location, const bool isReplaceLocation);
    int GetCountOfBytesInsertedBetween(const DWORD startLocation, const DWORD endLocation, const bool isReplaceLocation);

private:
};
