#ifndef simulacao_h
#define simulacao_h

void simulacao_maker(std::ofstream &src,std::ofstream &dat,int pallet,class Receita receita,class Pose app);
void simulacao_ponto(std::ofstream &src,std::ofstream &dat,int i,class Pose pose,bool type,int pallet);
void padrao_move(std::ofstream &out);

#endif