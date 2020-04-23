#include <iostream>
#include <fstream>
#include <iostream>
#include<bits/stdc++.h>

using namespace std;

const char* readFile(const char* fileName);
int strnum=0;  //字符串个数 


int main()
{
	ofstream data2;
	data2.open("output2.txt",ios::out); 
	const char* result = readFile("input2.txt");
    cout <<"传入内容："<<result;
	string s="";

	if(strnum==1)
	{
		cout<<endl<<"---字符串转化为结构体---"<<endl; 
		for(int i=0;i<strlen(result);i++)
       {
		   s=s+result[i];
       }
		int k=0,i=0;
        while(s.length()!=0)
       {
    	   k=s.find("\\r\\n");
    	   if(k>=0)
    	   {
    		   string str1=s.substr(i,k-i);
    		   cout<<str1<<endl;
    		   data2<<str1<<endl;
    		   int k2=k+4;
    		   s=s.substr(k2,s.length()-k2);
		   }
		   else
		   {
			   cout<<s<<endl;
			   data2<<s<<endl;
			   break;
	       }
	    }	
	} 
	else
	{
		cout<<endl<<"---结构体转化为字符串---"<<endl; 
		for(int i=0;i<strlen(result);i++)
		{
			if(result[i]=='\n')
			{
				s=s+"\\r\\n";
			}
			else s=s+result[i];
		}
		cout<<s<<endl;
		data2<<s<<endl;
	}
	data2.close();
	return 0; 
}
 
 
const char* readFile(const char* fileName) {
	ifstream file(fileName);                            // 打开文件
	char* content;
	content = (char*)malloc(sizeof(char) * 40960);           // 假设文件最大为40K，可自定义
	if (content)
	{
		char buff[1024];                                     // 1Kb的缓冲区
		int pt = 0;
		strnum=0;
		while (file.getline(buff, 1024))                     // 按行读取文件到缓冲区
		{
			for (int i = 0; i < 1024; i++) {
				char tmp = buff[i];
				if (tmp == '\0') {                           // 遇到\0时换行，继续读取下一行
					content[pt] = '\n';
					pt++;
					strnum++;
					break;
				}
				content[pt] = tmp;
				pt++;
			}
		}
		content[pt] = '\0';                                  // 读取结束，构建字符串尾
		char* result = (char*)malloc(sizeof(char) * (++pt)); // 申请pt+1个字节的内存空间
		if (!result)
			return NULL;
		for (int i = 0; i < pt; i++) {
			result[i] = content[i];   
			//std::cout<<result[i]<<'\n';                       // 字符串复制
		}
		cout<<strnum<<'\n';
		return result;
	}
	return NULL;
}
