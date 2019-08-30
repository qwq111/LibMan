#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <fstream>

using namespace std;


const string user="sbxz";
const string passwd="123";

#include<iostream>
#define BOOKSFILE "books.txt"
using namespace std;
class BookInfo{
private:
    string name;//书籍名
    string author;//书籍作者
    string flag;//书籍状态，可用|已借阅

public:
    BookInfo(string name,string author,string flag);
    string getName();
    void setName(string name);
    string getAuthor();
    void setAuthor(string author);
    string getFlag();
    void setFlag(string flag);

    void show();
    bool save();
};

BookInfo::BookInfo(string name,string author,string flag){
        this->setFlag(flag);
        this->setName(name);
        this->setAuthor(author);
}

string BookInfo::getName(){
        return name;
}
void BookInfo::setName(string name){
    this->name=name;
}
string BookInfo::getAuthor(){
    return author;
}
void BookInfo::setAuthor(string author){
    this->author=author;
}
string BookInfo::getFlag(){
    return flag;
}
void BookInfo::setFlag(string flag){
    this->flag=flag;
}
void BookInfo::show(){
    cout<<name<<"\t"<<author<<"\t"<<flag<<endl;
}
bool BookInfo::save(){
    ofstream out(BOOKSFILE,ios :: out|ios :: app| ios :: binary);
    if(out.is_open()){
        out<<name<<"\t"<<author<<"\t"<<flag<<"\r\n";
        out.close();
        return true;
    }
    return false;
}


bool compare(BookInfo l, BookInfo r)
{
    return l.getName() < r.getName(); // 如果写死返回真则排序算法报错
}


vector<BookInfo> bookinfos;
//添加书籍。删除书籍。借阅书籍。归还书籍。查询全部，查询书名。书籍计数
class Controller{
public:
    void showCopy();//版权
    void menu();//菜单
    bool login();//登陆
    void addBook();//添加
    void updateBooks();//修改
    void deleteBook();//删除
    void Borrow();//借阅
    void repay();//归还
    void viewBooks();//查询全部
    int searchBooks(string line,int* index,int num);//精准查询
    void Namcmp(string line);//模糊查询
    void countBooks();//书籍计数
    void rewrite();//重写数据文件
    void initDate();
    void showBookInfos(BookInfo&);
};

void Controller::addBook()
{
    showCopy();
    cout<<"\n\n\t添加图书信息";

    //提醒用户输入对应的学生信息
    string name,author,flag;

    cout<<"\n请输入图书名称：";
    cin>>name;

    cout<<"\n请输入图书作者名：";
    cin>>author;

    BookInfo book = BookInfo(name,author,"可用");
    cout<<"\n\n您要添加的图书信息如下,请确认：";

    int result = MessageBox(NULL,"您确定要添加此数据吗？","确认提示",MB_YESNO | MB_ICONQUESTION);
    if(result == 6)//说明确定要添加，那么将此数据添加到数据文件
    {
        //fstream fstream fstream
        if(book.save())
        {
            bookinfos.push_back(book);
            cout<<"\n\n图书信息添加成功"<<endl;
        }
          else
        {
            cout<<"\n\n图书信息添加成功"<<endl;
        }
    }
    cout<<"\n\n";
    system("pause");
}

void Controller::showCopy(){
    system("cls");
    cout<<"[*] 图书信息管理系统"<<endl;
    cout<<"[*] 作者: 随便小组\n"<<endl;
}

bool Controller::login(){
    int count=3;
    while(count--){
        showCopy();
        cout<<"\n**** 登陆界面 ****"<<endl;
        string name,pwd;
        char str[80],ch;
        cout<<"用户名: ";
        getline(cin,name);
        cout<<"密码 :";
        int num=0;
        while((ch=getch())!='\r'){
            if(ch=='\b')
            {
                num--;
                cout<<"\b \b";
            }
            else{
                cout<<"*";
                str[num++]=ch;
            }
        }
        str[num]='\0';
        pwd=str;

        if(name==user&&pwd==passwd){
            return true;
        }
        sprintf(str,"用户名或密码错误\n已输错%d次,剩余次数%d!\n,请确认后重新输入!",(3-count),count);
        MessageBox(NULL,str,"错误提示",MB_OK|MB_ICONERROR);
    }
    return false;
}

void Controller::showBookInfos(BookInfo&bf){
    cout<<"书名\t作者\t状态"<<endl;
    bf.show();
}

