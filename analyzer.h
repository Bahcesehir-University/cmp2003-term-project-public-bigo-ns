#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>

struct ZoneCount {
    std::string zone;
    long long count;
};

struct SlotCount {
    std::string zone;
    int hour;
    long long count;
};

class TripAnalyzer {
public:
    void ingestStdin();
    void ingestFile(const std::string& csvPath);
    std::vector<ZoneCount> topZones(int k = 10) const;
    std::vector<SlotCount> topBusySlots(int k = 10) const;

private:
    struct RawString {
        const char* ptr;
        int len;
        bool operator==(const RawString& o) const {
            return len == o.len && std::memcmp(ptr, o.ptr, len) == 0;
        }
    };

    struct RawHash {
        size_t operator()(const RawString& s) const {
            size_t h = 1469598103934665603ull;
            for (int i = 0; i < s.len; ++i) {
                h ^= (unsigned char)s.ptr[i];
                h *= 1099511628211ull;
            }
            return h;
        }
    };

    std::vector<char> buffer;
    std::unordered_map<RawString, int, RawHash> zoneIndex;
    std::vector<RawString> zoneNames;
    std::vector<long long> zoneCounts;
    std::vector<long long> zoneHourCounts;
};

#endif
