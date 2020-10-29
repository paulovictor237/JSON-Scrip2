#ifndef kuka_h
#define kuka_h

int kuka_maker(class Receita &receita);
std::string kuka_pose(class Pose pose);
void separa_layers(std::ofstream &file,int layer,int inicial,int final);
void matriz_pontos(std::ofstream &TMatriz_src,std::ofstream &TMatriz_dat,class Receita &receita,class Pose pose,int NumPlace)

#endif