void Controller::initDate(){
    cout<<endl;
    fstream read(BOOKSFILE,ios::in|ios::binary);
    if(!read.is_open()){
        return;
    }
    string line;
    int index,position;
    string arr[3];
    int num;
    while(getline(read,line))
    {
        position=0;num=0;
        index=line.find("\t",position);
        while(index>0){
            arr[num++]=line.substr(position,index-position);
            position=index+1;
            index=line.find("\t",position);
        }
        arr[num]=line.substr(position,line.find("\r\n",position)-position);
        bookinfos.push_back(BookInfo(arr[0],arr[1],arr[2]));
    }
    read.close();
}

void Controller::Borrow(){
    showCopy();
    cout<<"\n\t借阅界面"<<endl;
    if(bookinfos.empty()){
        cout<<"暂无书籍信息"<<endl;
        return;
    }
    string name;
    cout<<"请输入你需要借阅的书籍名或者作者名: ";
    cin>>name;
    int* index=new int[bookinfos.size()];
    int num=searchBooks(name,index,0);
    if(!num){
        cout<<"无此书籍"<<endl;
    }else{
        cout<<"您要借阅的书籍为"<<endl;
        for(int i=0;i<num;i++){
            showBookInfos(bookinfos[index[i]]);
            if(bookinfos[index[i]].getFlag()=="借出")
            {
                cout<<"该书籍已借出"<<endl;
            }else{

                int result = MessageBox(NULL,"确认借阅此书?","提示",MB_YESNO|MB_ICONQUESTION);
                if(result==6){
                    bookinfos[index[i]].setFlag("借出");
                    cout<<"借书成功"<<endl;
                }
            }
        }
    }
    system("pause");
    delete[] index;
    return;
}

void Controller::repay(){
    showCopy();
    cout<<"\n\t归还界面"<<endl;
    string name;
    cout<<"请输入你需要归还的书籍名或者作者名: ";
    cin>>name;
    int* index=new int[bookinfos.size()];
    int num=searchBooks(name,index,0);
    if(num==0){
        cout<<"无此书籍信息"<<endl;
    }else{
        cout<<"您要借阅的书籍为"<<endl;
        for(int i=0;i<num;i++){
            showBookInfos(bookinfos[index[i]]);
            if(bookinfos[index[i]].getFlag()=="可用")
            {
                cout<<"该书籍未借出"<<endl;
            }else{

                int result = MessageBox(NULL,"确认归还此书?","提示",MB_YESNO|MB_ICONQUESTION);
                if(result==6){
                    bookinfos[index[i]].setFlag("可用");
                    cout<<"换书成功"<<endl;
                }
            }
        }
    }
    system("pause");
    return;
}

int Controller::searchBooks(string line,int *index,int num){

    for(int i=0,len=bookinfos.size();i<len;i++){
        if(line==bookinfos[i].getName()||line==bookinfos[i].getAuthor()){
            index[num++]=i;

        }
    }
    return num;
}

void Controller::deleteBook()
{
    showCopy();
    cout << "\t 删除书籍信息"<<endl;

    if(bookinfos.empty())
    {
        cout << "暂无图书数据...\n\n"<<endl;
        system("pause");
        return;
    }

    string bname;
    cout << "请输入您要删除的书籍名: ";
    cin >> bname;

    int i = 0, len = bookinfos.size();
    for(i = 0; i < len; i++)
    {
        if(bookinfos[i].getName() == bname)
        {
            cout << "您要删除的书籍信息如下：\n";
            showBookInfos(bookinfos[i]);
            break;
        }
    }
    if( i >= len)
    {
        cout << "暂无您要删除的书籍的信息\n\n";
        system("pause");
        return;
    }

    int result = MessageBox(NULL, "数据一旦被删除将不能恢复，您确定要删除吗？", "删除确认", MB_YESNO | MB_ICONQUESTION);
    if(result == 6)
    {
        bookinfos.erase(bookinfos.begin() + i);//删除数据文件
    }
    cout << "数据删除成功...";
    system("pause");
}

