#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

vector<pair<long long, long long>> ranges;
vector<pair<long long, long long>> modRanges;

void modifyRanges()                         //sammanslå ranges
{
    sort(ranges.begin(), ranges.end());

    modRanges.push_back(ranges[0]);        //börja med den första

    for (auto i = 0; i < ranges.size(); i++)
    {
        pair<long long, long long>& prevMod = modRanges.back();

        //check if ranges overlap
        if(ranges[i].first <= prevMod.second)
            prevMod.second = (ranges[i].second >= prevMod.second) ? ranges[i].second : prevMod.second;
        else
            modRanges.push_back(ranges[i]);
    }

    sort(modRanges.begin(), modRanges.end());
}


long long partTwo()
{
    long long sumFresh = 0;

    for(auto range : modRanges)
        sumFresh += (range.second - range.first) + 1;

    return sumFresh;
}

bool partOne(long long llId)
{
    for(auto range : ranges)        //For each range: check if current ID llId is in range
    {
        if((range.first <= llId) && (llId <= range.second))
            return true;
    }
    return false;
}

int main()
{
    //Read in ranges from file
    fstream file("input_ranges.txt");
    string input;

    if(!file)
        cout << "Problem med fil " << endl;

    while(getline(file, input))
    {
        pair<long long, long long> currentRange;
        string delLimits = "-";

        auto pos = input.find(delLimits);
        currentRange.first = stoll(input.substr(0, pos));
        currentRange.second = stoll(input.substr(pos + delLimits.length()));

        ranges.push_back(currentRange);
    }
    file.close();


    //PART 1
    fstream fIds("input_ids.txt");                //Read IDs

    if(!fIds)
        cout << "Problem med fil " << endl;

    string sId;
    int countFreshIds = 0;

    while(getline(fIds, sId))
        countFreshIds += (partOne(stoll(sId))) ? 1 : 0;

    fIds.close();

    //PART 2
    modifyRanges();

    cout << "Part 1: " << countFreshIds << endl;
    cout << "Part 2: " << partTwo() << endl;

    return 0;
}
