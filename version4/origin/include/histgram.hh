#ifndef HISTGRAM_HH
#define HISTGRAM_HH

#include <iostream>
#include <string>
#include <vector>

#include "units.hh"

using namespace std;

class Histgram
{
    public:
        Histgram(string name, int bin, double min, double max);
        ~Histgram();

    public:
        void    Fill(double value, double weight);
        void    ShowHistgram();
        double  GetBinContent(int binID);
        int     GetBinTotalNumber();
        double  GetBinCoordinate(int binID);

    private:
        int getBinNumber(double value);

    private:
        string  name_;
        int     bin_;
        double  min_;
        double  max_;
        double  binSize_;

        vector<double> histgram_;

};
#endif
