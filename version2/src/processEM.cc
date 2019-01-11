#include "processEM.hh"

ProcessEM::ProcessEM(string name, int binx, double minx, double maxx, int biny, double miny, double maxy, int binz, double minz, double maxz)
:   name_(name),
    xaxis_("XAxis", binx, minx, maxx),
    yaxis_("YAxis", biny, miny, maxy),
    zaxis_("ZAxis", binz, minz, maxz),
    iterationTimes_(0)
{
    // debug
    cout<<"Process EM"<<endl;
    cout<<"    Name : "<<name_<<endl;

    // clear
    detectors_.clear();

    // step 0 : initate sources
    initiateSources();
}

ProcessEM::~ProcessEM()
{}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// public functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::setADetector(DataDetector adetector)
{
    DataDetector theDetector = adetector;

    // save distances
    for(int i=0;i<sources_.size();i++)
    {
        double deltax = sources_[i].getX() - theDetector.getX();
        double deltay = sources_[i].getY() - theDetector.getY();
        double deltaz = sources_[i].getZ() - theDetector.getZ();
        double distance = sqrt(deltax*deltax + deltay*deltay + deltaz*deltaz);

        /*
        // debug
        cout<<"source ID "<<i<<", location "<<sources_[i].getX()<<", "<<sources_[i].getY()<<", "<<sources_[i].getZ()<<" : delta ("<<deltax<<", "<<deltay<<", "<<deltaz<<"), distance "<<distance<<endl;
        */

        theDetector.addADistance(distance);
    }

    detectors_.push_back(theDetector);

    // debug
    theDetector.show();
    for(int i=0;i<sources_.size();i++)
    {
        double distance_currentPixel = theDetector.getDistance(i);
        double efficiency_area = theDetector.getAnEfficiencyArea(i);
        cout<<"source ID "<<i<<", location "<<sources_[i].getX()<<", "<<sources_[i].getY()<<", "<<sources_[i].getZ()<<" : distance "<<distance_currentPixel<<", efficiency_area "<<efficiency_area<<endl;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// EM Estimation 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::doEMEstimation(int iterationTimes)
{
    //
    cout<<"EM Estimation begin"<<endl;
    iterationTimes_ = iterationTimes;

    //// step 0 : initiate Detectors
    //for(int i=0;i<detectors_.size();i++)
    //{
    //    // get efficiency for all sources : detector i
    //    detectors_[i].doInitiation();

    //    /*
    //    // debug
    //    double efficiency_detector = detectors_[i].getEfficiencyAllSources();
    //    cout<<"Detector ID "<<i<<": "<<efficiency_detector<<endl;
    //    */
    //}

    // step 0 : initiate
    for(int j=0;j<sources_.size();j++)
    {
        double efficiency_allDetectors = 0; 
        for(int i=0;i<detectors_.size();i++)
        {
            efficiency_allDetectors += detectors_[i].getAnEfficiencyTotal(j);
            /*
            // debug
            cout<<"efficiency for sourceID "<<j<<", detectorID "<<i<<": "<<detectors_[i].getAnEfficiencyTotal(j)<<endl;
            */
        }
        efficiencies_ASource_allDetectors_.push_back(efficiency_allDetectors);

        /*
        //debug
        cout<<"efficiencies_ASource_allDetectors_ : sourceID "<<j<<", efficiency "<<efficiency_allDetectors<<endl;
        */
    }

    /*
    // debug
    for(int i=0;i<efficiencies_ASource_allDetectors_.size();i++)
    {
        cout<<"sourceID "<<i<<", efficiencies_ASource_allDetectors_ "<<efficiencies_ASource_allDetectors_[i]<<endl;
    }
    */
    
    // step 1 : iterations
    for(int k=0;k<iterationTimes_;k++)
    for(int j=0;j<sources_.size();j++)
    {
        cout<<"Iteration, sourceID : ("<<k<<", "<<j<<")"<<endl;
        // a function
        int sourceID = j;
        anIteration(sourceID);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug : set ground truth 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::debug_step0_setGroundTruth_intensities_initiation()
{
    // initiate
    for(int i=0;i<sources_.size();i++)
    {
        sources_[i].setGroungTruth_Intensty(0);
    }

    /*
    // set values
    sources_[0].setGroungTruth_Intensty(1e6);
    */
}

void ProcessEM::debug_step1_setGroundTruth_AnIntensity(int sourceID, double intensity)
{
    // set values
    sources_[sourceID].setGroungTruth_Intensty(intensity);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug  : get detector real counts
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::debug_step2_getDetectorCounts()
{
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    cout<<"~~ Debug Begin"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

    // step 1
    string filename1 = "debug_log_calculatingProcesses.txt";
    string filename2 = "debug_log_detectorCountingNumbers.txt";

    ofstream write1(filename1.c_str());
    ofstream write2(filename2.c_str());


    // step 2
    for(int i=0;i<detectors_.size();i++)
    {
        // detector i
        cout<<"Detector ID "<<i<<": ";

        double count_allSources = 0;
        // get intrinsic efficiency
        double efficient_instrinsic = detectors_[i].getEfficiencyIntrinsic();
        cout<<"Efficiency intrinsic  "<<efficient_instrinsic<<", "<<endl;

        //
        // writing
        //
        write1<<"Detector ID "<<i<<": ";
        write1<<"Efficiency intrinsic  "<<efficient_instrinsic<<", "<<endl;

        // get geo efficiency
        // get intensities ground truth
        for(int j=0;j<sources_.size();j++)
        {
            double efficiency_area = detectors_[i].getAnEfficiencyArea(j);
            cout<<"Efficiency area  "<<efficiency_area<<", ";
            double count_groundTruth = sources_[j].getGroungTruth_Intensty();
            cout<<"count Ground Truth "<<count_groundTruth<<", ";
            double count = efficient_instrinsic*efficiency_area*count_groundTruth;
            cout<<"detector counting number "<<count<<endl;
            count_allSources += count;

            //
            // writing
            //
            write1<<"Efficiency area  "<<efficiency_area<<", ";
            write1<<"count Ground Truth "<<count_groundTruth<<", ";
            write1<<"detector counting number "<<count<<endl;
        }
        cout<<"Count for all sources : "<<count_allSources<<endl;

        //
        // writing
        //
        write1<<"Count for all sources : "<<count_allSources<<"\n\n";

        double locationX = detectors_[i].getX();
        double locationY = detectors_[i].getY();
        double locationZ = detectors_[i].getZ();
        write2<<i<<"    "<<count_allSources<<"    "<<locationX/mm<<"    "<<locationY/mm<<"    "<<locationZ/mm<<endl;


    }
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    cout<<"~~ Debug End"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

    write1.close();
    write2.close();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sources' controlling
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::initiateSources()
{
    // get bins
    int binx = xaxis_.GetBinTotalNumber();
    int biny = yaxis_.GetBinTotalNumber();
    int binz = zaxis_.GetBinTotalNumber();

    // clear
    sources_.clear();

    // initiate sources
    for(int k=0;k<binz; k++)
    for(int j=0;j<biny; j++)
    for(int i=0;i<binx; i++)
    {
        int ID = getID(i,j,k);
        int IDx = -1;
        int IDy = -1;
        int IDz = -1;
        getIDForXYZ(ID, IDx, IDy, IDz);

        double locx = xaxis_.GetBinCoordinate(IDx);
        double locy = yaxis_.GetBinCoordinate(IDy);
        double locz = zaxis_.GetBinCoordinate(IDz);

        /*
        // debug
        cout<<"x y z ("<<i<<" "<<j<<" "<<k<<") : "<<ID<<" -- ("<<IDx<<" "<<IDy<<" "<<IDz<<");";
        cout<<" Location : ("<<locx<<" "<<locy<<" "<<locz<<")\n";
        */

        // add a source, i.e. a pixel
        DataSource ASource("ASourcePixel",locx,locy,locz,IDx,IDy,IDz,ID,10.);
        sources_.push_back(ASource);
    }

    // debug
    for(int i=0;i<sources_.size();i++)
    {
        sources_[i].show();
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~
// EM iteration : an iteration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ProcessEM::anIteration(int sourceID)
{
    //
    // Parameters introduction 
    //
    // intensity_next : lambda_j(l+1)
    // intensity_current : lambda_j(l)
    // lambda_current : lambda_k(l)
    // s : probability that a photon emitted from pixel j would be detected anywhere, s_j
    // Y : counting number for each detector
    // t : the probability that a gamma emmited from pixel j is collected in detector i, t_ij
    // sourceID : j
    // detectorID : i

    //
    // step 0 : get parameters
    //
    // intensity_next : lambda_j(l+1)
    double intensity_next = 0;
    // intensity_current : lambda_j(l)
    double intensity_current = sources_[sourceID].getIntensty();
    // s : probability that a photon emitted from pixel j would be detected anywhere, s_j
    double s = efficiencies_ASource_allDetectors_[sourceID];

    /*
    // debug
    cout<<"intensity_current : "<<intensity_current<<endl;
    */

    //
    // step 1 : get the Right part of the iteration's equation
    //
    double RightPart = 0;
    for(int i=0;i<detectors_.size();i++)
    {
        // get the demominator
        double t_Times_lambda_allSources = 0;
        for(int k=0;k<sources_.size();k++)
        {
            double t_current = detectors_[i].getAnEfficiencyTotal(k);
            double lambda_current = sources_[k].getIntensty();
            t_Times_lambda_allSources += (t_current*lambda_current);
        }

        // get the numerator
        double Y_current = detectors_[i].getCounts();
        double t_current_numerator = detectors_[i].getAnEfficiencyTotal(sourceID);
        double Y_Times_t_currentDetector = Y_current*t_current_numerator;

        /*
        // debug
        cout<<"t_Times_lambda_allSources for detectorID "<<i<<": "<<t_Times_lambda_allSources<<endl;
        cout<<"Y_Times_t_currentDetector for detectorID "<<i<<": "<<Y_Times_t_currentDetector<<endl;
        */

        // let the numerator be divided by the demominator
        double part1 = Y_Times_t_currentDetector/t_Times_lambda_allSources;
        RightPart += part1;
    }

    // step 2 : get the left part of the iteration's equation
    double LeftPart = 0;
    LeftPart = intensity_current/s;

    // step 3 : get the intensity for the next ieteration
    intensity_next = LeftPart*RightPart;

    // step 4 : Refresh the intensity for each source
    sources_[sourceID].setIntensty(intensity_next);

    /*
    // debug
    cout<<"RightPart "<<RightPart<<endl;
    cout<<"the intensity after the current ieteration : "<<intensity_next<<endl;
    */
    cout<<"the intensity after the current ieteration : "<<intensity_next<<endl;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histgrams' controlling
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
int ProcessEM::getID(int IDx, int IDy, int IDz)
{
    // get bins
    int binx = xaxis_.GetBinTotalNumber();
    int biny = yaxis_.GetBinTotalNumber();
    int binz = zaxis_.GetBinTotalNumber();

    // initiate ID
    int ID = 0;

    // get ID
    ID = ID + IDz*binx*biny;
    ID = ID + IDy*binx;
    ID = ID + IDx;

    return ID;
}

void ProcessEM::getIDForXYZ(int ID, int& IDx, int & IDy, int& IDz)
{
    // initiate IDs
    IDx = -1;
    IDy = -1;
    IDz = -1;

    // get bins
    int binx = xaxis_.GetBinTotalNumber();
    int biny = yaxis_.GetBinTotalNumber();
    int binz = zaxis_.GetBinTotalNumber();

    // get IDs
    IDz = ID/(binx*biny);
    IDy = ID%(binx*biny)/binx;
    IDx = ID%(binx*biny)%binx;
}
