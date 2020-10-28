#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision & std::setw
#include <algorithm>    // std::count
#include <math.h>       // M_PI
#include <sstream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

#include "comum.h"
# include "kuka.h"

std::string kuka_pose(class Pose pose)
{
  std::stringstream aux;
  aux << "{X " << pose.X << ",Y " << pose.Y << ",Z " << pose.Z << ",A " << pose.A << ",B 0,C 180,S 2,T 2}";
  return aux.str();
}

void kuka_imprime(std::ofstream &ofs,class Receita receita)
{
  std::stringstream aux;
  ofs<<";FOLD Produto "    << receita.nome << endl;

  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.X=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.Y=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.Z=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.A=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.B=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].OffsetProduto.C=0"<< endl;

  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].AlturaCaixa="<<receita.AlturaCaixa << endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].PlacesCamada=" <<receita.PlacesCamada << endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].Layers="<< receita.Layers << endl;
  ofs<<"  StrReceita[Prdt_"<< receita.nome << "].Camadas="<< receita.Camadas << endl;

  int i=1;
  for (auto &outt : receita.LayersVector)
  {
    ofs<<"  StrReceitaLayer[Prdt_"<< receita.nome <<","<<i<<"]="<< outt << endl;
    i++;
  }
  ofs<<";ENDFOLD\n" << endl;
  
  return;
}


void add_propriedades(std::ofstream &TMatriz_src,std::stringstream &endereco){
  std::ifstream file_in("file_in/PROPRIEDADES.md");
  if( !file_in )
  {
    std::cout << "Erro ao abrir os arquivos PROPRIEDADES.\n";
    return;
  }
  //faz a coleta dos dados
  std::string entrada;
  while (!file_in.eof())
  {
    getline(file_in,entrada);
    if (!file_in.good())break;
    TMatriz_src<<endereco.str()<<entrada<<endl;
  }
}


int kuka_maker(int pallet,class Receita receita,class Pose app)
{
//+------------------------------------------------------------<< 
  //abre os arquivos
  std::ofstream TReceita_src("file_out/KUKA/INIT/TReceita.src",std::ofstream::out);
  std::ofstream TReceita_dat("file_out/KUKA/INIT/TReceita.dat",std::ofstream::out);
  if( !TReceita_src  || !TReceita_dat){std::cout << "Erro ao abrir os arquivos TReceita.\n";return 0;}
  if( !TReceita_dat )
  {
    std::cout << "Erro ao abrir os arquivos TReceita.\n";
    std::system("pause");
    return 0;
  }

  std::ofstream TMatriz_src ("file_out/KUKA/INIT/TMatriz.src", std::ofstream::out);
  std::ofstream TMatriz_dat ("file_out/KUKA/INIT/TMatriz.dat", std::ofstream::out);
  if( !TMatriz_src || !TMatriz_dat )
  {
    std::cout << "Erro ao abrir os arquivos TMatriz.\n";
    std::system("pause");
    ;return 0;
  }
  init_files(TReceita_src,TReceita_dat,"TReceita");
  init_files(TMatriz_src,TMatriz_dat,"TMatriz");
  //padrao_move(simulacao_src);
  kuka_imprime(TReceita_src,receita);
//+------------------------------------------------------------<< 
  int layer=1;
  int contador=1;
  // Receita -> int PlacesCamada=0,AlturaCaixa=0,Camadas=0,Layers=0;
  //abre fold da receita
  TMatriz_src << ";FOLD Pallet " <<pallet<<" - Produto "<< receita.nome << endl;
  TMatriz_dat << ";FOLD Pallet " <<pallet<<" - Produto "<< receita.nome << endl;
  //fold layer
  TMatriz_src << ";FOLD LAYER "<<layer<<": PLACE "<<contador << " ate " << receita.PlacesCamada*layer << endl;
  TMatriz_dat << ";FOLD LAYER "<<layer<<": PLACE "<<contador << " ate " << receita.PlacesCamada*layer << endl;

  for (auto &outt : receita.all_poses){
    if (contador==(receita.PlacesCamada*layer+1)&& receita.Layers>1)
    {
      layer++;
      separa_layers(TMatriz_src,layer,contador,receita.PlacesCamada*layer);
      separa_layers(TMatriz_dat,layer,contador,receita.PlacesCamada*layer);
    }
    matriz_pontos(TMatriz_src,TMatriz_dat,pallet,contador,receita,outt,app);
    contador++;
  }
  TMatriz_src<<";ENDFOLD" << endl;
  TMatriz_dat<<";ENDFOLD" << endl;

  //fecha fold da receita
  TMatriz_src<<";ENDFOLD" << endl;
  TMatriz_dat<<";ENDFOLD" << endl;

  end_files(TReceita_src,TReceita_dat);
  end_files(TMatriz_src,TMatriz_dat);
  TReceita_src.close();
  TReceita_dat.close();
  TMatriz_src.close();
  TMatriz_dat.close();
}


int OffsetPlace(std::ofstream &TMatriz_src,std::stringstream &endereco)
{
  TMatriz_src<<";FOLD OffsetPlace " << endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.X=0"<< endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.Y=0"<< endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.Z=0"<< endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.A=0"<< endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.B=0"<< endl;
  TMatriz_src<< endereco.str() << "OffsetPlace.C=0"<< endl;
  TMatriz_src<<";ENDFOLD" << endl;
  return 0;
}

void separa_layers(std::ofstream &file,int layer,int inicial,int final)
{
  file << ";ENDFOLD"<< endl;
  file << ";FOLD LAYER "<<layer<<": PLACE "<<inicial << " ate " << final << endl;
}

void matriz_pontos(std::ofstream &TMatriz_src,std::ofstream &TMatriz_dat,int pallet,int NumPlace,class Receita receita,class Pose pose,class Pose app)
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
  endereco <<"MatrizPontos[Pallet_";
  endereco <<pallet<<",Prdt_";
  endereco <<receita.nome; 
  endereco << "," ;
  endereco << NumPlace;
  endereco << "].";
  
  TMatriz_src<<";FOLD PLACE " << NumPlace << endl;
    TMatriz_src<<";FOLD PROPRIEDADES " << endl;
      add_propriedades(TMatriz_src,endereco);
      TMatriz_src<< endereco.str() <<"XApp1Place=P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"App1"<<endl;
      TMatriz_src<< endereco.str() <<"XApp2Place=P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"App2"<<endl;
      TMatriz_src<< endereco.str() <<"XPlace=P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"Place"<<endl;
    TMatriz_src << ";ENDFOLD" << endl;
      OffsetPlace(TMatriz_src,endereco);
      TMatriz_src<< endl;
  TMatriz_src << ";ENDFOLD\n" << endl;

  //+-------------------------- DAT --------------------------+<<   
  TMatriz_dat<<";FOLD PLACE " << NumPlace << endl;
  TMatriz_dat<<"DECL E6POS P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"App1"<<"="<<kuka_pose(XApp1Place)<<endl;
  TMatriz_dat<<"DECL E6POS P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"App2"<<"="<<kuka_pose(XApp2Place)<<endl;
  TMatriz_dat<<"DECL E6POS P"<<pallet<<"_"<<receita.nome<<"_"<<NumPlace<<"_"<<"Place"<<"="<<kuka_pose(XPlace)<<endl;
  TMatriz_dat<<";ENDFOLD\n" << endl; 

  return;
}