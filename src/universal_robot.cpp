#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision & std::setw
#include <algorithm>    // std::count
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

#include "comum.h"
#include "universal_robot.h"

std::string ur_pose(class Pose pose)
{
  std::stringstream aux;
  aux << "p[";
  aux << pose.X/1000<<", "<<pose.Y/1000<<", "<<pose.Z/1000<<", ";
  //aux << pose.C*(M_PI/180)<<", "<<pose.B*(M_PI/180)<<", "<<pose.A*(M_PI/180)<<"]";
  aux << "2.221352, 2.221606, "<<pose.A*(M_PI/180);
  aux << "]";
  return aux.str();
}

void ur_pontos(std::ofstream &ur_file,int pallet,int NumPlace,class Receita receita,class Pose pose,class Pose app)
{
  class Pose XApp1Place,XApp2Place,XPlace;
  //valor compromisso de engenharia
  //Place
  XPlace=pose;
  //App2
  XApp2Place=pose;
  XApp2Place.X+=app.X;
  XApp2Place.Y+=app.Y;
  XApp2Place.Z+=receita.AlturaCaixa/2;
  //App1
  XApp1Place=XApp2Place;
  XApp1Place.Z+=(receita.AlturaCaixa/2)+app.Z;
  //+-------------------------- SRC --------------------------+<<
  std::stringstream endereco;
  endereco.clear(); 

  endereco << "  movel(pose_trans(";
  endereco << "p[pallet"<<pallet<<"_dx/1000.0,pallet"<<pallet<<"_dy/1000.0,delta_z/1000.0,";
  endereco << "d2r(pallet"<<pallet<<"_rx),d2r(pallet"<<pallet<<"_ry),d2r(pallet"<<pallet<<"_rz)],";

  ur_file<< endereco.str() << ur_pose(XApp1Place) << "),a=acc,v=vel,r=0.000000)" << endl;
  ur_file<< endereco.str() << ur_pose(XApp2Place) << "),a=acc,v=vel,r=0.000000)" << endl;
  ur_file<< endereco.str() << ur_pose(XPlace) << "),a=acc,v=vel,r=0.000000)" << endl;

  return;
}


void ur_separa_layers(std::ofstream &ur_file,int layer,int inicial,int final,int pallet)
{
  ur_file << "\nend\n" << endl;
  ur_file << "sync()\n" << endl;
  ur_file << "end #P"<<pallet<<"L"<<layer-1<<"\n"<< endl;
  ur_file << endl;
  ur_file << "# LAYER "<<layer<<": PLACE "<<inicial << " ate " << final << endl;
  ur_file << "def P"<<pallet<<"L"<<layer<<"(caixa):"<<endl;

}

void ur_altera_pontos(class Receita &receita)
{
  int quadrante=receita.quadrante;
  for (auto &outt:receita.all_poses)
  {
    if(outt.A==90)
    {
      outt.A=0;
      outt.pick_ur=2;
    }
    if(quadrante == 3 || quadrante == 4)
    {
      outt.A=(outt.A+180)>360?outt.A+180-360:outt.A+180;
    }
  }
}

int ur_maker(int pallet,class Receita receita,class Pose app)
{
//+------------------------------------------------------------<< 
  //abre os arquivos
  std::ofstream ur_file("file_out/universal_robot.script",std::ofstream::out);
  if( !ur_file )
  {
    std::cout << "Erro ao abrir os arquivos ur_file.\n";
    std::system("pause");
    return 0;
  }
  int layer=1;
  int contador=1;
  //init_files(TReceita_src,TReceita_dat,"TReceita");

  ur_file << "# LAYER "<<layer<<": PLACE "<<contador << " ate " << receita.PlacesCamada*layer << endl;
  ur_file << "def P"<<pallet<<"L"<<layer<<"(caixa):"<< endl;

  for (auto &outt : receita.all_poses){
    if (contador==(receita.PlacesCamada*layer+1)&& receita.Layers>1)
    {
      layer++;
      contador=1;
      ur_separa_layers(ur_file,layer,contador,receita.PlacesCamada,pallet);
    }
    ur_file<<"  # PLACE " << contador << endl;
    ur_file << "  "<<(contador==1?"if":"elif");
    ur_file << "(caixa=="<<contador<<"):" << endl;
    ur_file << "  App"<<1<<"Palete"<<pallet<<"() #subroutine call" << endl;
    ur_pontos(ur_file,pallet,contador,receita,outt,app);
    ur_file << "  Deposita() #subroutine call" << endl;
    contador++;
  }

  ur_file << "\nend\n" << endl;
  ur_file << "sync()\n" << endl;
  ur_file << "end #P"<<pallet<<"L"<<layer<<"\n"<< endl;
  //end_files(TReceita_src,TReceita_dat);
  ur_file.close();
}






