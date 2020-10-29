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

int simulacao_maker(class Receita &receita,string name)
{
  std::ofstream simulacao_src("simulacao/"+name+".src",std::ofstream::out);
  std::ofstream simulacao_dat("simulacao/"+name+".dat",std::ofstream::out);
  if( !simulacao_src || !simulacao_dat)
  {
    std::cout << "Erro ao abrir os arquivos simulacao.\n";
    std::system("pause");
    return 0;
  }
  init_files(simulacao_src,simulacao_dat,"simulacao");
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
    XApp2Place.X+=receita.AppPose.X;
    XApp2Place.Y+=receita.AppPose.Y;
    XApp2Place.Z+=receita.AlturaCaixa/2;
    //App1
    XApp1Place=XApp2Place;
    XApp1Place.Z+=receita.AlturaCaixa/2+receita.AppPose.Y;
    simulacao_src << endl;
    simulacao_src << ";FOLD ; ## PontoPlace ["<<j<<"] ##;%{PE}%R 8.5.16,%MKUKATPBASIS,\%CCOMMENT,%VNORMAL,%P 2: ## PontoPlace ["<<j<<"] ## \n;ENDFOLD" << endl;
    simulacao_src << "add()"<< endl;
    simulacao_src << "NumCaixas = " << outt.NumCaixas << endl;
    simulacao_src << "pick()"<< endl;
    if(name=="universal_robot")
    {
      simulacao_src << "pick"<<outt.pick_ur<<"()"<< endl;
      simulacao_src << "receita.AppPose"<<outt.AppPalete<<"Palete"<<receita.NumPallet<<"()"<< endl;
    }
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XApp1Place,receita.NumPallet,false);
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XApp2Place,receita.NumPallet,false);
    i++;
    simulacao_ponto(simulacao_src,simulacao_dat,i+20,XPlace,receita.NumPallet,false);
    simulacao_src << "place()"<< endl;
  }
  end_files(simulacao_src,simulacao_dat);
  simulacao_src.close();
  simulacao_dat.close();
  return 0;
}

void simulacao_ponto(std::ofstream &src,std::ofstream &dat,int i,class Pose pose,int Pallet,bool type)
{
  if(type==false)
  {
    src << ";FOLD LIN P"<<i<<" CONT Vel= 2 m/s CPDATP"<<i<<"Tool[1] Base["<<Pallet<<"]   ;%{PE}" << endl;
    src << ";FOLD Parameters ;%{h}"<< endl;
    src << ";Params IlfProvider=kukaroboter.basistech.inlineforms.movement.old; Kuka.IsGlobalPoint=False; Kuka.PointName=P"<<i<<"; Kuka.BlendingEnabled=True; Kuka.MoveDataName=CPDATP"<<i<<"; Kuka.VelocityPath=2; Kuka.CurrentCDSetIndex=0; Kuka.MovementParameterFieldEnabled=True; IlfCommand=LIN" << endl;
    src << ";ENDFOLD"<< endl;

    src << "$BWDSTART = FALSE"<< endl;
    src << "LDAT_ACT=LCPDATP"<<i<< endl;
    src << "FDAT_ACT=FP"<<i<< endl;
    src << "BAS (#CP_PARAMS,2)"<< endl;
    src << "SET_CD_PARAMS (0)"<< endl;
    src << "LIN" <<" XP"<<i<< endl;
    src << ";ENDFOLD"<< endl;

    dat << "DECL LDAT LCPDATP"<<i<<"={VEL 1,ACC 100,APO_DIST 10,APO_FAC 50.0,ORI_TYP #VAR,CIRC_TYP #BASE,JERK_FAC 50.0}" << endl;
    dat << "DECL FDAT FP"<<i<<"={TOOL_NO 1,BASE_NO "<<Pallet<<",IPO_FRAME #BASE,POINT2[] \" \",TQ_STATE FALSE}" << endl;
    dat << "DECL E6POS XP"<<i<<"={X "<<pose.X<<",Y "<<pose.Y<<",Z "<<pose.Z<<",A "<<pose.A<<",B "<<pose.B<<",C 180,S 2,T 2}" << endl;


  }
  else
  {
    src << ";FOLD PTP P"<<i<<" CONT Vel= 100 % PDATP"<<i<<"Tool[1] Base["<<Pallet<<"]   ;%{PE}" << endl;

    src << ";FOLD Parameters ;%{h}"<< endl;
    src << ";Params IlfProvider=kukaroboter.basistech.inlineforms.movement.old; Kuka.IsGlobalPoint=False; Kuka.PointName="<<i<<"; Kuka.BlendingEnabled=True; Kuka.MoveDataPtpName=PDATP"<<i<<"; Kuka.VelocityPtp=100; Kuka.CurrentCDSetIndex=0; Kuka.MovementParameterFieldEnabled=True; IlfCommand=PTP"<< endl;
    src << ";ENDFOLD"<< endl;

    src << "$BWDSTART = FALSE"<< endl;
    src << "PDAT_ACT=PPDATP"<<i<< endl;
    src << "FDAT_ACT=FP"<<i<< endl;
    src << "BAS (#PTP_PARAMS,100)"<< endl;
    src << "SET_CD_PARAMS (0)"<< endl;
    src << "PTP XP"<<i<<" C_DIS"<< endl;
    src << ";ENDFOLD"<< endl;

    dat << "DECL PDAT PPDATP"<<i<<"={VEL 100,ACC 100,APO_DIST 100,APO_MODE #CDIS,GEAR_JERK 100.000,EXAX_IGN 0}" << endl;
    dat << "DECL FDAT FP"<<i<<"={TOOL_NO 1,BASE_NO "<<Pallet<<",IPO_FRAME #BASE,POINT2[] \" \",TQ_STATE FALSE}"<< endl;
    dat << "DECL E6POS XP"<<i<<"={X "<<pose.X<<",Y "<<pose.Y<<",Z "<<pose.Z<<",A "<<pose.A<<",B "<<pose.B<<",C 180,S 2,T 2}" << endl;

  }
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