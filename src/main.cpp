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
#include "matriz_pontos.h"

int main(int argc, char **argv)
{
//+------------------------------------------------------------<< 
  // std::system("cls");
  //std::system("clear");
  std::cout << "$ >> PROGRAMA INICIADO << $" <<endl;
//+------------------------------------------------------------<< 
  //lê o arquivo
  std::ifstream file_in("file_in/My_Job.json");
  if( !file_in )
  {
    std::cout << "Erro ao abrir os arquivos My_Job.\n";
    std::system("pause");
    return 0;
  }
  std::ifstream PROPRIEDADES("file_in/PROPRIEDADES.md");
  if( !PROPRIEDADES)
  {
    std::cout << "Erro ao abrir os arquivos PROPRIEDADES.\n";
    std::system("pause");
    return 0;
  }
  PROPRIEDADES.close();
//+------------------------------------------------------------<<
  //criar os diretorios
  //if (_mkdir("testtmp") == 0)
  //std::system("mkdir \"myfolder\"");
  _mkdir("file_in");
  _mkdir("file_out");
  _mkdir("file_out/INIT");
  _mkdir("relatorio");
  _mkdir("simulacao");
//+------------------------------------------------------------<< 
  //configuracoes de inicializacao
  int pallet=1;
  int quadrante=3;
  class Pose app;
  //Quadrante e Pallet
  int system_in;
  std::cout << "Digite o valor do Pallet [1..20]" << endl;
  cin  >> system_in;
  pallet = system_in;
  if(pallet<1||pallet>20){
    cout << "valor invalido de pallet" << endl;
    std::system("pause");
    return 0;
  }
  std::cout << "Digite o valor do quadrante [1..4]" << endl;
  cin  >> system_in;
  quadrante = system_in;
  if(quadrante<1||quadrante>4){
    cout << "valor invalido de quadrante" << endl;
    std::system("pause");
    return 0;
  }
//+------------------------------------------------------------<< 
  std::cout << "\n-----------------------------\n" << endl;
  std::cout << "$ >> OPCOES << $\n"<< endl;
  std::cout << "Quadrante: " << quadrante << " | " << "Pallet: " << pallet << endl;
  std::cout << "\n-----------------------------\n" << endl;
//+------------------------------------------------------------<< 
  //determina quadrante
  switch (quadrante){
    case 1:
      app.X=(+60);
      app.Y=(-60);
      break;
    case 2:
      app.X=(-60);
      app.Y=(-60);
      break;
    case 3:
      app.X=(+60);
      app.Y=(+60);
      break;
    case 4:
      app.X=(-60);
      app.Y=(+60);
      break;
    default:
      app.X=(-60);
      app.Y=(+60);
      break;
  }
  app.Z=100;
//+------------------------------------------------------------<< 
  //variaveis
  std::string entrada;
  int NumPontos=0;
  int NumLayers=0;
  bool aux_name=false;

  class Pose pose_aux;
  class Receita receita;
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
  //abre os arquivos
  std::ofstream simulacao_src("simulacao/simulacao.src",std::ofstream::out);
  std::ofstream simulacao_dat("simulacao/simulacao.dat",std::ofstream::out);
  if( !simulacao_src || !simulacao_dat)
  {
    std::cout << "Erro ao abrir os arquivos simulacao.\n";
    std::system("pause");
    return 0;
  }

  std::ofstream TReceita_src("file_out/INIT/TReceita.src",std::ofstream::out);
  std::ofstream TReceita_dat("file_out/INIT/TReceita.dat",std::ofstream::out);
  if( !TReceita_src  || !TReceita_dat){std::cout << "Erro ao abrir os arquivos TReceita.\n";return 0;}
  if( !TReceita_dat )
  {
    std::cout << "Erro ao abrir os arquivos TReceita.\n";
    std::system("pause");
    return 0;
  }

  std::ofstream TMatriz_src ("file_out/INIT/TMatriz.src", std::ofstream::out);
  std::ofstream TMatriz_dat ("file_out/INIT/TMatriz.dat", std::ofstream::out);
  if( !TMatriz_src || !TMatriz_dat )
  {
    std::cout << "Erro ao abrir os arquivos TMatriz.\n";
    std::system("pause");
    ;return 0;
  }

  init_files(simulacao_src,simulacao_dat,"simulacao");
  init_files(TReceita_src,TReceita_dat,"TReceita");
  init_files(TMatriz_src,TMatriz_dat,"TMatriz");
  //padrao_move(simulacao_src);

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
  std::cout << "# >> OPCOES <<\n"<< endl;
  std::cout << "Quadrante: " << quadrante << " | " << "Pallet: " << pallet << endl;
  std::cout << "\n-----------------------------\n" << endl;
//+------------------------------------------------------------<< 
  //preenche a receita
  receita.FinalContador=NumPontos;
  receita.PlacesCamada=NumPontos/NumLayers;
  receita.AlturaCaixa=receita.Caixa.height;
  receita.Camadas=receita.LayersVector.size();
  receita.Layers=NumLayers;
  //receita.NumCaixasIndex();
  receita.imprime(TReceita_src);
  receita.quadrante_vector(quadrante);
  std::cout << receita << endl;
//+------------------------------------------------------------<< 
  // distribui as informações aos arquivos 
  matriz_maker(TMatriz_src,TMatriz_dat,pallet,receita,app);
  simulacao_maker(simulacao_src,simulacao_dat,pallet,receita,app);
//+------------------------------------------------------------<< 
  end_files(simulacao_src,simulacao_dat);
  end_files(TReceita_src,TReceita_dat);
  end_files(TMatriz_src,TMatriz_dat);
  simulacao_src.close();
  simulacao_dat.close();
  TReceita_src.close();
  TReceita_dat.close();
  TMatriz_src.close();
  TMatriz_dat.close();
  file_in.close();
  //Relatorio.close();
  std::cout << "# $ >> FIM << $"<<endl;
  std::system("pause");
  return 0;
}