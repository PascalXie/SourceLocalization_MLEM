#ifndef DATABASE_VOXEL_HH
#define DATABASE_VOXEL_HH 1

#include <iostream>
#include <string>

#include "units.hh"

using namespace std;

class DataBase_Voxel
{
    public:
        DataBase_Voxel(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID);
        ~DataBase_Voxel();

    public:
        double getX();
        double getY();
        double getZ();

    protected:
        string  name_;
        double  x_, y_, z_; // location
        int     ID_;
        int     IDx_;
        int     IDy_;
        int     IDz_;
};
#endif
