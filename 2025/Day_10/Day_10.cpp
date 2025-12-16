#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

typedef vector<vector<int>> vvi;
typedef vector<int> vi;

vector<vvi> machineButtons;
vector<vi> machineJoltageLevels;
vector<string> machineIndicatorLights;
const int MAX_PRESSES = 1000000;
int minOverallPresses = MAX_PRESSES;
vector<int> bestPressCounts; // Stores the final minimal button counts [x0, x1, ...]

struct MemoKey
{
    vector<int> target;      // The remaining joltage levels (b')
    long long multiplier;    // The current bit level (2^l)

    bool operator<(const MemoKey& other) const
    {
        if (multiplier != other.multiplier)
            return multiplier < other.multiplier;

        return target < other.target;
    }
};

using MemoValue = vector<int>;

vector<int> parseIntegerList(string list_content)
{
    vector<int> result;

    stringstream ss(list_content);
    string segment;

    // Split the stream by the comma delimiter
    while (getline(ss, segment, ','))
    {
        if (!segment.empty())
            result.push_back(stoi(segment));
    }
    return result;
}

long long powOfTwo(int a)
{
    return 1 << a;
}

vector<int> indicatorStringToVI(string& strIndicator)
{
    vector<int> res(strIndicator.length()+1, 0);

    for(int i = 0; i < strIndicator.length()+1; i++)
    {
        if(strIndicator[i] == '#')
            res[i] = 1;
    }
    return res;
}

vector<int> getJoltageTarget(const vi& joltageLevels, int maxIndex)
{
    int N = max((int)joltageLevels.size(), maxIndex + 1);

    vector<int> targetState(N, 0);
    for (size_t i = 0; i < joltageLevels.size(); ++i)
        targetState[i] = joltageLevels[i];

    return targetState;
}

//PART ONE
vector<int> findSmallestButtonCombination(vvi& buttons, string& indicatorLights, int maxIndex)
{
    int maxPossibleIndices = maxIndex + 1;  // Total number of possible indices (0 to maxIndex)
    int buttonsSize = buttons.size();       // Total number of buttons

    vector<int> ilDiagram = indicatorStringToVI(indicatorLights);

    // Initial best values
    int minButtonsUsed = buttonsSize + 1;
    vector<int> bestCombination;

    // Iterate through all 2^M combinations
    for (int iCombiNum = 0; iCombiNum < powOfTwo(buttonsSize); ++iCombiNum)    //2^M: (1 << M) -> all combinations
    {
        // Calculate Number of buttons pressed (in this combination iComNum)
        int buttonsUsed = 0;

        for(int j = 0; j < buttonsSize; ++j)
        {
            if ((iCombiNum / powOfTwo(j)) % 2) //(i >> j) & 1
                buttonsUsed++;
        }

        if (buttonsUsed >= minButtonsUsed)
            continue;

        vector<int> currentState(maxPossibleIndices, 0);// This vector stores the current state of light switches (count mod 2)
        //Press buttons in the iCombiNum -combination
        for (int j = 0; j < buttonsSize; ++j)
        {
            //iCombiNum:  0 0 1 1 0  -> buttons: 1/0 -> button pressed/not pressed
            // If the j-th bit in iCombiNum is set, 'press' button j.
            if ((iCombiNum / powOfTwo(j)) % 2) //(i >> j) & 1
            {
                // Apply the effect of button j (XOR its indices into the current state)
                for (int index : buttons[j])
                {
                    if (index < maxPossibleIndices)
                    {
                          //ToggleLight/Press Button                  // XOR operation: 1 if odd, 0 if even
                          currentState[index] = !currentState[index]; //currentState[index] ^= 1; -> bitwise flip
                    }
                }
            }
        }

        // Check if currentState matches the indicator light diagram
        bool match = true;
        for (int k = 0; k < maxPossibleIndices; ++k)
        {
            if (currentState[k] != ilDiagram[k])
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            minButtonsUsed = buttonsUsed;

            bestCombination.clear();    // Store the indices of the buttons that were pressed
            for(int j = 0; j < buttonsSize; ++j)
            {
                if ((iCombiNum / powOfTwo(j)) % 2)   //(i >> j) & 1)
                {
                    bestCombination.push_back(j);
                }
            }
        }
    }

    return bestCombination;
}

