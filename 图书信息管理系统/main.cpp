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
    string name;//�鼮��
    string author;//�鼮����
    string flag;//�鼮״̬������|�ѽ���

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
    return l.getName() < r.getName(); // ���д���������������㷨����
}


vector<BookInfo> bookinfos;
//����鼮��ɾ���鼮�������鼮���黹�鼮����ѯȫ������ѯ�������鼮����
class Controller{
public:
    void showCopy();//��Ȩ
    void menu();//�˵�
    bool login();//��½
    void addBook();//���
    void updateBooks();//�޸�
    void deleteBook();//ɾ��
    void Borrow();//����
    void repay();//�黹
    void viewBooks();//��ѯȫ��
    int searchBooks(string line,int* index,int num);//��׼��ѯ
    void Namcmp(string line);//ģ����ѯ
    void countBooks();//�鼮����
    void rewrite();//��д�����ļ�
    void initDate();
    void showBookInfos(BookInfo&);
};

void Controller::addBook()
{
    showCopy();
    cout<<"\n\n\t���ͼ����Ϣ";

    //�����û������Ӧ��ѧ����Ϣ
    string name,author,flag;

    cout<<"\n������ͼ�����ƣ�";
    cin>>name;

    cout<<"\n������ͼ����������";
    cin>>author;

    BookInfo book = BookInfo(name,author,"����");
    cout<<"\n\n��Ҫ��ӵ�ͼ����Ϣ����,��ȷ�ϣ�";

    int result = MessageBox(NULL,"��ȷ��Ҫ��Ӵ�������","ȷ����ʾ",MB_YESNO | MB_ICONQUESTION);
    if(result == 6)//˵��ȷ��Ҫ��ӣ���ô����������ӵ������ļ�
    {
        //fstream fstream fstream
        if(book.save())
        {
            bookinfos.push_back(book);
            cout<<"\n\nͼ����Ϣ��ӳɹ�"<<endl;
        }
          else
        {
            cout<<"\n\nͼ����Ϣ��ӳɹ�"<<endl;
        }
    }
    cout<<"\n\n";
    system("pause");
}

void Controller::showCopy(){
    system("cls");
    cout<<"[*] ͼ����Ϣ����ϵͳ"<<endl;
    cout<<"[*] ����: ���С��\n"<<endl;
}

bool Controller::login(){
    int count=3;
    while(count--){
        showCopy();
        cout<<"\n**** ��½���� ****"<<endl;
        string name,pwd;
        char str[80],ch;
        cout<<"�û���: ";
        getline(cin,name);
        cout<<"���� :";
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
        sprintf(str,"�û������������\n�����%d��,ʣ�����%d!\n,��ȷ�Ϻ���������!",(3-count),count);
        MessageBox(NULL,str,"������ʾ",MB_OK|MB_ICONERROR);
    }
    return false;
}

