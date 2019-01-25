#include <vector>
//
// parameters
//
int IterationTotalNumber_ = 20;
vector<TCanvas*>  cs; 
vector<TH2D*>  hs; 

int     binx_ = 4;
double  minx_ = 0;
double  maxx_ = 400;

int     biny_ = 4;
double  miny_ = 0;
double  maxy_ = 800;

int     binz_ = 1;
double  minz_ = 0;
double  maxz_ = 1e3;


//
// functions
//
int drawAnrIiteration(int fileID);

int alice_step1()
{
    cout<<"Hello"<<endl;

    for(int i=0; i<IterationTotalNumber_; i++)
    {
        int isFileReadingSucess = drawAnrIiteration(i);
    }

    return 0;
}

int drawAnrIiteration(int fileID)
{
    string name_p1 = "output_";
    string name_p3 = ".txt";
    string name_p2 = to_string(fileID);
    string filename = name_p1 + name_p2 + name_p3;

    string hName = "his_iteration_"+to_string(fileID);
    TH2D * h = new TH2D(hName.c_str(),hName.c_str(),binx_,minx_,maxx_,biny_,miny_,maxy_);

    // 
    // step 1 : reading 
    //
    cout<<filename<<endl;
    ifstream read(filename.c_str());

    if(read.fail())
    {
        cout<<"File \""<<filename<<"\" dose not exist!"<<endl;
        return 0;
    }

    double sourceID = -1;
    double intensityID = -1;
    double x = -1;
    double y = -1;
    double z = -1;
    string temp;

    while(!read.eof())
    {
        read>>temp>>temp>>sourceID>>temp
            >>temp>>temp>>intensityID>>temp
            >>temp>>temp>>x>>temp
            >>temp>>temp>>y>>temp
            >>temp>>temp>>z;

        if(read.eof()) break;

        cout<<sourceID<<" "<<intensityID<<" "<<x<<" "<<y<<" "<<z<<endl;

        h->Fill(x,y,intensityID);
    }

    read.close();


    //
    // step 2 : drawing
    //
    string cName = "map_iteration_"+to_string(fileID);
    TCanvas * c = new TCanvas(cName.c_str());

    c->cd();
    h->Draw("colz");

    c->SaveAs((cName+ ".png").c_str());







    return 1;
}
