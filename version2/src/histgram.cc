#include "histgram.hh"

Histgram::Histgram(string name, int bin, double min, double max)
:   name_(name),
    bin_(bin),
    min_(min),
    max_(max)
{
    // get bin size
    binSize_ = (max_-min_)/double(bin_);

    // initialization
    for(int i=0;i<bin_;i++)
    {
        histgram_.push_back(0.);
    }

    /*
    // debug
    cout<<"Histgram "<<name_<<": bin "<<bin_<<", min "<<min_<<", max "<<max_<<", binSize "<<binSize_<<endl;
    */
}


Histgram::~Histgram()
{}


void Histgram::Fill(double value, double weight)
{
    int binNum = getBinNumber(value);
    histgram_[binNum] += weight;
}

int Histgram::GetBinTotalNumber()
{
    return bin_;
}

double Histgram::GetBinContent(int binID)
{
    return histgram_[binID];
}

double Histgram::GetBinCoordinate(int binID)
{
    double coordinate = min_ + binSize_*double(binID+0.5); 

    return coordinate;
}

void Histgram::ShowHistgram()
{
    for(int i=0;i<bin_;i++)
    {
        double location = min_ + binSize_*double(i);
        cout<<"Bin "<<i<<":    Location "<<location<<"    Count "<<histgram_[i]<<endl;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Histgram::getBinNumber(double value)
{
    int binNumber = -1;

    /*
    // debug
    cout<<"value "<<value<<endl;
    */

    if(value<min_||value>=max_) {
        binNumber = -1;
    }

    else {
        for(int i=0;i<bin_;i++)
        {
            double current_min = min_ + double(i)*binSize_;
            double current_max = min_ + double(i+1)*binSize_;

            /*
            // debug
            cout<<"value "<<value<<", binNum "<<i<<": current_max "<<current_max<<endl;
            */

            if(value<current_max) {
                binNumber = i;
                break;
            }
        }
    }

    return binNumber;
}
