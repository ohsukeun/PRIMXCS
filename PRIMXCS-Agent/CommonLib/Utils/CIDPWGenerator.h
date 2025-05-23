#pragma once

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

#define ID_CHARTYPECOUNT    2
#define ID_UPPERCASE        0
#define ID_NUMBER           1

#define PW_CHARTYPECOUNT    4
#define PW_LOWERCASE        0
#define PW_UPPERCASE        1
#define PW_NUMBER           2
#define PW_SYMBOL           3

class CIDPWGenerator
{
public:
    int GetRandomKey(int nRange)
    {
        random_device rd;  //create random device
        mt19937 gen(rd()); //reset random engine
        uniform_int_distribution<int> dis(0, nRange-1);

        return dis(gen);
    }

    void Generate_ID(string& ID, int nUpperLength=3, int nNumberLength=4, BOOL bNoRepeat = TRUE)
    {
        vector<int> nCharCount = { 26,10 };
        vector<string> szCharacters = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                        "0123456789" };

        int nKey;
        string sBefore = "";
        string sSearch = "";
        int nCount;

        ID = "";  //Reset ID

        nCount = nUpperLength;
        while (nCount)
        {
            nKey = GetRandomKey(nCharCount[ID_UPPERCASE]);

            //delete Repeat character
            if (bNoRepeat) {
                sSearch = szCharacters[ID_UPPERCASE][nKey];
                if (sBefore.compare(sSearch) != 0)
                {
                    ID = ID + szCharacters[ID_UPPERCASE][nKey];

                    nCount = nCount - 1;
                    sBefore = sSearch;
                }
            }
            else
            {
                ID = ID + szCharacters[ID_UPPERCASE][nKey];

                nCount = nCount - 1;
            }
        }

        nCount = nNumberLength;
        while (nCount)
        {
            nKey = GetRandomKey(nCharCount[ID_NUMBER]);

            //delete Repeat character
            if (bNoRepeat) {
                sSearch = szCharacters[ID_NUMBER][nKey];
                if (sBefore.compare(sSearch) != 0)
                {
                    ID = ID + szCharacters[ID_NUMBER][nKey];

                    nCount = nCount - 1;
                    sBefore = sSearch;
                }
            }
            else
            {
                ID = ID + szCharacters[ID_NUMBER][nKey];

                nCount = nCount - 1;
            }
        }

    }

    void Generate_Password(string& password, int nLength, int nLimitLowercase = 0, int nLimitUppercase = 0, int nLimitNumber = 0, int nLimitSymbol = 0, BOOL bNoRepeat=TRUE)
    {
        vector<int> nCharCount = { 26,26,10,6 };
        vector<string> szCharacters = { "abcdefghijklmnopqrstuvwxyz",
                                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                        "0123456789",
                                        "!@#$%&" };

        int nKeyTpe;
        int nKey;
        string sBefore = "";
        string sSearch = "";
        int nCount = nLength;
        int nLimit[PW_CHARTYPECOUNT] = { nLimitLowercase, nLimitUppercase, nLimitNumber, nLimitSymbol };

        password = "";  //Reset Password
        
        while(nCount)
        {
            nKeyTpe = GetRandomKey(PW_CHARTYPECOUNT);
            nKey = GetRandomKey(nCharCount[nKeyTpe]);

            //delete Repeat character
            if (bNoRepeat) {
                sSearch = szCharacters[nKeyTpe][nKey];
                if (sBefore.compare(sSearch) != 0)
                {
                    password = password + szCharacters[nKeyTpe][nKey];
                    if (nLimit[nKeyTpe] > 0)
                        nLimit[nKeyTpe] = nLimit[nKeyTpe] - 1;

                    nCount = nCount - 1;
                    sBefore = sSearch;
                }
            }
            else
            {
                password = password + szCharacters[nKeyTpe][nKey];
                if (nLimit[nKeyTpe] > 0)
                    nLimit[nKeyTpe] = nLimit[nKeyTpe] - 1;

                nCount = nCount - 1;
            }
        }

        if (nLimit[PW_LOWERCASE] > 0)
        {
            while (nLimit[PW_LOWERCASE] > 0)
            {
                nKey = GetRandomKey(nCharCount[PW_LOWERCASE]);

                //delete Repeat character
                if (bNoRepeat) {
                    sSearch = szCharacters[PW_LOWERCASE][nKey];
                    if (sBefore.compare(sSearch) != 0)
                    {
                        password = password + szCharacters[PW_LOWERCASE][nKey];
                        nLimit[PW_LOWERCASE] = nLimit[PW_LOWERCASE] - 1;
                        sBefore = sSearch;
                    }
                }
                else {
                    password = password + szCharacters[PW_LOWERCASE][nKey];
                    nLimit[PW_LOWERCASE] = nLimit[PW_LOWERCASE] - 1;
                }
            }
        }

        if (nLimit[PW_UPPERCASE] > 0)
        {
            while (nLimit[PW_UPPERCASE] > 0)
            {
                nKey = GetRandomKey(nCharCount[PW_UPPERCASE]);

                //delete Repeat character
                if (bNoRepeat) {
                    sSearch = szCharacters[PW_UPPERCASE][nKey];
                    if (sBefore.compare(sSearch) != 0)
                    {
                        password = password + szCharacters[PW_UPPERCASE][nKey];
                        nLimit[PW_UPPERCASE] = nLimit[PW_UPPERCASE] - 1;
                        sBefore = sSearch;
                    }
                }
                else {
                    password = password + szCharacters[PW_UPPERCASE][nKey];
                    nLimit[PW_UPPERCASE] = nLimit[PW_UPPERCASE] - 1;
                }
            }
        }

        if (nLimit[PW_NUMBER] > 0)
        {
            while (nLimit[PW_NUMBER] > 0)
            {
                nKey = GetRandomKey(nCharCount[PW_NUMBER]);

                //delete Repeat character
                if (bNoRepeat) {
                    sSearch = szCharacters[PW_NUMBER][nKey];
                    if (sBefore.compare(sSearch) != 0)
                    {
                        password = password + szCharacters[PW_NUMBER][nKey];
                        nLimit[PW_NUMBER] = nLimit[PW_NUMBER] - 1;
                        sBefore = sSearch;
                    }
                }
                else {
                    password = password + szCharacters[PW_NUMBER][nKey];
                    nLimit[PW_NUMBER] = nLimit[PW_NUMBER] - 1;
                }
            }
        }

        if (nLimit[PW_SYMBOL] > 0)
        {
            while (nLimit[PW_SYMBOL] > 0)
            {
                nKey = GetRandomKey(nCharCount[PW_SYMBOL]);

                //delete Repeat character
                if (bNoRepeat) {
                    sSearch = szCharacters[PW_SYMBOL][nKey];
                    if (sBefore.compare(sSearch) != 0)
                    {
                        password = password + szCharacters[PW_SYMBOL][nKey];
                        nLimit[PW_SYMBOL] = nLimit[PW_SYMBOL] - 1;
                        sBefore = sSearch;
                    }
                }
                else {
                    password = password + szCharacters[PW_SYMBOL][nKey];
                    nLimit[PW_SYMBOL] = nLimit[PW_SYMBOL] - 1;
                }
            }
        }
    }

};