vector<vi> findModuloTwoSolutions(const vvi& buttons, const vi& targetMod2, int buttonsSize)
{
    int targetMod2Size = targetMod2.size();
    vector<vi> solutions;

    for (long long iCombiNum = 0; iCombiNum < powOfTwo(buttonsSize); ++iCombiNum)
    {
        vi currentCombination(buttonsSize, 0);
        vi currentState(targetMod2Size, 0);

        // Build the combination and apply effects
        for (int j = 0; j < buttonsSize; ++j)
        {
            if ((iCombiNum / powOfTwo(j)) % 2)
            {
                currentCombination[j] = 1;
                // Apply button j's effect (XOR/Modulo 2 sum)
                for (int index : buttons[j])
                {
                    if (index < targetMod2Size)
                        currentState[index] = (currentState[index] + 1) % 2;
                }
            }
        }

        //Check if the result matches the target b (mod 2)
        bool match = true;
        for (int k = 0; k < targetMod2Size; ++k)
        {
            if (currentState[k] != targetMod2[k])
            {
                match = false;
                break;
            }
        }

        if (match)
            solutions.push_back(currentCombination);
    }
    return solutions;
}

MemoValue solveRecursive(vvi& buttons,vi& currentTarget, map<MemoKey, MemoValue>& memo, long long multiplier)
{
    int currentTargetSize = currentTarget.size();
    int buttonsSize = buttons.size();

    //Check Memoization Table (Key: Target + Multiplier)
    MemoKey currentKey = {currentTarget, multiplier};
    if (memo.count(currentKey))
        return memo[currentKey];

    //Base Case: Solution Found (currentTarget is all zeros)
    bool isZeroTarget = all_of(currentTarget.begin(), currentTarget.end(), [](int i){ return i == 0; });

    if (isZeroTarget)
        return vector<int>(buttonsSize, 0);

    vi targetMod2(currentTargetSize);
    for (int i = 0; i < currentTargetSize; ++i)
    {
        if (currentTarget[i] < 0)
            return MemoValue{};
        targetMod2[i] = currentTarget[i] % 2;
    }

    vector<vi> mod2Solutions = findModuloTwoSolutions(buttons, targetMod2, buttonsSize);

    if(mod2Solutions.empty())
    {
        memo[currentKey] = {}; // Cache the failure
        return {};
    }

    //Recurse for the next bit (l+1)
    long long minTotalAdditionalPresses = numeric_limits<long long>::max();
    MemoValue bestAdditionalPressCounts; // Stores the best result found in this recursive call

    for (vi& mod2Button : mod2Solutions)
    {
        vi currentStateMod2Solutions(currentTargetSize, 0);
        for (int j = 0; j < buttonsSize; ++j)
        {
            if (mod2Button[j] == 1)
            {
                for (int index : buttons[j])
                {
                    if (index < currentTargetSize)
                        currentStateMod2Solutions[index] += 1;
                }
            }
        }

        vi nextTarget(currentTargetSize);
        bool allValid = true;
        for (int i = 0; i < currentTargetSize; ++i)
        {
            int difference = currentTarget[i] - currentStateMod2Solutions[i];
            if (difference % 2 != 0 || difference < 0)
            {
                allValid = false;
                break;
            }
            nextTarget[i] = difference / 2;
        }

        if (!allValid)
            continue; // Skip invalid path (shouldn't happen if findModuloTwoSolutions is correct)

        // Recursion - Cache into Memo
        MemoValue resultFromNextState = solveRecursive(buttons, nextTarget, memo, multiplier * 2);

        if (!resultFromNextState.empty())
        {

            long long currentPathTotal = 0;
            MemoValue combinedPresses(buttonsSize);

            for(int j = 0; j < buttonsSize; ++j)
            {
                combinedPresses[j] = mod2Button[j] * multiplier + resultFromNextState[j];
                currentPathTotal += combinedPresses[j];
            }

            if (currentPathTotal < minTotalAdditionalPresses)
            {
                minTotalAdditionalPresses = currentPathTotal;
                bestAdditionalPressCounts = combinedPresses;
            }
        }
    }

    memo[currentKey] = bestAdditionalPressCounts;//Memoize and Return
    return bestAdditionalPressCounts;
}

