#include "httpParse.hpp"

using namespace std;

const char* readFile(const char* fileName);
// string s="";
// map<string, string> post;  // ����һ��map����

void apart(const char* result, map<string, string> post,string s)  //��� 
{
	//cout<<endl<<"---�ַ���ת��Ϊ�ṹ��---"<<endl; 
	string str6;
	
	for(int i=0;i<strlen(result);i++)
	{
		s=s+result[i];
	}
	//cout<<"s="<<s<<endl;
	int k=0,i=0,k3=0;
    while(s.length()!=0)
    {
       	k3=s.find("\r\n\r\n");
       	//cout<<"k3="<<k3<<endl; 
       	if(k3>0)
		{
			str6=s.substr((k3)+4,s.length()-k3);
		//	post["Entity-body"]=str6;
       	//   cout<<"str6:"<<str6<<endl;
		}
       	
		s=s.substr(0,k3);
    	k=s.find("\r\n");
    	//cout<<"k="<<k<<endl; 
    	if(k>=0)
    	{
    		string str1=s.substr(0,k);
    		int id_num=str1.find(':');
    		if(id_num>0)
    		{
    		   	string str2=str1.substr(0,id_num);
    		   	string str3=str1.substr(id_num+1,str1.length()-id_num);
    		   	post[str2]=str3;
			}
			else
			{
			   	int id_num2=str1.find(' ');
			   	string str4=str1.substr(0,id_num2);
			   	string str5=str1.substr(id_num2+1,str1.length()-id_num2);
			   	post["Version"]=str4;
			   	post["State-code"]=str5;
			   	//post["Status-line"]=str1;
			}
    		cout<<str1<<endl;
    		int k2=k+2;
    		s=s.substr(k2,s.length()-k2);
		}
		else
		{
			cout<<s<<endl;
			break;
	    }
	}	
	cout<<endl<<str6<<endl<<endl<<endl;
	for (map<string,string>::iterator it = post.begin(); it != post.end(); it++) 
	{
        cout << it->first << " " << it->second << endl;   
	}
}

void connect(char* filename,char* request,char* header,string s)  //����
{
	const char* result = readFile(filename);
  //  cout <<"�������ݣ�"<<result;
    string s3="";
	for(int i=0;i<strlen(request);i++)
    {
		s3=s3+request[i];  //ת��Ϊ�ַ��� 
    }
    s3=s3+"\\r\\n";
    
    string s2="";
    for(int i=0;i<strlen(header);i++)
    {
		if(header[i]=='\r'&&header[i+1]=='\n')
		{
			s2=s2+"\\r\\n";
			i++;
		}
		else s2=s2+header[i];
    }

	string s1="";
	for(int i=0;i<strlen(result);i++)
	{
		if(result[i]=='\n')
		{
			s1=s1+"\\r\\n";
		}
		else s1=s1+result[i];
	}
	string s4=s3+s2+s1;
	cout<<s4<<endl;
}
 
 
const char* readFile(const char* fileName) {
	ifstream file(fileName);                            // ���ļ�
	char* content;
	content = (char*)malloc(sizeof(char) * 40960);           // �����ļ����Ϊ40K�����Զ���
	if (content)
	{
		char buff[1024];                                     // 1Kb�Ļ�����
		int pt = 0;
		int strnum=0;
		while (file.getline(buff, 1024))                     // ���ж�ȡ�ļ���������
		{
			for (int i = 0; i < 1024; i++) {
				char tmp = buff[i];
				if (tmp == '\0') {                           // ����\0ʱ���У�������ȡ��һ��
					content[pt] = '\n';
					pt++;
					strnum++;
					break;
				}
				content[pt] = tmp;
				pt++;
			}
		}
		content[pt] = '\0';                                  // ��ȡ�����������ַ���β
		char* result = (char*)malloc(sizeof(char) * (++pt)); // ����pt+1���ֽڵ��ڴ�ռ�
		if (!result)
			return NULL;
		for (int i = 0; i < pt; i++) {
			result[i] = content[i];   
			//std::cout<<result[i]<<'\n';                       // �ַ�������
		}
		//cout<<strnum<<'\n';
		return result;
	}
	return NULL;
}

/*
int main()
{
	char* request="GET /index.html HTTP/1.1 ";
	char* header="Content-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\n";
	char* result="HTTP/1.1 200 OK\r\nContent-Type: application\r\nUser-Agent:Runtime/7.1\r\nCache-Control: no-cache\r\nHost: 127.0.0.1:8080\r\n\r\n<title>CS06142</title>";
	connect("input4.txt",(char*) request,(char*) header);
	apart((char*) result);
	//Parse("input4.txt","output4.txt");
	return 0;
}
*/
