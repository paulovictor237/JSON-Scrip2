#ifndef kuka_h
#define kuka_h

int kuka_maker(int pallet,class Receita receita,class Pose app);
std::string kuka_pose(class Pose pose);
void matriz_pontos(std::ofstream &TMatriz_src,std::ofstream &TMatriz_dat,int pallet,int NumPlace,class Receita receita,class Pose pose,class Pose app);
void separa_layers(std::ofstream &file,int layer,int inicial,int final);


#endif