void Controller::showBookInfos(BookInfo&bf){
    cout<<"����\t����\t״̬"<<endl;
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
    cout<<"\n\t���Ľ���"<<endl;
    if(bookinfos.empty()){
        cout<<"�����鼮��Ϣ"<<endl;
        return;
    }
    string name;
    cout<<"����������Ҫ���ĵ��鼮������������: ";
    cin>>name;
    int* index=new int[bookinfos.size()];
    int num=searchBooks(name,index,0);
    if(!num){
        cout<<"�޴��鼮"<<endl;
    }else{
        cout<<"��Ҫ���ĵ��鼮Ϊ"<<endl;
        for(int i=0;i<num;i++){
            showBookInfos(bookinfos[index[i]]);
            if(bookinfos[index[i]].getFlag()=="���")
            {
                cout<<"���鼮�ѽ��"<<endl;
            }else{

                int result = MessageBox(NULL,"ȷ�Ͻ��Ĵ���?","��ʾ",MB_YESNO|MB_ICONQUESTION);
                if(result==6){
                    bookinfos[index[i]].setFlag("���");
                    cout<<"����ɹ�"<<endl;
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
    cout<<"\n\t�黹����"<<endl;
    string name;
    cout<<"����������Ҫ�黹���鼮������������: ";
    cin>>name;
    int* index=new int[bookinfos.size()];
    int num=searchBooks(name,index,0);
    if(num==0){
        cout<<"�޴��鼮��Ϣ"<<endl;
    }else{
        cout<<"��Ҫ���ĵ��鼮Ϊ"<<endl;
        for(int i=0;i<num;i++){
            showBookInfos(bookinfos[index[i]]);
            if(bookinfos[index[i]].getFlag()=="����")
            {
                cout<<"���鼮δ���"<<endl;
            }else{

                int result = MessageBox(NULL,"ȷ�Ϲ黹����?","��ʾ",MB_YESNO|MB_ICONQUESTION);
                if(result==6){
                    bookinfos[index[i]].setFlag("����");
                    cout<<"����ɹ�"<<endl;
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
    cout << "\t ɾ���鼮��Ϣ"<<endl;

    if(bookinfos.empty())
    {
        cout << "����ͼ������...\n\n"<<endl;
        system("pause");
        return;
    }

    string bname;
    cout << "��������Ҫɾ�����鼮��: ";
    cin >> bname;

    int i = 0, len = bookinfos.size();
    for(i = 0; i < len; i++)
    {
        if(bookinfos[i].getName() == bname)
        {
            cout << "��Ҫɾ�����鼮��Ϣ���£�\n";
            showBookInfos(bookinfos[i]);
            break;
        }
    }
    if( i >= len)
    {
        cout << "������Ҫɾ�����鼮����Ϣ\n\n";
        system("pause");
        return;
    }

    int result = MessageBox(NULL, "����һ����ɾ�������ָܻ�����ȷ��Ҫɾ����", "ɾ��ȷ��", MB_YESNO | MB_ICONQUESTION);
    if(result == 6)
    {
        bookinfos.erase(bookinfos.begin() + i);//ɾ�������ļ�
    }
    cout << "����ɾ���ɹ�...";
    system("pause");
}

void Controller::menu(){
    showCopy();
    cout<<"\t*******�˵�*******"<<endl;
    cout<<"1.ͼ���ѯ"<<endl;
    cout<<"2.ͼ�������黹"<<endl;
    cout<<"3.ͼ�����"<<endl;
    cout<<"4.ͼ���޸�"<<endl;
    cout<<"5.ͼ��ɾ��"<<endl;
    cout<<"6.ͼ��ͳ��"<<endl;
    cout<<"7.�˳�ϵͳ"<<endl;
    int option;
    cout<<"��ѡ��:";
    cin>>option;
    switch(option)
    {
    case 1:
        showCopy();
        cout<<"\n\t**********ͼ���ѯ*********"<<endl;
        cout<<"1.��׼��ѯ"<<endl;
        cout<<"2.ģ����ѯ"<<endl;
        cout<<"3.�˳�"<<endl;
        cout<<"��ѡ��:";
        cin>>option;
        if(option==1||option==2){
            string name;
            cout<<"�����ѯ����������������:";
            cin>>name;
            if(option==1){
                int* index=new int[bookinfos.size()];
                int num=searchBooks(name,index,0);
                if(num){
                    for(int i=0;i<num;i++)
                        showBookInfos(bookinfos[index[i]]);
                }else{
                    cout<<"���޴��鼮��Ϣ"<<endl;
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
        cout<<"\n\t**********ͼ�������黹*********"<<endl;
        cout<<"1.ͼ�����"<<endl;
        cout<<"2.ͼ��黹"<<endl;
        cout<<"3.�˳�"<<endl;
        cout<<"��ѡ��:";
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
        cout<<"��л����ʹ��!"<<endl;
        exit(0);
        break;
    }
}

void Controller::Namcmp(string name){
    int count=0;
    vector<string> t;
    for(int i=0,len=name.length(); i<len; i++)
	{
		if(name[i]<255 && name[i]>0)//�����ASCII�ַ���ΧΪ0-255,����,����һ���ֽ�
		{
			t.push_back(name.substr(i,1));
		}
		else//<0,>255���Ǻ���,���������ֽ�
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
    cout<<"�˴�������ѯ��"<<count<<"������"<<endl;
}

void Controller::rewrite()
{
    int len = bookinfos.size();
    if(len <= 0)
    {
        // ˵���������Ѿ�û�������ˣ����������ݱ���ɾ����
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
    cout<<"\t�޸�ͼ����Ϣ\n";
    if(bookinfos.empty())
    {
        cout<<"\n\n\t��������...\n\n\t";
        system("pause");
        return;
    }
    string name;
    cout<<"\n�������鼮��";
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
        cout<<"\n������Ҫ�޸ĵ�ͼ����Ϣ...\n\n\t";
        system("pause");
        return;
    }
    cout<<"\n��Ҫ�޸ĵ���Ϣ���£�"<<endl;
    showBookInfos(bookinfos[index]);

    string author;
    cout<<"\n�������޸ĺ�ͼ�����ƺ��������ƣ�";
    cin>>name>>author;

    int result = MessageBox(NULL,"��ȷ��Ҫ��Ӵ�������","ȷ����ʾ",MB_YESNO | MB_ICONQUESTION);
    if(result == 6)
    {
       bookinfos[index].setName(name),bookinfos[index].setAuthor(author);
    }

    cout<<"\n\n�����޸ĳɹ�...\n\n";
    system("pause");
}

void Controller::viewBooks()
{
    showCopy();
    cout << "\t��ѯȫ���鼮��Ϣ\n"<<endl;;
    cout << "Ĭ�ϰ��������ֵ�����\n\n"<<endl;
    sort(bookinfos.begin(),bookinfos.end(),compare);
    if(bookinfos.size() <= 0)
    {
        cout << "�����鼮��Ϣ...\n\n"<<endl;;
        system("pause");
        return;
    }

    cout << "����\t����\t״̬\n" << endl;
    for(int i = 0, len = bookinfos.size(); i < len; i++)
    {
        bookinfos[i].show();
    }
    cout << "\n\n�ܹ���" << bookinfos.size() << "���鼮��Ϣ" << "\n\n";
    system("pause");
}

int main()
{
    //�任��ɫΪ��ɫ
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
