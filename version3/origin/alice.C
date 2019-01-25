#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "dataDetector.hh"
#include "dataSource.hh"
#include "processEM.hh"

using namespace std;

//
// parameters
//
double radius_ = 40.*mm;
double intrinsic_efficiency_ = 1e-2;

//
// functions
//
int import_sourcesGroundTruth(ProcessEM &processEM, string filename);

int main(int argc,char** argv)
{
    cout<<"Hello"<<endl;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // part 1 : get detector counting numbers
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // 
    // step 0 : set parameters
    //
    double radius = radius_;
    double detect_area = PI*radius*radius;

    // 
    int     binx = 4;
    double  minx = 0;
    double  maxx = 400;

    int     biny = 4;
    double  miny = 0;
    double  maxy = 800;

    int     binz = 1;
    double  minz = 0;
    double  maxz = 1e3;

    double  binSize_x = (maxx-minx)/double(binx);
    double  binSize_y = (maxy-miny)/double(biny);
    double  binSize_z = (maxz-minz)/double(binz);

    ProcessEM process_part1("process_part1", binx, minx, maxx, biny, miny, maxy, binz, minz, maxz);

    //
    vector<DataDetector> detectors;

    //
    // step 1 : get detectors' location
    //
    vector<double> locx, locy, locz;

    locx.push_back(minx);
    locy.push_back(miny);
    locz.push_back(minz+0.5*binSize_z);

    locx.push_back(maxx);
    locy.push_back(miny);
    locz.push_back(minz+0.5*binSize_z);

    locx.push_back(maxx);
    locy.push_back(maxy);
    locz.push_back(minz+0.5*binSize_z);

    locx.push_back(minx);
    locy.push_back(maxy);
    locz.push_back(minz+0.5*binSize_z);

    for(int i=0; i<locx.size(); i++)
    {
        string name = "detector" + to_string(i);;
        double loc[3] = {0,0,0};
        loc[0] = locx[i];
        loc[1] = locy[i];
        loc[2] = locz[i];

        int countingNumber = 0; // we hadn't known the counting number yet
        cout<<"detector ID : "<<i<<", name : "<<name<<",    intensity : "<<countingNumber<<", location : ("<<loc[0]<<", "<<loc[1]<<", "<<loc[2]<<")"<<endl;
        detectors.push_back(DataDetector(name,loc[0],loc[1],loc[2],-1,-1,-1,-1,countingNumber,intrinsic_efficiency_,detect_area));
    }

    //
    // step 2 : set detectors
    //
    for(int i=0;i<detectors.size();i++)
    {
        process_part1.setADetector(detectors[i]);
    }

    //
    // step 3 : set intensities 
    //
    /*
    // debug
    process_part1.debug_step0_setGroundTruth_intensities_initiation();
    process_part1.debug_step1_setGroundTruth_AnIntensity(0,1e6);
    process_part1.debug_step1_setGroundTruth_AnIntensity(3,1e6);
    process_part1.debug_step1_setGroundTruth_AnIntensity(9,1e6);
    process_part1.debug_step2_getDetectorCounts();
    */

    process_part1.debug_step0_setGroundTruth_intensities_initiation();

    import_sourcesGroundTruth(process_part1, "parameters_sourcesGroundTruth.txt");

    process_part1.debug_step2_getDetectorCounts();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // part 2 : do estimation
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    cout<<"Part 2"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    //
    // step 0 : set parameters
    //
    ProcessEM process_part2("process_part2", binx, minx, maxx, biny, miny, maxy, binz, minz, maxz);
    vector<DataDetector> detectors_part2;

    //
    // step 1 : import data
    //
    string filename = "input.txt";
    ifstream read(filename.c_str());

    if(read.fail())
    {
        cout<<"Data File \""<<filename<<"\" did not EXIST!"<<endl;
        return 0;
    }

    int detectorID = -1;
    double intensity = 0;
    double x=0;
    double y=0;
    double z=0;

    int counter = 0;
    while(!read.eof())
    {
        read>>detectorID>>intensity>>x>>y>>z;

        if(read.eof()) break;

        string name = "detector" + to_string(counter);;
        /*
        // debug
        cout<<"detector ID : "<<detectorID<<",    intensity : "<<intensity<<", location x,y,z : ("<<x<<", "<<y<<", "<<z<<")"<<endl;
        */
        detectors_part2.push_back(DataDetector(name,x,y,z,-1,-1,-1,-1,intensity,intrinsic_efficiency_,detect_area));

        counter ++;
    }

    read.close();

    //
    // step 2 : set detectors
    //
    for(int i=0;i<detectors_part2.size();i++)
    {
        process_part2.setADetector(detectors_part2[i]);
    }

    //
    // step 3 : set intensities 
    //
    process_part2.doEMEstimation(20);

    return 0;
}

int import_sourcesGroundTruth(ProcessEM &processEM, string filename)
{
    // filename = "parameters_sourcesGroundTruth.txt"
    ifstream read(filename.c_str());

    if(read.fail())
    {
        cout<<"File \""<<filename<<"\" did not exist!"<<endl;
        return 0;
    }

    int ID = 0;
    double intensity = 0;

    while(!read.eof())
    {
        read>>ID>>intensity;

        if(read.eof()) break;

        processEM.debug_step1_setGroundTruth_AnIntensity(ID,intensity);
        /*
        // debug
        cout<<"ID : "<<ID<<"; intensity : "<<intensity<<endl;
        */
    }

    read.close();

    return 1;
}
