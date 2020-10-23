#ifndef matriz_pontos_h
#define matriz_pontos_h

void matriz_maker(std::ofstream &TMatriz_src,std::ofstream &TMatriz_dat,int pallet,class Receita receita,class Pose app);
void matriz_pontos(std::ofstream &TMatriz_src,std::ofstream &TMatriz_dat,int pallet,int NumPlace,class Receita receita,class Pose pose,class Pose app);
void separa_layers(std::ofstream &file,int layer,int inicial,int final);


#endif