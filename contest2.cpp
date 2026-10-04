#include<iostream>

#include<fstream>
#include<string> 

#include<vector>
#include<regex>

#include<stack>

#include<cstdlib>

using namespace std;
string st;
int countLines(const std::string&filename){
	ifstream file(filename);
	string line;
	int count=0;
	while (std::getline(file,line)){
		count++;
	}
	return count;
} 
//代码行数统计 

vector<int> 
checkMissingSemicolon(const string& filename) {
    ifstream file(filename);
    string line;
    vector<int> errorLines;
    int lineNum = 0;
    
    while (getline(file, line)) {
        lineNum++;
        string trim_line=line;
        // 跳过空行和注释
        
        size_t start=trim_line.find_first_not_of(" \t");
        if(start==string::npos) continue;//直接跳过空行
		trim_line=trim_line.substr(start);
		 
		if(trim_line[0]=='#'){
			continue;
		}
		if(trim_line.substr(0,2)=="//"){
			continue;
		}
		char last_char=trim_line.back();
		if(last_char==';' || last_char=='{' || last_char=='}'){
			continue;
		}
        errorLines.push_back(lineNum);
    }
    return errorLines;
}

//分号等缺失检查

vector<int> 
checkBraceMatch(const string& filename){
	ifstream file(filename);
	string line;
	stack<int> stk;
	vector<int> errLines;
	int lineNum=0;
	
	while(getline(file,line)){
		lineNum++;
		for(char c:line){
			if(c=='{'){
				stk.push(lineNum);
			}
			else if(c=='}'){
				if(stk.empty()){
					errLines.push_back(lineNum);
				}
				else{
					stk.pop();
				}
			}
		}
	}//循环读完文件，栈不为空 
	while(!stk.empty()){
		errLines.push_back(stk.top());
		stk.pop(); 
	}
	return errLines;
}

//内存版 
vector<int> checkBraceMatchMemory(const vector<string>& codeLines) {
	stack<int> stk; 
    vector<int> errLines;
    int lineNum =0;
    for(const string& line : codeLines)
    {
        lineNum ++;
        for(char c:line){
        	if(c=='{'){
        		stk.push(lineNum);
			}
			else if(c=='}'){
				if(stk.empty()){
					errLines.push_back(lineNum);
				}
				else{
					stk.pop();
				}
			}
		}
    }
	while(!stk.empty()){
		errLines.push_back(stk.top());
		stk.pop();
	}
		return errLines; 
}
vector<int> checkMissingSemicolonMemory(const vector<string>& codeLines){
	vector<int> errorLines;
	int lineNum=0;
	for(const string& line : codeLines){
		lineNum++;
		string trim_line=line;
		
		size_t start=trim_line.find_first_not_of(" \t");
		if(start==string::npos) continue;
		trim_line=trim_line.substr(start);
		
		if(trim_line[0]=='#'){
			continue;
		}
		if(trim_line.size()>=2 && trim_line.substr(0,2)=="//"){
			continue;
		}
		
		size_t endpos=trim_line.find_last_not_of(" \t");
		trim_line=trim_line.substr(0,endpos+1);
		
		char last_char=trim_line.back();
		if(last_char==';' || last_char=='{' || last_char=='}'){
			continue;
		}
		errorLines.push_back(lineNum);
	}
	return errorLines;
}


int main(){
	cout<<"====程序智能诊断工具===="<<endl;
	cout<<"1=读取本地test.cpp文件检查"<<endl;
	cout<<"2=手动输入代码片段检查"<<endl;
	int op;
	cin>>op;
	cin.ignore();
	
	if(op==1){//读取本地文件 
		int total=countLines("test.cpp");
		cout<<"文件总行数："<<total<<endl;
		
		vector<int> err_semi=checkMissingSemicolon("test.cpp");
		vector<int> err_brace=checkBraceMatch("test.cpp");
		
		if(err_semi.empty()){
			cout<<"未检测到缺少分号"<<endl;
			 
		} 
		else{
			cout<<"检测到缺少分号：";
			for(auto num:err_semi){
				cout<<num<<" ";
			} 
			cout<<endl;
		}
		if(err_brace.empty()){
			cout<<"未检测到大括号配对错误"<<endl; 
		}
		else{
			cout<<"大括号配对错误:";
			for(auto num:err_brace){
				cout<<num<<" ";
			} 
			cout<<endl;
		}
	}
	else if(op==2){//手动输入 
		cout<<"请输入你的代码，#end 结束输入！"<<endl;
		vector<string> code_buf;
		string temp;
		while(getline(cin,temp)){
			if(temp.find("#end") !=string::npos) break;
			code_buf.push_back(temp); 
		} 
		
		/*ofstream out("temp_code.cpp");//将输入的代码临时写到 temp_code.cpp
		for(auto &s:code_buf){
			out<<s<<endl;
		}
		out.close();
		//调用检查函数读取临时文件*/   //直接内存计算，不写磁盘文件
		 
		int total=code_buf.size(); 
		cout<<"输入代码总行数："<<total<<endl; 
		
		vector<int> err_semi=checkMissingSemicolonMemory(code_buf);
		vector<int> err_brace=checkBraceMatchMemory(code_buf);
		
		if(err_semi.empty()){
			cout<<"未检测到缺少分号"<<endl;
			 
		} 
		else{
			cout<<"缺少分号:";
			for(auto num:err_semi){
				cout<<num<<" ";
			}
			cout<<endl;
		}
		if(err_brace.empty()){
			cout<<"未检测到大括号配对错误"<<endl;
		}
		else{
			cout<<"大括号配对错误：";
			for(auto num:err_brace){
				cout<<num<<" ";
			} 
			cout<<"\n提示：该符号开启的代码块末尾需要补上右大括号，具体补在哪一行请查看AI分析结果\n";
		}
	}
	cout<<"\n请按任意键继续...";
	system("pause");
	 return 0;
} 
