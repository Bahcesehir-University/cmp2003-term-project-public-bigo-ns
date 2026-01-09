#include "analyzer.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

void TripAnalyzer::ingestStdin() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    buffer.assign(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
    if (buffer.empty()) return;

    const char* cur = buffer.data();
    const char* end = cur + buffer.size();

    if (cur < end && (*cur < '0' || *cur > '9')) {
        const char* nl = (const char*)std::memchr(cur, '\n', end - cur);
        if (nl) cur = nl + 1;
        else cur = end;
    }

    while (cur < end) {
        const char* lineEnd = (const char*)std::memchr(cur, '\n', end - cur);
        if (!lineEnd) lineEnd = end;

        if (cur >= lineEnd) {
            cur = lineEnd + 1;
            continue;
        }

        const char* c1 = (const char*)std::memchr(cur, ',', lineEnd - cur);
        if (!c1) { cur = lineEnd + 1; continue; }

        const char* c2 = (const char*)std::memchr(c1 + 1, ',', lineEnd - (c1 + 1));
        if (!c2) { cur = lineEnd + 1; continue; }

        const char* c3 = (const char*)std::memchr(c2 + 1, ',', lineEnd - (c2 + 1));
        if (!c3) { cur = lineEnd + 1; continue; }

        const char* c4 = (const char*)std::memchr(c3 + 1, ',', lineEnd - (c3 + 1));
        if (!c4) { cur = lineEnd + 1; continue; }

        const char* c5 = (const char*)std::memchr(c4 + 1, ',', lineEnd - (c4 + 1));
        if (!c5) { cur = lineEnd + 1; continue; }

        const char* zStart = c1 + 1;
        const char* zEnd = c2;
        while (zStart < zEnd && (*zStart == ' ' || *zStart == '\t')) zStart++;
        while (zEnd > zStart && (*(zEnd - 1) == ' ' || *(zEnd - 1) == '\t')) zEnd--;
        
        if (zStart >= zEnd) { cur = lineEnd + 1; continue; }

        const char* tStart = c3 + 1;
        const char* tEnd = c4;
        
        const char* space = nullptr;
        for (const char* p = tStart; p < tEnd; ++p) {
            if (*p == ' ') {
                space = p;
                break;
            }
        }
        
        if (!space) { cur = lineEnd + 1; continue; }

        const char* timePtr = space + 1;
        if (timePtr + 5 > tEnd) { cur = lineEnd + 1; continue; }

        if (timePtr[0] < '0' || timePtr[0] > '9' || 
            timePtr[1] < '0' || timePtr[1] > '9') {
            cur = lineEnd + 1; continue; 
        }
        int h = (timePtr[0] - '0') * 10 + (timePtr[1] - '0');

        if (timePtr[2] != ':') { cur = lineEnd + 1; continue; }

        if (timePtr[3] < '0' || timePtr[3] > '9' || 
            timePtr[4] < '0' || timePtr[4] > '9') {
            cur = lineEnd + 1; continue; 
        }
        int m = (timePtr[3] - '0') * 10 + (timePtr[4] - '0');

        if (h > 23 || m > 59) {
            cur = lineEnd + 1;
            continue;
        }

        RawString zoneKey{ zStart, (int)(zEnd - zStart) };
        
        int zid;
        auto it = zoneIndex.find(zoneKey);
        if (it == zoneIndex.end()) {
            zid = (int)zoneNames.size();
            zoneIndex.emplace(zoneKey, zid);
            zoneNames.push_back(zoneKey);
            zoneCounts.push_back(0);
            zoneHourCounts.resize(zoneHourCounts.size() + 24, 0);
        } else {
            zid = it->second;
        }

        zoneCounts[zid]++;
        zoneHourCounts[zid * 24 + h]++;

        cur = lineEnd + 1;
    }
}

