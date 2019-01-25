#include "dataSource.hh"

DataSource::DataSource(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID, double intensity)
:   DataBase_Voxel(name, x, y, z, IDx, IDy, IDz, ID),
    intensity_(intensity),
    groundTruth_intensity_(-1)
{
    vector_intensity_.push_back(intensity);
    /*
    // debug
    cout<<"Source : "<<endl;
    cout<<"    Name : "<<name_<<endl;
    cout<<"    location(mm) : "<<x_/mm<<",  "<<y_/mm<<", "<<z_/mm<<endl;
    cout<<"    IDs(x,y,z) : ("<<IDx_<<", "<<IDy_<<", "<<IDz_<<"); ID : "<<ID_<<endl;
    cout<<"    intensity : "<<intensity_<<endl;
    */
}

DataSource::~DataSource()
{}


void DataSource::setIntensty(double intensity)
{
    intensity_ = intensity;
    vector_intensity_.push_back(intensity);
}

double DataSource::getIntensty()
{
    return intensity_;
}

double DataSource::getIntensity(int iterationID)
{
    double anIntensity = vector_intensity_[iterationID];
    return anIntensity;
}

void DataSource::setGroungTruth_Intensty(double intensity)
{
    groundTruth_intensity_ = intensity;
}

double DataSource::getGroungTruth_Intensty()
{
    return groundTruth_intensity_;
}

void DataSource::show()
{
    // debug
    cout<<"Source : "<<endl;
    cout<<"    Name : "<<name_<<endl;
    cout<<"    location(mm) : "<<x_/mm<<",  "<<y_/mm<<", "<<z_/mm<<endl;
    cout<<"    IDs(x,y,z) : ("<<IDx_<<", "<<IDy_<<", "<<IDz_<<"); ID : "<<ID_<<endl;
    cout<<"    intensity : "<<intensity_<<endl;
}
