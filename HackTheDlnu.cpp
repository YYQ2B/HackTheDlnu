#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
using namespace std;

struct node_filesystem                      //�ļ�ϵͳ  ���
{
    string name;                            //�˽������

    string document[1000];                  //�ļ���㣬�����鱣���ļ���
    int    document_length;
    node_filesystem *filedir[1000];         //
    int              filedir_length;

    node_filesystem *pri_dir;

    node_filesystem()
    {
        document_length = 0;
        filedir_length = 0;
    }
};

node_filesystem *rootdir = new node_filesystem();                    //��Ŀ¼
string target_ip;
string target_port;
string target_user;
string target_password;


node_filesystem *workpoint ;                //����ָ��

int filesystem_read(string filename)        //�ļ�ϵͳ
{
    rootdir->pri_dir = NULL;
    rootdir->name = "/";
    workpoint = rootdir;
    filename = "FileSystem\\" + filename;

    ifstream infile(filename.c_str());

	if(!infile)
	{
		cerr << "��ʼ�ļ���ȡ����" <<filename<<"\n";
		return 1;
	}

	string fileinput;
	string fileflag = "-";
    while(infile>>fileinput)
	{
	   // cout<<fileflag<<":"<<fileflag.length()<<endl;
	    int flag = 0;
	    for(int i = fileflag.length() - 1; i >= 0  ; i --)       //�ļ��жϱ�־���жϵ�ǰ¼������ļ������ļ���
        {
  //          cout<<"fileinput="<<fileinput<<"   fileflag="<<fileflag<<endl;
            if(fileinput[i] != fileflag[i])
            {
                if(fileinput[i] == '+')
                {
                    flag = 1;
                    break;
                }
                else
                {
                    workpoint = workpoint->pri_dir;
                    fileflag = fileflag.substr(0,fileflag.length()-1);
                }
            }
        }
        if (flag)
        {
            node_filesystem *newfilepoint = new node_filesystem();
            newfilepoint->name = fileinput.substr(fileflag.length(),fileinput.length());
            newfilepoint->pri_dir = workpoint;
            workpoint->filedir[workpoint->filedir_length++] = newfilepoint;
            workpoint = workpoint->filedir[workpoint->filedir_length - 1];
            fileflag += "-";
        }
        else
        {
            workpoint->document[workpoint->document_length++] = fileinput.substr(fileflag.length(),fileinput.length());
        }
	}
	infile.close();
	workpoint = rootdir;
}

void mail()                       //��ȡmail
{
    string email = "Mail_01.htd";
    email="MissionPack\\"+email;
    ifstream infile(email.c_str());         //��ȡ�ʼ�ϵͳ
    string input_file;
    system("cls");
    while(getline(infile , input_file))               //��ȡ�ʼ��Ի���ļ��е�IP��ַ
    {
        cout<<input_file<<endl;
        for(int i=0;i<input_file.length();i++)
        {
            if(input_file[i]=='I' && input_file[i+1]=='P')          //��ȡIP
            {
                target_ip=input_file.substr(3 , input_file.length());
            }
            if(input_file[i]=='p' && input_file[i+1]=='o' && input_file[i+2]=='r' &&input_file[i+3]=='t')   //��ȡ�˿�
            {
                target_port=input_file.substr(5 , input_file.length());
            }
             if(input_file[i]=='u' && input_file[i+1]=='s' && input_file[i+2]=='e' &&           //��ȡ�û���
                input_file[i+3]=='r')
            {
                target_user=input_file.substr(5 , input_file.length());
            }
           if(input_file[i]=='p' && input_file[i+1]=='a' && input_file[i+2]=='s' &&         //��ȡ����
                input_file[i+3]=='s' && input_file[i+4]=='w' && input_file[i+5]=='o'
                && input_file[i+6]=='r' &&input_file[i+7]=='d')
            {
                target_password=input_file.substr(9 , input_file.length());
            }

        }
    }
    cout<<endl<<endl;
}

void command_telnet(string IP , string port)
{
    void login();
    if(IP == "" || port  == "")
    {
        cout<<"wrong input !"<<endl;
        return;
    }
    if(IP==target_ip && port == target_port)
    {
        system("cls");
        login();
    }
}

void command_ls()
{
    cout<<endl;
    for(int i = 0; i < workpoint->filedir_length ; i++)
    {
        cout<<workpoint->filedir[i]->name<<" <dir>"<<endl;
    }
    for(int i = 0; i < workpoint->document_length ; i++)
    {
        cout<<workpoint->document[i]<<endl;
    }
    cout<<endl
        <<workpoint->document_length<<"  file(s)"<<endl
        <<workpoint->filedir_length<<"  folder(s)"<<endl<<endl;
}

