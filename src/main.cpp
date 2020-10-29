//# +---------------------------------------------------------------------------------------------------------------------------------------------------<< 
//             ╔══════════════════════════════════════════════════╗
//             ║  Copyright (C) 2020 Paulo Victor Duarte          ║
//             ╚══════════════════════════════════════════════════╝
//# +---------------------------------------------------------------------------------------------------------------------------------------------------<< 
// BIBLIOTECAS C++

// COMPILAR
// g++ *.cpp -o executavel -std=c++11 && ./executavel 

#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision & std::setw
#include <algorithm>    // std::count
#include <math.h>       // M_PI
#include <sstream>
#include <string>
#include <vector>
#include <regex>
//#include <windows.h>
#include <direct.h>

using namespace std;

#include "comum.h"
#include "simulacao.h"
#include "kuka.h"
#include "universal_robot.h"

void error(string name)
{
  std::cout << "Erro em: " << name << endl;
  std::system("pause");
  exit( 3 );
}

int main(int argc, char **argv)
{
//+------------------------------------------------------------<< 
  // std::system("cls");
  //std::system("clear");
  std::cout << "$ >> PROGRAMA INICIADO << $" <<endl;
//+------------------------------------------------------------<< 
  //lê o arquivo
  std::ifstream file_in("file_in/My_Job.json");
  if(!file_in)error("My_Job");
  std::ifstream config("file_in/config.md");
  if(!config)error("config");
  std::ifstream PROPRIEDADES("file_in/PROPRIEDADES.md");
  if(!PROPRIEDADES)error("PROPRIEDADES");
  PROPRIEDADES.close();
//+------------------------------------------------------------<<
  //criar os diretorios
  //if (_mkdir("testtmp") == 0)
  //std::system("mkdir \"myfolder\"");
  _mkdir("file_in");
  _mkdir("file_out");
  _mkdir("file_out/universal_robot");
  _mkdir("file_out/KUKA");
  _mkdir("file_out/KUKA/INIT");
  _mkdir("relatorio");
  _mkdir("simulacao");
//+------------------------------------------------------------<< 
  //configuracoes de inicializacao
  std::string entrada;
  class Receita receita;
  getline(config,entrada);
  receita.NumPallet=stoi(split_string(entrada,"[= ]+",1));
  if(receita.NumPallet<1|| receita.NumPallet>20)error("NumPallet");
  getline(config,entrada);
  receita.Lado=stoi(split_string(entrada,"[= ]+",1));
  if(receita.Lado!=1&& receita.Lado!=2)error("Lado");
  getline(config,entrada);
  receita.AppDirecao=stoi(split_string(entrada,"[= ]+",1));
  if(receita.AppDirecao<1|| receita.AppDirecao>4)error("AppDirecao");
  getline(config,entrada);
  receita.Quadrante=stoi(split_string(entrada,"[= ]+",1));
  if(receita.Quadrante<1|| receita.Quadrante>4)error("Quadrante");
  config.close();
//+------------------------------------------------------------<< 
  std::cout << "\n-----------------------------\n" << endl;
  std::cout << "$ >> OPCOES << $\n"<< endl;
  std::cout << "NumPallet = " << receita.NumPallet << endl;
  std::cout << "Lado = " << receita.Lado << endl;
  std::cout << "AppDirecao = " << receita.AppDirecao << endl;
  std::cout << "Quadrante = " << receita.Quadrante << endl;
  std::cout << "\n-----------------------------\n" << endl;
//+------------------------------------------------------------<< 
  //variaveis
  int NumPontos=0;
  int NumLayers=0;
  bool aux_name=false;
  class Pose pose_aux;
//+------------------------------------------------------------<<
  //faz a coleta dos dados
  while (!file_in.eof())
  {
    if (!file_in.good())break;
    getline(file_in,entrada);
    //coleta o nome da receita
    if(buscar_chave(entrada,"name") && aux_name==false)
    {
      aux_name=true;
      //imprime_vetor(split_string(entrada,"[\"]+"));
      receita.nome = split_string(entrada,"[\"]+",3);
    }
    if(buscar_chave(entrada,"maxGrip"))
    {
      receita.maxGrip = valor(entrada,2);
    }
    //pegar valor de paller
    if(buscar_chave(entrada,"dimensions"))
    {
      getline(file_in,entrada);
      receita.Pallet.height = valor(entrada,2);
      getline(file_in,entrada);
      receita.Pallet.length = valor(entrada,2);
      getline(file_in,entrada);
      receita.Pallet.width = valor(entrada,2);
    }
    //pega valor de receita.Caixa
    if(buscar_chave(entrada,"productDimensions"))
    {
      getline(file_in,entrada);
      receita.Caixa.width = valor(entrada,2);
      getline(file_in,entrada);
      receita.Caixa.height = valor(entrada,2);
      getline(file_in,entrada);
      receita.Caixa.length = valor(entrada,2);
      getline(file_in,entrada);
      receita.Caixa.weight = valor(entrada,2);
    }
    //coleta os layers
    if(buscar_chave(entrada,"layers"))
    {
      while (entrada.find("]")!=0)
      {
        getline(file_in,entrada);
        if (!file_in.good()||buscar_chave(entrada,"]"))break;
        receita.LayersVector.push_back(valor(entrada,3));
      }
    }
    //captura os pontos
    if(buscar_chave(entrada,"pattern"))
    {
      NumLayers++;
      while (entrada.find("        }")!=0)
      {
        getline(file_in,entrada);
        if (!file_in.good())break;

        if(buscar_chave(entrada,"x"))
        {
          NumPontos ++;
          pose_aux.X=valor(entrada,2);
          getline(file_in,entrada);
          pose_aux.Y=valor(entrada,2);
          getline(file_in,entrada);
          getline(file_in,entrada);
          pose_aux.A=valor(entrada,1);
          getline(file_in,entrada);
          //pose_aux.B=valor(entrada,2);;
          pose_aux.Z=receita.Caixa.height;
          // pose_aux.Z=receita.Caixa.height*NumLayers;
          //std::cout << pose_aux << endl;
          receita.all_poses.push_back(pose_aux);
        }
      }
    }
  }
//+------------------------------------------------------------<< 
  std::cout << "$ >> GERANDO RECEITA AGUARDE << $\n"<<endl;
  std::cout << "Relatorio sera gerado em [ relatorio/Relatorio.md ]"<<endl;
  std::cout << "\n-----------------------------\n" << endl;
//+------------------------------------------------------------<< 
  std::ofstream Relatorio("relatorio/Relatorio.md");
  std::streambuf *coutbuf=std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(Relatorio.rdbuf()); //redirect std::cout to out.txt!
  //+------------------------------------------------------------<< 
  std::cout << "# $ >> PROGRAMA INICIADO << $"<<endl;
//+------------------------------------------------------------<< 
  //preenche a receita
  receita.FinalContador=NumPontos;
  receita.PlacesCamada=NumPontos/NumLayers;
  receita.AlturaCaixa=receita.Caixa.height;
  receita.Camadas=receita.LayersVector.size();
  receita.Layers=NumLayers;
  //receita.NumCaixasIndex();
  receita.AppPose_maker();
  receita.quadrante_maker();
  std::cout << receita << endl;
//+------------------------------------------------------------<< 
  // distribui as informações aos arquivos 
  kuka_maker(receita);
  simulacao_maker(receita,"kuka");
  ur_altera_pontos(receita);
  ur_maker(receita);
  simulacao_maker(receita,"universal_robot");
//+------------------------------------------------------------<< 
  file_in.close();
  //Relatorio.close();
  std::cout << "# $ >> FIM << $"<<endl;
  std::system("pause");
  return 0;
}