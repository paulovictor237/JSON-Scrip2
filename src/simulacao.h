#ifndef simulacao_h
#define simulacao_h

int simulacao_maker(class Receita &receita,string name);
void simulacao_ponto(std::ofstream &src,std::ofstream &dat,int i,class Pose pose,int Pallet,bool type);
void padrao_move(std::ofstream &out);

#endif