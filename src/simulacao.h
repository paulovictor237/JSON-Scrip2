#ifndef simulacao_h
#define simulacao_h

int simulacao_maker(int pallet,class Receita receita,class Pose app,string name);
void simulacao_ponto(std::ofstream &src,std::ofstream &dat,int i,class Pose pose,int Pallet,bool type);
void padrao_move(std::ofstream &out);

#endif