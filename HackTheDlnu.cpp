#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
using namespace std;

struct node_filesystem                      //文件系统  结点
{
    string name;                            //此结点名字

    string document[1000];                  //文件结点，用数组保存文件名
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

node_filesystem *rootdir = new node_filesystem();                    //根目录
string target_ip;
string target_port;
string target_user;
string target_password;


node_filesystem *workpoint ;                //工作指针

int filesystem_read(string filename)        //文件系统
{
    rootdir->pri_dir = NULL;
    rootdir->name = "/";
    workpoint = rootdir;
    filename = "FileSystem\\" + filename;

    ifstream infile(filename.c_str());

	if(!infile)
	{
		cerr << "初始文件读取出错" <<filename<<"\n";
		return 1;
	}

	string fileinput;
	string fileflag = "-";
    while(infile>>fileinput)
	{
	   // cout<<fileflag<<":"<<fileflag.length()<<endl;
	    int flag = 0;
	    for(int i = fileflag.length() - 1; i >= 0  ; i --)       //文件判断标志，判断当前录入的是文件还是文件夹
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

void mail()                       //读取mail
{
    string email = "Mail_01.htd";
    email="MissionPack\\"+email;
    ifstream infile(email.c_str());         //读取邮件系统
    string input_file;
    system("cls");
    while(getline(infile , input_file))               //读取邮件以获得文件中的IP地址
    {
        cout<<input_file<<endl;
        for(int i=0;i<input_file.length();i++)
        {
            if(input_file[i]=='I' && input_file[i+1]=='P')          //读取IP
            {
                target_ip=input_file.substr(3 , input_file.length());
            }
            if(input_file[i]=='p' && input_file[i+1]=='o' && input_file[i+2]=='r' &&input_file[i+3]=='t')   //读取端口
            {
                target_port=input_file.substr(5 , input_file.length());
            }
             if(input_file[i]=='u' && input_file[i+1]=='s' && input_file[i+2]=='e' &&           //读取用户名
                input_file[i+3]=='r')
            {
                target_user=input_file.substr(5 , input_file.length());
            }
           if(input_file[i]=='p' && input_file[i+1]=='a' && input_file[i+2]=='s' &&         //读取密码
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

void command_copy(string file_name)                 //拷贝文件函数
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
//    delete rootdir ;                //此处仅为测试是否能写入文件
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

int command(string inputcommand)            //命令系统
{
	istringstream inputcommand_stream(inputcommand);

	while(inputcommand_stream>>inputcommand)
	{
		int temp = calculatecommand(inputcommand);

		switch (temp)
		{
		case 104358:			//exit   数值210来自自己计算
			{
				exit(0);
				break;
			}
        case 1585264:           //clear 清屏
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



void welcomeinformation()                  //欢迎信息
{
    cout<<"Hack The Dlnu -Console Command"<<endl
        <<"         -version 0.1"<<endl
        <<"         By  Vic_"<<endl                                //自己把名字补上
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
