#ifndef PROCESSEM_HH
#define PROCESSEM_HH 1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

#include "dataDetector.hh"
#include "dataSource.hh"
#include "histgram.hh"

using namespace std;

class ProcessEM
{
    public:
        ProcessEM(string name, int binx, double minx, double maxx, int biny, double miny, double maxy, int binz, double minz, double maxz);
        ~ProcessEM();
    public:
        void setADetector(DataDetector adetector);
        void doEMEstimation(int iterationTimes);

        // debug
        void debug_step0_setGroundTruth_intensities_initiation();
        void debug_step1_setGroundTruth_AnIntensity(int sourceID, double intensity);
        void debug_step2_getDetectorCounts();

    private:
        void initiateSources();
        int getID(int IDx, int IDy, int IDz);
        void getIDForXYZ(int ID, int& IDx, int & IDy, int& IDz);
        // Iteration
        void anIteration(int sourceID);

    private:
        string name_;
        vector<DataDetector>    detectors_;
        vector<DataSource>      sources_;

        // Axis
        Histgram xaxis_;
        Histgram yaxis_;
        Histgram zaxis_;

        // Loop
        int iterationTimes_;

        // efficiency
        vector<double> efficiencies_ASource_allDetectors_; // s_j
};

#endif