void TripAnalyzer::ingestFile(const std::string& csvPath) {
    buffer.clear();
    zoneIndex.clear();
    zoneNames.clear();
    zoneCounts.clear();
    zoneHourCounts.clear();

    std::ifstream file(csvPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;

    std::streamsize size = file.tellg();
    if (size <= 0) return;

    file.seekg(0, std::ios::beg);
    buffer.resize(size);
    if (!file.read(buffer.data(), size)) return;
    file.close();

    const char* cur = buffer.data();
    const char* end = cur + size;

    if (cur < end && (*cur < '0' || *cur > '9')) {
        const char* nl = (const char*)std::memchr(cur, '\n', end - cur);
        if (nl) cur = nl + 1;
        else cur = end;
    }

    while (cur < end) {
        const char* lineEnd = (const char*)std::memchr(cur, '\n', end - cur);
        if (!lineEnd) lineEnd = end;

        if (cur >= lineEnd) {
            cur = lineEnd + 1;
            continue;
        }

        const char* c1 = (const char*)std::memchr(cur, ',', lineEnd - cur);
        if (!c1) { cur = lineEnd + 1; continue; }

        const char* c2 = (const char*)std::memchr(c1 + 1, ',', lineEnd - (c1 + 1));
        if (!c2) { cur = lineEnd + 1; continue; }

        const char* c3 = (const char*)std::memchr(c2 + 1, ',', lineEnd - (c2 + 1));
        if (!c3) { cur = lineEnd + 1; continue; }

        const char* c4 = (const char*)std::memchr(c3 + 1, ',', lineEnd - (c3 + 1));
        if (!c4) { cur = lineEnd + 1; continue; }

        const char* c5 = (const char*)std::memchr(c4 + 1, ',', lineEnd - (c4 + 1));
        if (!c5) { cur = lineEnd + 1; continue; }

        const char* zStart = c1 + 1;
        const char* zEnd = c2;
        while (zStart < zEnd && (*zStart == ' ' || *zStart == '\t')) zStart++;
        while (zEnd > zStart && (*(zEnd - 1) == ' ' || *(zEnd - 1) == '\t')) zEnd--;
        
        if (zStart >= zEnd) { cur = lineEnd + 1; continue; }

        const char* tStart = c3 + 1;
        const char* tEnd = c4;
        
        const char* space = nullptr;
        for (const char* p = tStart; p < tEnd; ++p) {
            if (*p == ' ') {
                space = p;
                break;
            }
        }
        
        if (!space) { cur = lineEnd + 1; continue; }

        const char* timePtr = space + 1;
        if (timePtr + 5 > tEnd) { cur = lineEnd + 1; continue; }

        if (timePtr[0] < '0' || timePtr[0] > '9' || 
            timePtr[1] < '0' || timePtr[1] > '9') {
            cur = lineEnd + 1; continue; 
        }
        int h = (timePtr[0] - '0') * 10 + (timePtr[1] - '0');

        if (timePtr[2] != ':') { cur = lineEnd + 1; continue; }

        if (timePtr[3] < '0' || timePtr[3] > '9' || 
            timePtr[4] < '0' || timePtr[4] > '9') {
            cur = lineEnd + 1; continue; 
        }
        int m = (timePtr[3] - '0') * 10 + (timePtr[4] - '0');

        if (h > 23 || m > 59) {
            cur = lineEnd + 1;
            continue;
        }

        RawString zoneKey{ zStart, (int)(zEnd - zStart) };
        
        int zid;
        auto it = zoneIndex.find(zoneKey);
        if (it == zoneIndex.end()) {
            zid = (int)zoneNames.size();
            zoneIndex.emplace(zoneKey, zid);
            zoneNames.push_back(zoneKey);
            zoneCounts.push_back(0);
            zoneHourCounts.resize(zoneHourCounts.size() + 24, 0);
        } else {
            zid = it->second;
        }

        zoneCounts[zid]++;
        zoneHourCounts[zid * 24 + h]++;

        cur = lineEnd + 1;
    }
}

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    struct E { int id; long long c; };
    std::vector<E> tmp;
    tmp.reserve(zoneNames.size());

    for (int i = 0; i < (int)zoneNames.size(); ++i) {
        if (zoneCounts[i] > 0)
            tmp.push_back({i, zoneCounts[i]});
    }

    std::sort(tmp.begin(), tmp.end(), [&](const E& a, const E& b) {
        if (a.c != b.c) return a.c > b.c;
        const RawString& A = zoneNames[a.id];
        const RawString& B = zoneNames[b.id];
        int minLen = std::min(A.len, B.len);
        int cmp = std::memcmp(A.ptr, B.ptr, minLen);
        if (cmp != 0) return cmp < 0;
        return A.len < B.len;
    });

    int limit = std::min(k, (int)tmp.size());
    std::vector<ZoneCount> out;
    out.reserve(limit);
    for (int i = 0; i < limit; ++i) {
        const RawString& s = zoneNames[tmp[i].id];
        out.push_back({std::string(s.ptr, s.len), tmp[i].c});
    }
    return out;
}

std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    struct E { int id; int h; long long c; };
    std::vector<E> tmp;

    for (int z = 0; z < (int)zoneNames.size(); ++z) {
        const long long* base = &zoneHourCounts[z * 24];
        for (int h = 0; h < 24; ++h) {
            if (base[h] > 0)
                tmp.push_back({z, h, base[h]});
        }
    }

    std::sort(tmp.begin(), tmp.end(), [&](const E& a, const E& b) {
        if (a.c != b.c) return a.c > b.c;
        const RawString& A = zoneNames[a.id];
        const RawString& B = zoneNames[b.id];
        int minLen = std::min(A.len, B.len);
        int cmp = std::memcmp(A.ptr, B.ptr, minLen);
        if (cmp != 0) return cmp < 0;
        if (A.len != B.len) return A.len < B.len;
        return a.h < b.h;
    });

    int limit = std::min(k, (int)tmp.size());
    std::vector<SlotCount> out;
    out.reserve(limit);
    for (int i = 0; i < limit; ++i) {
        const RawString& s = zoneNames[tmp[i].id];
        out.push_back({std::string(s.ptr, s.len), tmp[i].h, tmp[i].c});
    }
    return out;
}
