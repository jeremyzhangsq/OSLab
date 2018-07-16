//
// Created by Jeremy on 2018/6/7.
//

#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <list>
#include <iterator>
using namespace std;

struct usr{
    string name;
    string pwd;
};
struct file {
    string fname;
    bool opened;
    int fprotect;
    usr *creater;
    usr *opener;
    int pntr;
    string content;
};
struct dir{
    string name;
    dir* pre;
    vector<dir*> dirs;
    vector<file*> files;
};


void manual();

// directory operation
void mkdir(string name);
void dedir(string name);
void create(string name, int mode);
string pwd();
dir* cd(string target);
dir* cdp(string dst);
dir* dcheck(string name);
dir* root();
string ls();

// file operation
file* fcheck(string name);
string search(string name);
void open(string name);
string lsfile();
void write(string name, string buff, int mode);
void reposition(string name, int pos);
void cat(string name);
void truncate(string name, int pos);
void chmod(string name, int level);
void close(string name);
void del(string name);

// user operation
usr* regusr(string name, string pwd, int type);
void delusr(string name);
usr* ucheck(string name);
void disusr();
usr* login(string name);

static dir* path;
static usr* user;
static vector<usr*> users;

// 检查合法性的操作：是否已经存在
dir* dcheck(string name){
    for(dir *each:path->dirs){
        if(each->name == name)
            return each;
    }
    return nullptr;
}
file* fcheck(string name){
    for(file *each:path->files){
        if(each->fname == name)
            return each;
    }
    return nullptr;
}
usr* ucheck(string name){
    for(usr *each:users){
        if(each->name == name)
            return each;
    }
    return nullptr;
}
void mkdir(string name){
    if(dcheck(name) != nullptr)
        cout <<"directory already exists" << endl;
    else{
        dir *adir = new dir;
        adir->name = name;
        adir->pre = path;
        //  update current dir: add subdir
        path->dirs.push_back(adir);
    }
}
void create(string name, int mode){
    if(fcheck(name) != nullptr)
        cout <<"file already exists" << endl;
    else{
        file *adir = new file;
        adir->fname = name;
        adir->opened = false;
        adir->content = "";
        adir->pntr = 0;
        if (mode >= 0 and mode < 8){
            adir->fprotect = mode;
            adir->creater = user;
            path->files.push_back(adir);
        }
        else
            cout<<"Error file mode"<<endl;
        //  update current path: update file
    }
}
void del(string name){
    file* cur = fcheck(name);
    if(cur == nullptr)
        cout <<"file does not exist" << endl;
    else{
        for(auto it = path->files.begin();it!=path->files.end();++it){
            if(*it == cur){
                path->files.erase(it);
                return;
            }
        }
    }
}
void dedir(string name){
    dir* cur = dcheck(name);
    if(cur == nullptr)
        cout <<"directory does not exist" << endl;
    else{
        for(auto it = path->dirs.begin();it!=path->dirs.end();++it){
            if(*it == cur){
                path->dirs.erase(it);
                return;
            }
        }
    }
}
dir* root(){
    dir* p = path;
    while(p->pre!= nullptr)
        p = p->pre;
    return p;
}
dir* cd(string target){
    dir* cur = path;
//    add: cd . and cd ..
    if (target == ".")
        return cur->pre != nullptr ? cur->pre : path;
    else if (target == ".."){
        if (cur->pre!= nullptr)
            return cur->pre->pre != nullptr ? cur->pre->pre : path;
        else
            return path;
    }
    else{
        cur = dcheck(target);
        if(cur == nullptr){
            cout << "directory does not exist" << endl;
            return nullptr;
        }
        else
            return cur;
    }


}
string search(string name){
    string out = "";
    dir* temp = path;
    path = root();
    list<dir*> queue;
    queue.push_back(path);
    while(!queue.empty()){
        path = queue.front();
        queue.pop_front();
        for(dir* each : path->dirs)
            queue.push_back(each);
        for(file* each : path->files){
            if(each->fname == name)
                out+=pwd()+name+"\n";
        }
    }
    path = temp;
    if (out.empty())
        out+="file does not found\n";
    return out;
}
void open(string name){
    file* f = fcheck(name);
    if(f == nullptr)
        cout <<"file does not exist" << endl;
    else{
        if(f->opened)
            cout << "file already open" <<endl;
        else{
            f->opened = true;
            f->opener = user;
        }
    }
}
dir* cdp(string dst) {
    dir* cur;
    dir* temp = path;
    path = root();
    if(dst[dst.length()-1] != '/')
        dst = dst += '/';
    dst = dst.substr(1,dst.length());
    string target;
    int index = dst.find('/');
    while(index>0){
        target = dst.substr(0,index);
        path = cd(target);
        if(path == nullptr){
            path = temp;
            return nullptr;
        }
        dst = dst.substr(index+1,dst.length());
        index = dst.find('/');
    }
    cur = path;
    path = temp;
    return cur;
}
string pwd(){
    dir* p = path;
    vector<string> a;
    while(p->pre!= nullptr){
        a.push_back(p->name);
        p = p->pre;
    }
    string out = "/";
    while(!a.empty()){
        out += a.back()+"/";
        a.pop_back();
    }
    return out;

}
void write(string name, string buff, int mode){
    file* f = fcheck(name);
    if(f == nullptr)
        cout << "file does not exist" << endl;
    else{
        if(!f->opened){
            cout << "file does not open" << endl;
        }
        else{
            int m = f->fprotect;
            if(m!=2 and m!=3 and m!=6 and m!=7){
                cout << "permission denied" << endl;
            }
            else{
                if(mode == 0){
                    f->content += buff;
                    f->pntr = f->content.length();
                    cout << "write ok"<<endl;
                }
                else if(mode == 1){
                    string first = f->content.substr(0,f->pntr);
                    string second = "";
                    if (f->pntr+buff.length() < f->content.length())
                        second = f->content.substr(f->pntr+buff.length(),f->content.length());
                    f->content = first+buff+second;
                    f->pntr = f->content.length();
                    cout << "write ok"<<endl;
                }
                else if(mode == 2){
                    string first = f->content.substr(0,f->pntr);
                    string second = f->content.substr(f->pntr,f->content.length());
                    f->content = first+buff+second;
                    f->pntr = f->content.length();
                    cout << "write ok"<<endl;
                }
                else
                    cout << "permission denied"<<endl;
            }
        }
    }
}
void reposition(string name, int pos){
    file* f = fcheck(name);
    if(f == nullptr)
        cout << "file does not exist" << endl;
    else{
        if(pos <= f->content.length()){
            f->pntr = pos;
            cout << "reposition ok" << endl;
        }
        else
            cout << "reposition failed" << endl;
    }
}
void cat(string name){
    file* f = fcheck(name);
    if(f == nullptr)
        cout << "file does not exist" << endl;
    else{
        if(!f->opened){
            cout << "file does not open" << endl;
        }
        else{
            if(f->fprotect < 4) cout << "permission denied" << endl;
            else cout << f->content << endl; }
    }
}
void truncate(string name, int pos){
    file* f = fcheck(name);
    if(f == nullptr)
        cout << "file does not exist" << endl;
    else{
        if(pos <= f->content.length())
            f->content = f->content.substr(0,pos);
    }

}
void chmod(string name, int level){
    file* f = fcheck(name);
    if(f == nullptr)
        cout << "file does not exist" << endl;
    else
        f->fprotect = level;
}
void close(string name){
    file* f = fcheck(name);
    if(f == nullptr)
        cout <<"file does not exist" << endl;
    else{
        if(!f->opened)
            cout << "file does not open" <<endl;
        else{
            f->opened = false;
            f->opener = user; }
    }
}
string ls(){
    string out = "";
    for(dir *each : path->dirs){
        out += each->name + "\n"; }
    for(file *each : path->files){
        out += each->fname + "\n"; }
    return out;
}
string lsfile(){
    string out = "";
    dir* temp = path;
    path = root();
    list<dir*> queue;
    queue.push_back(path);
    while(!queue.empty()){
        path = queue.front();
        queue.pop_front();
        for(dir* each : path->dirs)
            queue.push_back(each);
        for(file* each : path->files){
            if(each->opened)
                out+=pwd()+" "+each->fname+" "+each->creater->name+" "+each->opener->name+" "+to_string(each->fprotect)+"\n";
        }
    }
    path = temp;
    return out;
}
usr* regusr(string name, string pwd, int type){
    if(ucheck(name) != nullptr){
        cout <<"user already exists" << endl;
        return nullptr;
    }
    else{
        usr* ausr = new usr;
        ausr->name = name;
        ausr->pwd = pwd;
        users.push_back(ausr);
        if (type)
            cout <<"register ok"<<endl;
        return ausr;
    }

}
void delusr(string name){
    usr* cur = ucheck(name);
    if(cur == nullptr)
        cout <<"user does not exist" << endl;
    else{
        for(auto it = users.begin();it!=users.end();++it){
            if(*it == cur){
                users.erase(it);
                return;
            }
        }
    }
}
void disusr(){
    if(user->name != "root")
        cout << "permission denied"<<endl;
    else{
        for(auto each:users)
            cout << each->name << endl;
    }
}
usr* login(string name){
    usr* cur = ucheck(name);
    if(cur == nullptr){
        cout <<"user does not exist" << endl;
        return nullptr;
    }
    else{
        string pwd;
        cout << "password:";
        cin >> pwd;
        if(cur->pwd == pwd){
            cout << "login ok" << endl;
            return cur;
        }
        else{
            cout << "password wrong" << endl;
            return nullptr;
        }
    }
}

