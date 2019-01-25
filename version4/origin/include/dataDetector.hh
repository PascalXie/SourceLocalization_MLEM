#ifndef DATADETECTOR_HH
#define DATADETECTOR_HH 1

#include <vector>

#include "dataBase_voxel.hh"

using namespace std;

class DataDetector : public DataBase_Voxel
{
    public:
        DataDetector(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID, double counts, double efficiency_intrinsic, double detect_area);
        ~DataDetector();

    public:
        void addADistance(double adistance);
        double getDistance(int sourceID);

        double getAnEfficiencyArea(int sourceID);
        double getAnEfficiencyTotal(int sourceID);

        double getEfficiencyIntrinsic();

        double getCounts();

        void show();


    private:
        double counts_;

        //
        // Efficiency
        //
        // Intrinsic Efficiency
        double efficiency_intrinsic_;
        // Geometric efficiency
        double detect_area_;
        vector<double> distances_;
        vector<double> efficiencies_area_;

};
#endif
