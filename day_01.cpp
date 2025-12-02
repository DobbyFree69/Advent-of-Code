#include <iostream>
#include <cstdlib>

#define RANGE_LENGTH 100
#define INDEX_START 50

//4177
//index = (index + rangeLength + increment/decrement) % rangeLength
//range 0-99

using namespace std;


int main()
{
    int index = INDEX_START;
    int temp = 0;
    int countIndxZero = 0;
    int countPassingZero = 0;

    FILE* fp = fopen("input2.txt", "r");


    if(fp == NULL)
        cout << "Problem med input fil" << endl;

    char currentCh;
    int currentInt;

    while(fscanf(fp, "%c%d\n", &currentCh, &currentInt) == 2)
    {
        //int a = 0;
        //index = 50;


        cout << "\n " << currentCh << " " << currentInt << " index " << index << endl;
        if(currentCh == 'L')
        {
           //a -= currentInt;

           countPassingZero += (currentInt/100);    //räkna hela rundor först
           currentInt -= ((currentInt/100)*100);    //subtrahera från offset-Input


           if((index - currentInt) < 0 && index != 0)
               countPassingZero++;

            //countPassingZero += (((index - currentInt) < 0) && (index != 0))? 1 : 0;

            temp = (index + RANGE_LENGTH - currentInt) % RANGE_LENGTH;
            index = (temp >= 0) ? temp : temp + RANGE_LENGTH;

           //cout << "index - currentInt " << (index - currentInt) << " passing zero " << countPassingZero  << endl;

        }
        else if(currentCh == 'R')
        {
            countPassingZero += (currentInt/100);       //hantera hela rundor först
            currentInt -= ((currentInt/100)*100);

            if((index + currentInt) > 100 && index != 0)
            {
                countPassingZero += ((index + currentInt)/100);

            }
            //countPassingZero += (((index + currentInt) > 100 && index != 0)) ? (index + currentInt)/100 : 0;

            temp = (index + RANGE_LENGTH + currentInt) % RANGE_LENGTH;
            index = (temp >= 0) ? temp : temp + RANGE_LENGTH;

           // cout << "index + currentInt " << (index + currentInt) << " passing zero " << countPassingZero << endl;
        }


        if(index == 0)
        {
            countIndxZero++;
          //  cout << " countIndxZero " << countIndxZero << endl;
        }


       // cout << "A " << currentCh << " B " << currentInt << " temp " << temp << " index " << index << "\n\n" << endl;
    }

    fclose(fp);

    cout << "Count index is zero (PART 1): " << countIndxZero << " count passing zero " << countPassingZero << "\n\t\tsum (PART 2): " << (countIndxZero + countPassingZero) << endl;

//5402
//5560
//5846
//5997
//6230
//6248
//6389
//6802


//rätt svar 6106

    cout << "Hello world!" << endl;
    return 0;
}
