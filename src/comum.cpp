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

void Receita::inverte_coluna(void)
{
  std::vector<Pose> aux_1;
  std::vector<Pose> aux_2;
  std::vector<Pose> aux_3;
  double aux2;
  // double diferenca=(Caixa.weight<Caixa.length?Caixa.weight:Caixa.length);
  int inicio=0,fim=0,k=0,j=0,i=0;
  int size_vector=all_poses.size();
  for(int i=0;i<Layers;i++){
    while(fim!=PlacesCamada*(i+1)){
      if (fim>=size_vector)break;
      aux2 =  all_poses[fim].Y;
      //while(!(abs(all_poses[fim].Y) < abs(aux2)-diferenca|| abs(aux2)+diferenca< abs(all_poses[fim].Y))){
      while(fim<size_vector && aux2 == all_poses[fim].Y){
        //begfile_in/end
        aux_1.insert(aux_1.begin(),all_poses[fim]);
        //std::cout << " " << all_poses[fim].Y;
        fim++;
      }
      //std::cout << endl;
      for (auto &outt : aux_1){
        aux_2.insert(aux_2.end(),outt);
      }
      aux_1.clear();    
    }
    for (auto &outt : aux_2){
      aux_3.push_back(outt);
    }
    aux_2.clear();
  }
  all_poses=aux_3;
}

void Receita::inverte_linha(void)
{
  std::vector<Pose> aux_1;
  std::vector<Pose> aux_2;
  std::vector<Pose> aux_3;
  double aux2;
  int inicio=0,fim=0,k=0,j=0,i=0;
  int size_vector=all_poses.size();
  for(int i=0;i<Layers;i++){
    while(fim!=PlacesCamada*(i+1)){
      if (fim>=size_vector)break;
      aux2 =  all_poses[fim].Y;
      while(fim<size_vector && aux2 == all_poses[fim].Y){
        //begfile_in/end
        aux_1.insert(aux_1.begin(),all_poses[fim]);
        //std::cout << " " << all_poses[fim].Y;
        fim++;
      }
      //std::cout << endl;
      for (auto &outt : aux_1){
        aux_2.insert(aux_2.begin(),outt);
      }
      aux_1.clear();    
    }
    for (auto &outt : aux_2){
      aux_3.push_back(outt);
    }
    aux_2.clear();
  }
  all_poses=aux_3;
}


void Receita::quadrante_vector(int quadrante)
{
  switch (quadrante){
    case 1:
      inverte_linha();
      inverte_coluna();
      break;
    case 2:
      inverte_linha();
      break;
    case 3:
      inverte_coluna();
      break;
    case 4:
      //all_poses=all_poses;
      break;
    default:
      //all_poses=all_poses;
      break;
  }
}

void ERROR_messege(std::string mensagem)
{
  std::cout << "<span style=\"color:red\">**ERROR: " << mensagem << "**</span>"<<endl;
}

std::string split_string(std::string tokenString,string delim,int saida)
{
  vector<string> tokens;
  std::regex delimiters {delim};
  sregex_token_iterator tokens_begin { tokenString.begin(), tokenString.end(), delimiters, -1 };
  auto tokens_end=sregex_token_iterator {};
  for (auto token_it=tokens_begin; token_it != tokens_end; token_it++)tokens.push_back(*token_it);
  return tokens[saida];
}

vector<string> split_string(std::string tokenString,std::string delim)
{
  vector<string> tokens;
  std::regex delimiters {delim};
  sregex_token_iterator tokens_begin { tokenString.begin(), tokenString.end(), delimiters, -1 };
  auto tokens_end=sregex_token_iterator {};
  for (auto token_it=tokens_begin; token_it != tokens_end; token_it++)tokens.push_back(*token_it);
  return tokens;
}

double valor(std::string entrada,int retorno)
{
  // return stod(split_string(entrada,"[^0-9]+",1));
  vector<string> aux=split_string(entrada,"[:, ]+");
  //imprime_vetor(aux);
  return stod(aux[retorno]);
}

void imprime_vetor(vector<string> tokens)
{
  int i=0;
  for (const string& token: tokens){
    std::cout << "[" << i << "]"<< "-> " << token << "\n";
    i++;
  }
}

void init_files(std::ofstream &src,std::ofstream &dat,std::string name)
{
  src << "DEF " << name <<"()"<< endl;
  dat << "DEFDAT " << name << endl;
  return;
}

void end_files(std::ofstream &src,std::ofstream &dat)
{
  src << "END";
  dat << "ENDDAT";
  return;
}

std::string Pose::kuka(void)
{
  std::stringstream aux;
  aux << "{X " << X << ",Y " << Y << ",Z " << Z << ",A " << A << ",B 0,C 180,S 2,T 2}";
  return aux.str();
}

void Receita::imprime(std::ofstream &ofs)
{
  std::stringstream aux;
  ofs<<";FOLD Produto "    << nome << endl;

  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.X=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.Y=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.Z=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.A=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.B=0"<< endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].OffsetProduto.C=0"<< endl;

  ofs<<"  StrReceita[Prdt_"<< nome << "].AlturaCaixa="<<AlturaCaixa << endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].PlacesCamada=" << PlacesCamada << endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].Layers="<< Layers << endl;
  ofs<<"  StrReceita[Prdt_"<< nome << "].Camadas="<< Camadas << endl;

  int i=1;
  for (auto &outt : LayersVector)
  {
    ofs<<"  StrReceitaLayer[Prdt_"<< nome <<","<<i<<"]="<< outt << endl;
    i++;
  }
  ofs<<";ENDFOLD\n" << endl;
  
  return;
}
bool buscar_chave(std::string entrada,std::string chave)
{
  return entrada.find(chave) !=std::string::npos;
}

std::ostream &operator<<(std::ostream &os, Pose const &m)
{
  os << "{ X " << m.X;
  os << ", Y " << m.Y;
  os << ", Z " << m.Z;
  os << ", A " << m.A;
  os << ", B " << m.B;
  os << ", C " << m.C;
  os << " }";
  return os;
}

std::ostream &operator<<(std::ostream &os, Objeto const &m)
{
  // os << "width : "  << m.width << endl;
  // os << "length : " << m.length << endl;
  // os << "height : " << m.height << endl;
  os << "Dimensoes : "  << m.width << " x " << m.length << " x " << m.height << endl;
  os << "Peso : " << m.weight;

  return os;
}

std::ostream &operator<<(std::ostream &os, Receita const &m)
{
  os << "# >> RECEITA <<\n"<< endl;
  os << "nome: " << m.nome << endl;
  os << "PlacesCamada: " << m.PlacesCamada << endl;
  os << "AlturaCaixa: " << m.AlturaCaixa << endl;
  os << "Camadas: " << m.Camadas << endl;
  os << "Layers: " << m.Layers << endl;
  os << "FinalContador: " << m.FinalContador << endl;
  os << "\n**Caixa**\n" << m.Caixa << endl;
  os << "\n**Pallet**\n" << m.Pallet << endl;
  os << "\n**LayersVector**\n";
  int i=1;
  for (auto &outt : m.LayersVector){
    os << (i<10?"[ ":"[") << i++ <<"] { " << outt << " }"<<endl;
  }
  os << "\n-----------------------------\n" << endl;
  os << "# >> POSICOES <<\n"<< endl;
  i=1;
  for (auto &outt : m.all_poses) os << (i<10?"[ ":"[") << i++ <<"]" << outt<<endl;
  os << "\n-----------------------------\n" << endl;
  return os;
}



