#include "dataDetector.hh"

DataDetector::DataDetector(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID, double counts, double efficiency_intrinsic, double detect_area)
:   DataBase_Voxel(name, x, y, z, IDx, IDy, IDz, ID),
    counts_(counts),
    efficiency_intrinsic_(efficiency_intrinsic),
    detect_area_(detect_area)
{
    /*
    // debug
    cout<<"Detector : "<<endl;
    cout<<"    Name : "<<name_<<endl;
    cout<<"    location(mm) : "<<x_/mm<<",  "<<y_/mm<<", "<<z_/mm<<endl;
    cout<<"    ID (x,y,z) : ("<<IDx_<<", "<<IDy_<<", "<<IDz_<<")"<<endl;
    cout<<"    counts : "<<counts_<<endl;
    */
}

DataDetector::~DataDetector()
{}

void DataDetector::addADistance(double adistance)
{
    distances_.push_back(adistance);

    // get area efficiency
    double an_efficiency_area = detect_area_ / (PI*adistance*adistance);
    if(an_efficiency_area>=1) an_efficiency_area=1;
    efficiencies_area_.push_back(an_efficiency_area);
}

double DataDetector::getDistance(int sourceID)
{
    if(sourceID>=distances_.size())
    {
        cout<<"ERROR: An Error happend in DataDetector::getDistance;"<<endl;
        return -1;
    }
    return distances_[sourceID];
}

double DataDetector::getAnEfficiencyArea(int sourceID)
{
    if(sourceID>=efficiencies_area_.size())
    {
        cout<<"ERROR: An Error happend in DataDetector::getAnEfficiencyArea;"<<endl;
        return -1;
    }
    return efficiencies_area_[sourceID];
}

double DataDetector::getEfficiencyIntrinsic()
{
    return efficiency_intrinsic_;
}

double DataDetector::getAnEfficiencyTotal(int sourceID)
{
    if(sourceID>=efficiencies_area_.size())
    {
        cout<<"ERROR: An Error happend in DataDetector::getEfficiencyTotal;"<<endl;
        return -1;
    }
    double eff_area = efficiencies_area_[sourceID];

    double efficiencyTotal = efficiency_intrinsic_*eff_area;
    
    return efficiencyTotal;
}

double DataDetector::getCounts()
{
    return counts_;
}

void DataDetector::show()
{
    cout<<"Detector : "<<endl;
    cout<<"    Name : "<<name_<<endl;
    cout<<"    location(mm) : "<<x_/mm<<",  "<<y_/mm<<", "<<z_/mm<<endl;
    cout<<"    ID (x,y,z) : ("<<IDx_<<", "<<IDy_<<", "<<IDz_<<")"<<endl;
    cout<<"    counts : "<<counts_<<endl;
    cout<<"    efficiency_intrinsic_ : "<<efficiency_intrinsic_<<endl;
    cout<<"    detect_area : "<<detect_area_/mm2<<" mm"<<endl;
}
