#ifndef comum_h
#define comum_h

class Pose{
public:
    double X=0,Y=0,Z=0,A=0,B=0,C=0;
public:
    std::string kuka(void);
    friend std::ostream &operator<<(std::ostream &os, Pose const &m);
};

class Objeto{
public:
    double width=0,height=0,length=0,weight=0;
public:
    friend std::ostream &operator<<(std::ostream &os, Objeto const &m);
};

class Receita{
public:
  std::string nome;
  int PlacesCamada=0,AlturaCaixa=0,Camadas=0,Layers=0,FinalContador=0;
  class Objeto Pallet,Caixa;
  std::vector<Pose> all_poses;
  std::vector <int> LayersVector;
public:
  void quadrante_vector(int quadrante);
  void inverte_linha(void);
  void inverte_coluna(void);
  void imprime(std::ofstream &ofs);
  friend std::ostream &operator<<(std::ostream &os, Receita const &m);
};

std::string split_string(std::string tokenString,std::string delim,int saida);
vector<string> split_string(std::string tokenString,std::string delim);
void imprime_vetor(vector<string> tokens);

double valor(std::string entrada,int retorno);
bool buscar_chave(std::string entrada,std::string chave);

void init_files(std::ofstream &src,std::ofstream &dat,std::string name);
void end_files(std::ofstream &src,std::ofstream &dat);

#endif