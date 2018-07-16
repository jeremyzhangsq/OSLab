//
// Created by Jeremy on 2018/5/22.
//
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;

void FCFS();
void SSTF();
void SCAN();
void C_SCAN();
void LOOK();
void C_LOCK();
int start;
int upper;
int size;
vector<int> task;
int
main(){
    cin>>start;
    cin>>upper;
    cin>>size;
    int ta;
    task.push_back(start);
    for(int i = 0; i< size;i++){
        cin >> ta;
        task.push_back(ta);
    }
    FCFS();
    SSTF();
    SCAN();
    C_SCAN();
    LOOK();
    C_LOCK();
}
void FCFS(){
    int sum = 0;
    int cur = start;
    printf("FCFS\n");
    for(int i = 0;i<size;i++){
        int t = task[i];
        printf("%d -> ",t);
        sum+=abs(cur-t);
        cur = t;
    }
    printf("%d\n",task[size]);
    sum+=abs(cur-task[size]);
    printf("%d\n",sum);

}
void SSTF(){
    int sum = 0;
    int pointer = -1;
    vector<int> tasks;
    for(int i:task) tasks.push_back(i);
    printf("SSTF\n");
    sort(tasks.begin(),tasks.end());
    for(int i = 0;i<=size;i++){
        if(tasks[i]==start){
            pointer = i;
            break;
        }
    }
    int cur = pointer;
    int left = pointer-1;
    int right = pointer+1;
    for(int time = 0;time<size;time++){
        printf("%d -> ",tasks[cur]);
        if(left >=0  && right <= size){
            int ls = abs(tasks[left]-tasks[cur]);
            int rs = abs(tasks[right]-tasks[cur]);
            if(ls<rs){
                sum+=ls;
                cur = left;
                left --;
            }
            else{
                sum+=rs;
                cur = right;
                right++;
            }
        }
        else if(right <= size){
            sum+=abs(tasks[right]-tasks[cur]);
            cur = right;
            right++;
        }
        else if(left >=0){
            sum+=abs(tasks[left]-tasks[cur]);
            cur = left;
            left --;
        }
    }
    if(right <= size){
        sum+=abs(tasks[right]-tasks[cur]);
        cur = right;
    }
    else if(left >=0){
        sum+=abs(tasks[left]-tasks[cur]);
        cur = left;
    }
    printf("%d\n",tasks[cur]);
    printf("%d\n",sum);
}
void SCAN(){
    int sum = 0;
    int pointer = -1;
    vector<int> tasks;
    for(int i:task) tasks.push_back(i);
    printf("SCAN\n");
    sort(tasks.begin(),tasks.end());
    for(int i = 0;i<=size;i++){
        if(tasks[i]==start){
            pointer = i;
            break;
        }
    }
    for(int i = pointer;i>=0;i--){
        printf("%d -> ",tasks[i]);
    }
    printf("0 -> ");
    sum+=tasks[pointer];
    for(int i = pointer+1;i<size;i++){
        printf("%d -> ",tasks[i]);
    }
    printf("%d\n",tasks[size]);
    sum+=tasks[size];
    printf("%d\n",sum);
}
void C_SCAN(){
    int sum = 0;
    int pointer = -1;
    vector<int> tasks;
    for(int i:task) tasks.push_back(i);
    printf("C-SCAN\n");
    sort(tasks.begin(),tasks.end());
    for(int i = 0;i<=size;i++){
        if(tasks[i]==start){
            pointer = i;
            break;
        }
    }
    for(int i = pointer;i>=0;i--){
        printf("%d -> ",tasks[i]);
    }
    printf("0 -> %d -> ",upper-1);
    sum+=tasks[pointer];
    for(int i = size;i>pointer+1;i--){
        printf("%d -> ",tasks[i]);
    }
    printf("%d\n",tasks[pointer+1]);
    sum+=upper-1-tasks[pointer+1];
    printf("%d\n",sum);
}
void LOOK(){
    int sum = 0;
    int pointer = -1;
    vector<int> tasks;
    for(int i:task) tasks.push_back(i);
    printf("LOOK\n");
    sort(tasks.begin(),tasks.end());
    for(int i = 0;i<=size;i++){
        if(tasks[i]==start){
            pointer = i;
            break;
        }
    }
    for(int i = pointer;i>=0;i--){
        printf("%d -> ",tasks[i]);
    }
    sum+=tasks[pointer]-tasks[0];
    for(int i = pointer+1;i<size;i++){
        printf("%d -> ",tasks[i]);
    }
    printf("%d\n",tasks[size]);
    sum+=tasks[size]-tasks[0];
    printf("%d\n",sum);
}
void C_LOCK(){
    int sum = 0;
    int pointer = -1;
    vector<int> tasks;
    for(int i:task) tasks.push_back(i);
    printf("C-LOOK\n");
    sort(tasks.begin(),tasks.end());
    for(int i = 0;i<=size;i++){
        if(tasks[i]==start){
            pointer = i;
            break;
        }
    }
    for(int i = pointer;i>=0;i--){
        printf("%d -> ",tasks[i]);
    }
    sum+=tasks[pointer]-tasks[0];
    for(int i = size;i>pointer+1;i--){
        printf("%d -> ",tasks[i]);
    }
    printf("%d\n",tasks[pointer+1]);
    sum+=tasks[size]-tasks[pointer+1];
    printf("%d\n",sum);
}