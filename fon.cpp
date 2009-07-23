//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include<vcl>
#include<winuser>
#include<file.h>
#include<iostream>
#pragma argsused
bool Result=false;
bool func(const char* str)
{
        Result=SystemParametersInfo(SPI_SETDESKWALLPAPER,0,(void *)str,SPIF_UPDATEINIFILE);
        return Result;
}
bool main(int argc, char* argv[])
{
        char dir[MAXPATH];
        File_Adr adr;
        char name[128];
        //файл с директорией поиска
        adr = file_adr_join(argv[0]);
        strcpy(name,adr.fname);
        strcat(name,"_cnf.txt");
        //
        if (argc==2)
                if(!strcmp(argv[1],"help")){
                        cout<<"Bez parametrov poick v papke, kotoraya ykazana v faile "<<name<<"\r\n"<<
                                "Odin parametr - imya faila, kotorii yctanovitcya kak fonovii ricynok rabochego ctola\r\nhelp - cpravka.";
                        return 0;}
        if (argc==1){
                ifstream in(name,ios::binary);
                in.get(dir,1024);
                in.close();
                }
                else{
                strcpy(dir,argv[1]);
                for(int i=2;i<argc;i++){
                        strcat(dir," ");
                        strcat(dir,argv[i]);}
                }
        if(func(dir)) return true;
        cout<<"Changing wallpapper..."<<"\r\n";
        cout<<"Seach in "<<dir;
        seach_dir(dir,func,true);
        cout<<Result;
        return Result;
}
//---------------------------------------------------------------------------
 