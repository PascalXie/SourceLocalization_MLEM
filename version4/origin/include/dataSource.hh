#ifndef DATASOURCE_HH
#define DATASOURCE_HH 1

#include <vector>
#include "dataBase_voxel.hh"

class DataSource : public DataBase_Voxel
{
    public:
        DataSource(string name, double x, double y, double z, int IDx, int IDy, int IDz, int ID, double intensity);
        ~DataSource();

    public:
        void    setIntensty(double intensity);
        double  getIntensty();
        double  getIntensity(int iterationID);

        void    setGroungTruth_Intensty(double intensity);
        double  getGroungTruth_Intensty();

        void show();

    private:
        double intensity_;
        vector<double> vector_intensity_;
        double groundTruth_intensity_;
};
#endif

