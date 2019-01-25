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

//
// functions
//
void importData_countingNumbers(string filename, vector<double> &countingNumbers);

int main(int argc,char** argv)
{
    cout<<"Hello"<<endl;

    /*
    //DataDetector d1("detector1",1,2,3,1,2,3,1e5,1e-2);
    DataSource s1("source1",1,2,3,1,2,3,1,1e4);
    */


    /*
    // way 1
    ProcessEM process1("process1", 2, 0, 400, 2, 0, 800, 1, 0, 1e3);

    // set detectors
    double radius = 40.*mm;
    double detect_area = PI*radius*radius;
    vector<double> detectorCounts;
    detectorCounts.push_back(320);
    detectorCounts.push_back(246);
    detectorCounts.push_back(130);
    detectorCounts.push_back(142);
    DataDetector adetector0("detector0",0,  0,  500,-1,-1,-1,-1,detectorCounts[0],1e-2,detect_area);
    DataDetector adetector1("detector1",400,0,  500,-1,-1,-1,-1,detectorCounts[1],2e-2,detect_area);
    DataDetector adetector2("detector2",0,  800,500,-1,-1,-1,-1,detectorCounts[2],3e-2,detect_area);
    DataDetector adetector3("detector3",400,800,500,-1,-1,-1,-1,detectorCounts[3],4e-2,detect_area);
    process1.setADetector(adetector0);
    process1.setADetector(adetector1);
    process1.setADetector(adetector2);
    process1.setADetector(adetector3);


    process1.debug_step0_setGroundTruth_intensities_initiation();
    process1.debug_step1_setGroundTruth_AnIntensity(0,1e6);
    process1.debug_step2_getDetectorCounts();

    process1.doEMEstimation(20);
    */


    // way 2
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // part 1 : get detector counting numbers
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // 
    // step 0
    //
    // set detectors
    double radius = radius_;
    double detect_area = PI*radius*radius;

    vector<DataDetector> detectors;

    //
    // step 1 : import data from a file
    //
    double  minx = 0;
    double  maxx = 400;
    int     binx = 4;

    double  miny = 0;
    double  maxy = 800;
    int     biny = 2;

    double  minz = 0;
    double  maxz = 1e3;
    int     binz = 1;

    double  binSize_x = (maxx-minx)/double(binx);
    double  binSize_y = (maxy-miny)/double(biny);
    double  binSize_z = (maxz-minz)/double(binz);

    ProcessEM process_part1("process_part1", binx, minx, maxx, biny, miny, maxy, binz, minz, maxz);

    vector<double> detectorCountingNumbers;

    string filename = "input_countingNumbers.txt";
    importData_countingNumbers(filename,detectorCountingNumbers);

    for(int i=0;i<detectorCountingNumbers.size();i++)
    {
        string name = "detector" + to_string(i);;
        double loc[3] = {0,0,0};
        loc[0] = minx + (double(i)+0.5)*binSize_x;
        loc[1] = 0.;
        loc[2] = 1e3/2.;

        cout<<"detector ID : "<<i<<", name : "<<name<<",    intensity : "<<detectorCountingNumbers[i]<<", location : ("<<loc[0]<<", "<<loc[1]<<", "<<loc[2]<<")"<<endl;
        detectors.push_back(DataDetector(name,loc[0],loc[1],loc[2],-1,-1,-1,-1,detectorCountingNumbers[i],1e-2,detect_area));
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
    process_part1.debug_step0_setGroundTruth_intensities_initiation();
    process_part1.debug_step1_setGroundTruth_AnIntensity(0,1e6);
    process_part1.debug_step2_getDetectorCounts();

    //process_part1.doEMEstimation(20);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // part 2 : do estimation
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ProcessEM process_part2("process_part2", binx, minx, maxx, biny, miny, maxy, binz, minz, maxz);

    return 0;
}

void importData_countingNumbers(string filename, vector<double> &countingNumbers)
{
    // intensity is counting number

    ifstream read(filename.c_str());

    if(read.fail())
    {
        cout<<"Data File \""<<filename<<"\" did not EXIST!"<<endl;
        return;
    }

    int detectorID = -1;
    double intensity = 0;
    double x=0;
    double y=0;
    double z=0;

    while(!read.eof())
    {
        read>>detectorID>>intensity>>x>>y>>z;

        if(read.eof()) break;

        /*
        // debug
        cout<<"detector ID : "<<detectorID<<",    intensity : "<<intensity<<", location x,y,z : ("<<x<<", "<<y<<", "<<z<<")"<<endl;
        */

        countingNumbers.push_back(intensity);
    }

    read.close();
}