void Controller::menu(){
    showCopy();
    cout<<"\t*******菜单*******"<<endl;
    cout<<"1.图书查询"<<endl;
    cout<<"2.图书借阅与归还"<<endl;
    cout<<"3.图书添加"<<endl;
    cout<<"4.图书修改"<<endl;
    cout<<"5.图书删除"<<endl;
    cout<<"6.图书统计"<<endl;
    cout<<"7.退出系统"<<endl;
    int option;
    cout<<"请选择:";
    cin>>option;
    switch(option)
    {
    case 1:
        showCopy();
        cout<<"\n\t**********图书查询*********"<<endl;
        cout<<"1.精准查询"<<endl;
        cout<<"2.模糊查询"<<endl;
        cout<<"3.退出"<<endl;
        cout<<"请选择:";
        cin>>option;
        if(option==1||option==2){
            string name;
            cout<<"输入查询的书名或者作者名:";
            cin>>name;
            if(option==1){
                int* index=new int[bookinfos.size()];
                int num=searchBooks(name,index,0);
                if(num){
                    for(int i=0;i<num;i++)
                        showBookInfos(bookinfos[index[i]]);
                }else{
                    cout<<"查无此书籍信息"<<endl;
                }
            }else{
                Namcmp(name);
            }
        }else if(option==3){
            break;
        }
        system("pause");
        break;
    case 2:
        showCopy();
        cout<<"\n\t**********图书借阅与归还*********"<<endl;
        cout<<"1.图书借阅"<<endl;
        cout<<"2.图书归还"<<endl;
        cout<<"3.退出"<<endl;
        cout<<"请选择:";
        cin>>option;
        if(option==1){
            Borrow();
        }else if(option==3){
            break;
        }else if(option==2){
            repay();
        }
        break;
    case 3:
        addBook();
        break;
    case 4:
        updateBooks();
        break;
    case 5:
        deleteBook();
        break;
    case 6:
        viewBooks();
        break;
    case 7:
        rewrite();
        cout<<"感谢您的使用!"<<endl;
        exit(0);
        break;
    }
}

void Controller::Namcmp(string name){
    int count=0;
    vector<string> t;
    for(int i=0,len=name.length(); i<len; i++)
	{
		if(name[i]<255 && name[i]>0)//扩充的ASCII字符范围为0-255,如是,处理一个字节
		{
			t.push_back(name.substr(i,1));
		}
		else//<0,>255的是汉字,处理两个字节
		{
			t.push_back(name.substr(i,2));
			++i;
		}
	}
    for(int i=0,len=bookinfos.size();i<len;i++){
        for(int j=0,lenj=t.size();j<lenj;j++){
            if(bookinfos[i].getName().find(t[j],0)!=-1||bookinfos[i].getAuthor().find(t[j],0)!=-1){
                showBookInfos(bookinfos[i]);count++;
                break;
            }
        }
    }
    cout<<"此次搜索查询到"<<count<<"条数据"<<endl;
}

void Controller::rewrite()
{
    int len = bookinfos.size();
    if(len <= 0)
    {
        // 说明集合中已经没有数据了，即所有数据被你删除了
        ofstream out(BOOKSFILE, ios::out | ios::trunc | ios::binary);
        out.close();
        return;
    }

    ofstream out(BOOKSFILE, ios::out | ios::binary);
    if(out.is_open())
    {
        for(int i = 0; i < len; i++)
        {
            out << bookinfos[i].getName() << "\t"
            << bookinfos[i].getAuthor() << "\t"
            << bookinfos[i].getFlag() << "\r\n";
        }
        out.close();
    }
}
void Controller::updateBooks()
{
    showCopy();
    cout<<"\t修改图书信息\n";
    if(bookinfos.empty())
    {
        cout<<"\n\n\t暂无数据...\n\n\t";
        system("pause");
        return;
    }
    string name;
    cout<<"\n请输入书籍名";
    cin >> name;
    int index=-1;
    for(int i=0,len=bookinfos.size();i<len;i++){
        if(name==bookinfos[i].getName()){
            index=i;
            break;
        }
    }
    if(index==-1)
    {
        cout<<"\n暂无您要修改的图书信息...\n\n\t";
        system("pause");
        return;
    }
    cout<<"\n您要修改的信息如下："<<endl;
    showBookInfos(bookinfos[index]);

    string author;
    cout<<"\n请输入修改后图书名称和作者名称：";
    cin>>name>>author;

    int result = MessageBox(NULL,"您确定要添加此数据吗？","确认提示",MB_YESNO | MB_ICONQUESTION);
    if(result == 6)
    {
       bookinfos[index].setName(name),bookinfos[index].setAuthor(author);
    }

    cout<<"\n\n数据修改成功...\n\n";
    system("pause");
}

void Controller::viewBooks()
{
    showCopy();
    cout << "\t查询全部书籍信息\n"<<endl;;
    cout << "默认按照书名字典排序\n\n"<<endl;
    sort(bookinfos.begin(),bookinfos.end(),compare);
    if(bookinfos.size() <= 0)
    {
        cout << "暂无书籍信息...\n\n"<<endl;;
        system("pause");
        return;
    }

    cout << "书名\t作者\t状态\n" << endl;
    for(int i = 0, len = bookinfos.size(); i < len; i++)
    {
        bookinfos[i].show();
    }
    cout << "\n\n总共有" << bookinfos.size() << "条书籍信息" << "\n\n";
    system("pause");
}

int main()
{
    //变换颜色为蓝色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    Controller controller;
    if(controller.login()){
        controller.initDate();
        while(1){
            controller.menu();
        }
    }
    return 0;
}