vector<int> findSmallestButtonCombination_Joltage(vvi& buttons, vi& joltageLevels, int maxIndex)
{
    // Reset global state for each machine
    minOverallPresses = MAX_PRESSES;
    bestPressCounts.clear();
    int buttonsSize = buttons.size();

    map<MemoKey, MemoValue> memo;

    vi initialTarget = getJoltageTarget(joltageLevels, maxIndex);

    // Recursion - with memoization
    MemoValue finalPressCounts = solveRecursive(buttons, initialTarget, memo, 1);

    // Recursion - without memoization ca. 600 secs
//    solveRecursive(buttons, initialTarget, vector<int>(buttonsSize, 0), 1);

    if (finalPressCounts.empty()) {
        return {}; // No solution found (MemoValue{} was returned)
    }
//
//    if (minOverallPresses == MAX_PRESSES) {       //without memoization
//        return {}; // No solution found
//    }

    return finalPressCounts; //Return result from Memoized approach
   // return bestPressCounts;
}

long long solvePartTwo(vvi& buttons, vi& joltageLevels, int maxIndex)
{
    vector<int> solutionCounts = findSmallestButtonCombination_Joltage(buttons, joltageLevels, maxIndex);

    long long totalPresses = 0;
    for (int countCurrentPress : solutionCounts)
        totalPresses += countCurrentPress;

    return totalPresses;
}

long long solvePartOne(vvi& buttons, string& indicatorLights, int maxIndex)
{
    vector<int> solution = findSmallestButtonCombination(buttons, indicatorLights, maxIndex);
    return solution.size();
}

int main()
{
    long long sumMinButtonPresses = 0;
    long long sumMinButtonPressesP1 = 0; // New accumulator for Part 1
    long long sumMinButtonPressesP2 = 0; // New accumulator for Part 2 (Joltage)

    fstream f("input.txt");

    if(!f)
        cout << "Problem med fil " << endl;

    string line;
    int lineCount = 0;
    while(getline(f, line))
    {
        lineCount++;
        cout << "--- Processing Line " << lineCount <<  endl;
        vvi vviButtons;
        vi viJoltageLevels;
        string indicatorLights;

        size_t start, endp;
        size_t currentPos = 0; // Tracks the current search position

        //Between [ and ] -> Indicator Light Diagram
        start = line.find('[', currentPos);
        endp = line.find(']', start);

        indicatorLights = line.substr(start + 1, endp - start - 1);
        currentPos = endp + 1; // Start searching after ']'

        //Betwwen { and } -> Joltage Levels
        size_t braceStart = line.find('{', currentPos);
        size_t braceEnd = line.find('}', braceStart);

        string sJoltageLevels = line.substr(braceStart + 1, braceEnd - braceStart - 1);
        viJoltageLevels = parseIntegerList(sJoltageLevels);

        //Between ] and { -> Buttons
        string sButtons = line.substr(currentPos, braceStart - currentPos);

        size_t buttonsStart = 0;
        while((buttonsStart = sButtons.find('(', buttonsStart)) != string::npos)
        {
            vi viButton;
            size_t buttonsEnd = sButtons.find(')', buttonsStart);

            //Button
            string sButton = sButtons.substr(buttonsStart + 1, buttonsEnd - buttonsStart - 1);

            //string to Vector<int>
            viButton = parseIntegerList(sButton);
            vviButtons.push_back(viButton);

            buttonsStart = buttonsEnd + 1;
        }

        machineButtons.push_back(vviButtons);
        machineJoltageLevels.push_back(viJoltageLevels);
        machineIndicatorLights.push_back(indicatorLights);


        int maxIndex = maxIndex = (int)viJoltageLevels.size() - 1;

        // Part 1
        sumMinButtonPressesP1 += solvePartOne(vviButtons, indicatorLights, indicatorLights.length());

        // Part 2
        sumMinButtonPressesP2 += solvePartTwo(vviButtons, viJoltageLevels, maxIndex);


        cout << "Line " << lineCount << " solved. P1 Total: " << sumMinButtonPressesP1 << ", P2 Total: " << sumMinButtonPressesP2 << endl;
    }
    f.close();

    cout << "Part 1: " << sumMinButtonPressesP1 << endl;
    cout << "Part 2: " << sumMinButtonPressesP2 << endl;

    cout << "Hello world!" << endl;
    return 0;
}
