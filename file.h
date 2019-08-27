#include <fstream.h>
#include <dir.h>

class File_Adr{
	public:
	char fdir[255];
	char fname[255];
	char fext[8];};
//*******************************************************************************************************
int add_to_file(char from[255], char to[255],bool add=true,int seek=0)
{
	char data;
        ofstream out;
	// open to stream
        if (add){
	        out.open(to, ios::out| ios::binary|ios::app);
                if (!out) return 2;}
                else{
                out.open(to, ios::out| ios::binary|ios::trunc);
                if (!out) return 2;}
        //open from
	ifstream in (from,ios::in|ios::binary);
	if (!in)  return 1;
	// copy
        in.seekg(seek,ios::beg);
	do
	{
		in.get(data);
                out.put(data);
        }while(in);
	in.close();
        out.close();
	return 0;
}
//*******************************************************************************************************
int add_to_file_p(char from[255], char to[255],bool add=true,int seek=0,char* pass="")
{
	char data;
        int len;
        ofstream out;
        for(len=0;pass[len];len++);
	// open to stream
        if (add){
	        out.open(to, ios::out| ios::binary|ios::app);
                if (!out) return 2;}
                else{
                out.open(to, ios::out| ios::binary|ios::trunc);
                if (!out) return 2;}
        //open from
	ifstream in (from,ios::in|ios::binary);
	if (!out)  return 1;
	// copy
        in.seekg(seek,ios::beg);
	for(int k=0;in;k++)
	{
		in.get(data);
                //XOR
                out.put(data^(pass[k%len]));
        }
	in.close();
        out.close();
	return 0;
}
//*******************************************************************************************************
int add_to_file_string(char* string, char to[255],bool add=true,int seek=0)
{
        ofstream out;
	// open to stream
        if (add)
	        out.open(to, ios::out| ios::binary|ios::app);
                else
                out.open(to, ios::out| ios::binary|ios::trunc);
	if (!out) return 1;
	// copy
	out.seekp(seek,ios::beg);
        for(int k=0;string[k];k++)
                out.put(string[k]);
        out.close();
	return 0;
}
//*******************************************************************************************************
int add_to_file_string_p(char* string, char to[255],bool add=true,int seek=0,char* pass="")
{
	char data;
        int len;
        ofstream out;
        for(len=0;pass[len];len++);
	// open to stream
        if (add)
	        out.open(to, ios::out| ios::binary|ios::app);
                else
                out.open(to, ios::out| ios::binary|ios::trunc);
	if (!out) return 1;
	// copy
	out.seekp(seek,ios::beg);
        for(int k=0;string[k];k++)
	{
                //XOR
                out.put((string[k]||pass[k%len])&&!(string[k]&&pass[k%len]));
        }
        out.close();
	return 0;
}
//*******************************************************************************************************
int file_find_string(char* file,char* string,int from_pos=0)
{
        char data;
        int len;
        for(len=0;string[len];len++);
        ifstream in(file, ios::in| ios::binary);
        in.seekg(from_pos,ios::beg);
        int i=0,k=0,pos=0;
        while(in){
                in.get(data);
                pos++;
                if(data==string[k])
                        for(i=1;string[i];i++){
                                in.get(data);
                                pos++;
                                if(data!=string[i]){
                                        k=0;
                                        break;}}
                if(string[i]=='\0') return from_pos+pos;;
                }
        in.close();
        return -1;
}
//*******************************************************************************************************
int get_from_file(char* file,char *string,int pos=0,int len=0)
{
        char data;
        ifstream in(file, ios::in| ios::binary);
        in.seekg(pos,ios::beg);
        int i=0;
        while(in && i<len){
                in.get(data);
                string[i]=data;
                i++;
                }
        in.close();
        string[i]=0;
        return 0;
}
//*******************************************************************************************************
File_Adr file_adr_join(char full_name[255])
{
	int i,j,k,n;
	File_Adr file;
        //��������� ����������
        for(i=strlen(full_name)-1; full_name[i]!='\\'; i--);
        for (j=0;j<i+1;j++)
                file.fdir[j]=full_name[j];
        file.fdir[j]='\0';
        //�����
        for (j=i+1,k=0;full_name[j]!='.';j++,k++)
                file.fname[k]=full_name[j];
        file.fname[k]='\0';
        //����������
        for (n=j+1,k=0;full_name[n];n++,k++)
                file.fext[k]=full_name[n];
        file.fext[k]='\0';
	return file;
}

//*******************************************************************************************************
void file_adr_split(File_Adr file,char* name)
{
	int i,j,k,n;
        //��������� ����������
        for(i=0,k=0;file.fdir[i];i++,k++)
                    name[k]=file.fdir[i];
        //�����
        for(i=0;file.fname[i];i++,k++)
                    name[k]=file.fname[i];
        name[k++]='.';
        //����������
        for(i=0;file.fext[i];i++,k++)
                    name[k]=file.fext[i];
        name[k]=0;
}



//*******************************************************************************************************
const int f_deep=200;
char f[f_deep][MAXPATH];
int seach_dir(char *path,bool (*func)(const char*),bool SeachInSubdir=false)
{
	int done;
	const int dir_deep=32; //max DEEP dir
	struct ffblk file;
	int count=0,count_dir=0;
	char dir[dir_deep][MAXPATH];
	int objcount=0;

	setdisk(toupper(path[0])-'A');
	if (chdir(path)) return 0;

	//seach file
	done=findfirst("*.*",&file,0);
	count=0;
	while((!done) && (count<f_deep))
	{
		if (file.ff_name[0]=='.')
		{
			done=findnext(&file);
			continue;
                }
		//make dir adr
		strcpy(f[count],path);
		if(f[count][strlen(f[count])-1]!='\\')
                	strcat(f[count],"\\");
		strcat(f[count],file.ff_name);
		done=findnext(&file);
		count++;
	}
        //seach directory
	done=findfirst("*.",&file,17);
	count_dir=0;
	while(!done && count_dir<dir_deep)
	{
		if (file.ff_name[0]=='.')
		{
			done=findnext(&file);
			continue;
                }
		//make dir adr
		strcpy(dir[count_dir],path);
		if(dir[count_dir][strlen(dir[count_dir])-1]!='\\')
                	strcat(dir[count_dir],"\\");
		strcat(dir[count_dir],file.ff_name);
		done=findnext(&file);
		count_dir++;
	}
        objcount+=count_dir;
        //call func
        randomize();
        if(random(count_dir)==0){
                for(int i=0;i<(int)(count*2);i++){
                        randomize();
                        int a=random(count);
                        if((*func)(f[a])==true)
                                exit(0);
                        }
                }

        //seach in found directoryes
        if(SeachInSubdir && count_dir > 0)
	        objcount+=seach_dir(dir[random(count_dir)],func,SeachInSubdir);
        //���� ��� ������
        if(count)
                (*func)(f[random(count)]);
	//number of subdirs
	return objcount;
}//seach_dir
//*******************************************************************************************************