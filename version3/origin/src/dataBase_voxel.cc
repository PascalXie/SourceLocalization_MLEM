#include "dataBase_voxel.hh"

DataBase_Voxel::DataBase_Voxel(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID)
:   name_(name),
    x_(x*mm),
    y_(y*mm),
    z_(z*mm),
    IDx_(IDx),
    IDy_(IDy),
    IDz_(IDz),
    ID_(ID)
{}

DataBase_Voxel::~DataBase_Voxel()
{}

double DataBase_Voxel::getX()
{
    return x_;
}
double DataBase_Voxel::getY()
{
    return y_;
}
double DataBase_Voxel::getZ()
{
    return z_;
}
