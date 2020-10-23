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
#include "simulacao.h"

void simulacao_maker(std::ofstream &simulacao_src,std::ofstream &simulacao_dat,int pallet,class Receita receita,class Pose app)
{
  class Pose XApp1Place,XApp2Place,XPlace;
  int i=0,j=0,Layers=0;
  for (auto &outt : receita.all_poses)
  {
    //valor compromisso de engenharia
    //Place
    XPlace=outt;
    //sobe os layers
    XPlace.Z+=receita.AlturaCaixa*Layers;
    if((j+1)%receita.PlacesCamada==0)Layers++;
    j++;
    //App2
    XApp2Place=XPlace;
    XApp2Place.X+=app.X;
    XApp2Place.Y+=app.Y;
    XApp2Place.Z+=receita.AlturaCaixa/2;
    //App1
    XApp1Place=XApp2Place;
    XApp1Place.Z+=receita.AlturaCaixa/2+app.Y;
    simulacao_src << "pick()"<< endl;
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XApp1Place,false,pallet);
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XApp2Place,false,pallet);
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XPlace,false,pallet);
    simulacao_src << "place()"<< endl;
  }
  return;
}

void simulacao_ponto(std::ofstream &src,std::ofstream &dat,int i,class Pose pose,bool type,int Pallet)
{
  if(type)
  {
    src << ";FOLD PTP P"<<i<<" CONT Vel= 100 %  PDATP3 Tool[1] Base["<<Pallet<<"]   ;%{PE}" << endl;
  }else{
    src << ";FOLD LIN P"<<i<<" CONT Vel= 2 m/s CPDATP3 Tool[1] Base["<<Pallet<<"]   ;%{PE}" << endl;
  }
  src << ";FOLD Parameters ;%{h}"<< endl;
  if(type)
  {
    src << ";Params IlfProvider=kukaroboter.basistech.inlineforms.movement.old; Kuka.IsGlobalPoint=False; Kuka.PointName=P"<<i<<"; Kuka.BlendingEnabled=True; Kuka.MoveDataPtpName=PDATP3; Kuka.VelocityPtp=100; Kuka.CurrentCDSetIndex=0; Kuka.MovementParameterFieldEnabled=True; IlfCommand=PTP" << endl;
  }else{
    src << ";Params IlfProvider=kukaroboter.basistech.inlineforms.movement.old; Kuka.IsGlobalPoint=False; Kuka.PointName=P"<<i<<"; Kuka.BlendingEnabled=True; Kuka.MoveDataName=CPDATP3; Kuka.VelocityPath=2; Kuka.CurrentCDSetIndex=0; Kuka.MovementParameterFieldEnabled=True; IlfCommand=LIN" << endl;
  }
  src << ";ENDFOLD"<< endl;
  src << (type?"PTP":"LIN") <<" XP"<<i<< endl;
  src << ";ENDFOLD"<< endl;

  dat << "DECL E6POS XP"<<i<<"={X "<<pose.X<<",Y "<<pose.Y<<",Z "<<pose.Z<<",A "<<pose.A<<",B "<<pose.B<<",C 180,S 2,T 2}" << endl;
}

void padrao_move(std::ofstream &out)
{
  out << ";FOLD PTP HOME  Vel= 100 % DEFAULT;%{PE}%MKUKATPBASIS,%CMOVE,%VPTP,%P 1:PTP, 2:HOME, 3:, 5:100, 7:DEFAULT"<< endl;
  out << "    $BWDSTART = FALSE"<< endl;
  out << "    PDAT_ACT=PDEFAULT"<< endl;
  out << "    FDAT_ACT=FHOME"<< endl;
  out << "    BAS (#PTP_PARAMS,100 )"<< endl;
  out << "    $H_POS=XHOME"<< endl;
  out << "    PTP  XHOME"<< endl;
  out << " ;ENDFOLD"<< endl;

  out << ";FOLD CONFIGURACOES PADRAO DE MOVIMENTO" << endl;
  out << "   $VEL.CP = 2.0  ; 0 a 3 m/s: velocidade da ponta do TCP" << endl;
  out << "   $ACC.CP = 2.0  ; 0 a 2  m/s^2: aceleracao da ponta do TCP" << endl; 
  out << "   $APO.CDIS = 50 ; Blend a ser utilizado"<< endl;
  out << "   $TOOL = TOOL_DATA[1]" << endl;
  out << "   ;$BASE = BASE_DATA[5]" << endl;
  out << ";ENDFOLD " << endl;
  return;
}