void command_cd(string inputdir)
{
    if(inputdir == "")
    {
        cout<<endl
            <<"Syntax error! Correct syntax : cd foldername"<<endl<<endl;
        return ;
    }
    if(inputdir == "..")
    {
        if(workpoint->pri_dir != NULL)
        {
           workpoint = workpoint->pri_dir;
        }
        return ;
    }
    for(int i = 0 ; i < workpoint->filedir_length ; i++)
    {
        if(inputdir == workpoint->filedir[i]->name)
        {
            workpoint = workpoint->filedir[i];
            return ;
        }
    }
    cout<<endl<< "cannot find the folder "<<inputdir<<endl<<endl;
}

void command_copy(string file_name)                 //�����ļ�����
{
    if(file_name == "")
    {
        cout<<endl
            <<"      Syntax error! Correct syntax : copy [filename]"<<endl<<endl;

        return ;
    }
//    workpoint=rootdir ;
    ofstream out("FileSystem\\default.htd" , ios::app);
    for(int i=0 ; i<workpoint->document_length ; i++)
    {
        if(workpoint->document[i] == file_name)
        {
            string str="\n--";
            str=str + workpoint->document[i];
            out<<str;

            cout<<endl<<endl<<"       copy succssfully!"<<endl<<endl;
        }
    }
    out.close();
//    delete rootdir ;                //�˴���Ϊ�����Ƿ���д���ļ�
//    rootdir = new node_filesystem() ;
//    filesystem_read("default.htd");
}

void command_hangup()
{
    delete rootdir ;
    rootdir = new node_filesystem() ;
    workpoint = rootdir ;
    filesystem_read("default.htd");
}

void command_del(string ip)
{

}

long long calculatecommand(string inputcommand)
{
	long long result = 0 ;
	long long power = 1;
	for(int i = inputcommand.length() -1 ; i >=0  ; i--)
	{
		result +=  power * (inputcommand[i] - 'a' + 1 );
		power *= 26;
	}
	return result;
}

int command(string inputcommand)            //����ϵͳ
{
	istringstream inputcommand_stream(inputcommand);

	while(inputcommand_stream>>inputcommand)
	{
		int temp = calculatecommand(inputcommand);

		switch (temp)
		{
		case 104358:			//exit   ��ֵ210�����Լ�����
			{
				exit(0);
				break;
			}
        case 1585264:           //clear ����
            {
                system("cls");
                return 0;
            }
        case 82:            //cd
            {
                string inputdir;
                inputcommand_stream>>inputdir;
                command_cd(inputdir);
                return 0;
            }
        case 331:            //ls
            {
                command_ls();
                return 0;
            }
        case 2956:           //dir
            {
                return 0;
            }
        case 144316:           //help
            {
                return 0;
            }
        case 240132926:
            {
                string ip , port;
                inputcommand_stream>>ip>>port;
                command_telnet(ip,port);
                return 0;
            }
        case 229410:
            {
                 mail();
                 return 0;
            }
        case 63309:
            {
                string file_name;
                inputcommand_stream>>file_name;
                command_copy(file_name);
                return 0;
            }
        case 95759342:              //hangup
            {
                command_hangup();
                return 0;
            }
        case 2846:
            {
                string ip;
                inputcommand_stream>>ip;
                command_del(ip);
                return 0;
            }
		default :
			{
			    return 1;
			}
		}
	}
}

void work()
{
    delete rootdir;
    rootdir = new node_filesystem();
    filesystem_read("TheFirstStage.htd");
    while(1)
    {
        string inputcommand;
        cout<<"[Target@"<<workpoint->name<<"]#";
        getline(cin,inputcommand);
        if(command(inputcommand))
        {
            cout<<endl<<"   Unknown  command"<<endl<<endl;
        }
    }
}

void login()
{

    string user , password;
    cout<<"user:";
    cin>>user;
    cout<<"password:";
    cin>>password;
    if(user == target_user && target_password==password)
    {
        getchar();
        system("cls");
        work();
    }
}



void welcomeinformation()                  //��ӭ��Ϣ
{
    cout<<"Hack The Dlnu -Console Command"<<endl
        <<"         -version 0.1"<<endl
        <<"         By  Vic_"<<endl                                //�Լ������ֲ���
        <<endl
        <<"---------------------------------------"<<endl
        <<"Input \"help\" get help;"<<endl
        <<endl;
}


int main ()
{
    welcomeinformation();
    // filesystem_read
    filesystem_read("default.htd");
    string str;
	while(1)
	{
		string inputcommand = "";
		cout<<"[Vic@"<<workpoint->name<<"]#";
		getline(cin,inputcommand);

		if(command(inputcommand))
        {
            cout<<endl<<"   Unknown  command"<<endl<<endl;
        }
	}
	return 0;
}