void manual(){
    cout << "0) pwd (current work directory)" << endl;
    cout << "1) mkdir(p) (path) directory_name" << endl;
    cout << "2) dedir(p) (path) directory_name" << endl;
    cout << "3) search file_name" << endl;
    cout << "4) create(p) (path) file_name omode" << endl;
    cout << "5) cat(p) (path) file_name" << endl;
    cout << "6) delete(p) (path) file_name" << endl;
    cout << "7) write(p) (path) file_name buff wmode" << endl;
    cout << "8) reposition(p) (path) file_name pos" << endl;
    cout << "9) truncate(p) (path) file_name cnt" << endl;
    cout << "10) ls(p) (path)" << endl;
    cout << "11) open(p) (path) file_name" << endl;
    cout << "12) close(p) (path) file_name" << endl;
    cout << "13) regusr user_name password" << endl;
    cout << "14) delusr user_name [root only]" << endl;
    cout << "15) disUsr" << endl;
    cout << "16) login user_name" << endl;
    cout << "17) lsfile" << endl;
    cout << "18) chmod(p) (path) file_name mode" << endl;
    cout << "19) cd(p) (path) dir_name" << endl;
    cout << "20) exit" << endl;
}


int
main(){
    path = new dir;
    path->name = "/";
    path->pre = nullptr;
    user = regusr("root","111",0);
    string command;
    while(true){
        cout <<user->name<<"$"<<pwd()<<"%:";
        cin >> command;
        if(command == "exit") break;
        else if(command == "mkdir"){
            string name;
            cin >> name;
            mkdir(name);
        }
        else if(command == "mkdirp"){
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                mkdir(name);
            path = temp;
        }
//      对于所有的p类型的操作（例如dedirp）思路为：
//      先将当前path保存下来，然后进入目标目录执行原子操作（例如dedir），执行操作完还原path
        else if(command == "dedir"){
            string name;
            cin >> name;
            dedir(name);
        }
        else if(command == "dedirp"){
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                dedir(name);
            path = temp;
        }
        else if(command == "create"){
            string name;
            int mode;
            cin >> name >> mode;
            create(name,mode);
        }
        else if(command == "createp") {
            string dst,name;
            int mode;
            cin >> dst >> name >> mode;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                create(name,mode);
            path = temp;
        }
        else if(command == "delete"){
            string name;
            cin >> name;
            del(name);
        }
        else if(command == "deletep") {
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                del(name);
            path = temp;
        }
//      search和lsfile采用BFS搜索
        else if(command == "search"){
            string name;
            cin >> name;
            cout << search(name);
        }
        else if(command == "lsfile") {
            cout<<lsfile();
        }
        else if(command == "open"){
            string name;
            cin >> name;
            open(name);
        }
        else if(command == "openp"){
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                open(name);
            path = temp;
        }
        else if(command == "close"){
            string name;
            cin >> name;
            close(name);
        }
        else if(command == "closep"){
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                close(name);
            path = temp;
        }
        else if(command == "write"){
            string name,buff;
            int mode;
            cin >> name >> buff >> mode;
            write(name,buff,mode);
        }
        else if(command == "writep"){
            string dst,name,buff;
            int mode;
            cin >> dst >> name >> buff >> mode;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                write(name,buff,mode);
            path = temp;

        }
        else if(command == "cat"){
            string name;
            cin >> name;
            cat(name);
        }
        else if(command == "catp"){
            string dst,name;
            cin >> dst >> name;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                cat(name);
            path = temp;

        }
        else if(command == "truncate"){
            string name;
            int pos;
            cin >> name >> pos;
            truncate(name,pos);
        }
        else if(command == "truncatep"){
            string dst,name;
            int mode;
            cin >> dst >> name >> mode;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                truncate(name,mode);
            path = temp;
        }
        else if(command == "reposition"){
            string name;
            int pos;
            cin >> name >> pos;
            reposition(name,pos);
        }
        else if(command == "repositionp"){
            string dst,name;
            int mode;
            cin >> dst >> name >> mode;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                reposition(name,mode);
            path = temp;
        }
        else if(command == "chmod"){
            string name;
            int pos;
            cin >> name >> pos;
            chmod(name,pos);
        }
        else if(command == "chmodp"){
            string dst,name;
            int mode;
            cin >> dst >> name >> mode;
            dir* temp = path;
            path = cdp(dst);
            if(path!= nullptr)
                chmod(name,mode);
            path = temp;
        }
        else if(command == "pwd"){
            cout << pwd() << endl;
        }
        else if(command == "cd") {
            string name;
            cin >> name;
            dir* cur = cd(name);
            path = cur != nullptr ? cur : path;
        }
        else if(command == "cdp") {
            string dst;
            cin >> dst;
            dir* cur = cdp(dst);
            path = cur != nullptr ? cur : path;
        }
        else if(command == "man") {
            manual();
        }
        else if(command == "ls") {
            cout << ls();
        }
        else if(command == "lsp") {
            string dst;
            cin >> dst;
            dir* temp = path;
            path = cdp(dst);
            if(path != nullptr)
                cout << ls();
            path = temp;
        }
        else if(command == "regusr"){
            string name,pwd;
            cin >> name >> pwd;
            regusr(name,pwd,1);
        }
        else if(command == "delusr"){
            string name;
            cin >> name;
            delusr(name);
        }
        else if(command == "disusr")
            disusr();
        else if(command == "login"){
            string name;
            cin >> name;
            usr* t = login(name);
            user =  t != nullptr? t : user;
        }
    }
    return 0;
}