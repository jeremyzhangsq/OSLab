#include<bits/stdc++.h>
#include<unistd.h>
#include <list>
#include <map>
using namespace std;
#define DEFUALT_CACHE_SIZE 123
int cache_size,  hit, miss, type, page_size, fifo_size, lru_size;
vector<int> pages;
list<int> cache, fifo_cache, lru_cache;
map<int,list<int>::iterator> cache_map, fifo_cache_map, lru_cache_map;
map<int,int> queue_map;
map<int, list<int>> page_info;


void FIFO_algorithm();
bool isHit(int ele, map<int,list<int>::iterator> &cache_map);
void LRU_algorithm();
void SecondChance_algorithm();
void MIN_algorithm();
void Clock_algorithm();
void update(int ele,list<int> &cache, map<int,list<int>::iterator> &cache_map);
int delete_head(list<int> &cache, map<int,list<int>::iterator> &cache_map);
void insert_tail(int ele, list<int> &cache, map<int,list<int>::iterator> &cache_map);
void printlist(list<int> &cache);

int main(){
    int op;
    cache_size = DEFUALT_CACHE_SIZE;
    cin >> cache_size;
    cin >> type;
    cin >> page_size;
    pages.reserve(page_size);
    fifo_size = cache_size/2;
    lru_size = cache_size - fifo_size;

    for (int i = 0; i<page_size; i++){
        cin >> op;
        pages.push_back(op);
        if(page_info.count(op) > 0)
            page_info[op].push_back(i);
        else{
            list<int> alist;
            alist.push_back(i);
            page_info.insert(make_pair(op,alist));
        }
    }
//    for(auto& l: page_info){
//        printf("%d's list:\n",l.first);
//        printlist(l.second);
//    }
//    exit(-1);
    switch (type){
        case 0: FIFO_algorithm(); break;
        case 1: LRU_algorithm(); break;
        case 2: MIN_algorithm(); break;
        case 3: Clock_algorithm(); break;
        case 4: SecondChance_algorithm(); break;
        default: break;
    }
}

void printlist(list<int> &cache){
    for(list<int>::iterator i = cache.begin(); i != cache.end(); ++i)
        cout << *i << " ";
    cout << endl;
}

bool isHit(int ele, map<int,list<int>::iterator> &cache_map){
    return cache_map.count(ele) > 0 ;
}

void update(int ele, list<int> &cache, map<int,list<int>::iterator> &cache_map){
    cache.erase(cache_map[ele]);
    insert_tail(ele,cache,cache_map);
}

int delete_head(list<int> &cache, map<int,list<int>::iterator> &cache_map){
    int ele = cache.front();
    cache.pop_front();
    cache_map.erase(ele);
    return ele;
}
void insert_tail(int ele, list<int> &cache, map<int,list<int>::iterator> &cache_map){
    cache.push_back(ele);
    if(isHit(ele,cache_map))
        cache_map[ele] = prev(cache.end(),1);
    else
        cache_map.insert(make_pair(ele,prev(cache.end(),1)));
}

void FIFO_algorithm(){
    while(!pages.empty()){
        int ele = pages.front();
        pages.erase(pages.begin());
        if(isHit(ele, cache_map)) hit++;
        else{
            miss++;
            if(cache.size()< cache_size){
                insert_tail(ele, cache, cache_map);
            }
            else{
                delete_head(cache, cache_map);
                insert_tail(ele, cache, cache_map);
            }
        }
    }
    printf("Hit ratio = %.2f%%\n",hit/(double)page_size*100);
}


void LRU_algorithm(){
    while(!pages.empty()){
        int ele = pages.front();
        pages.erase(pages.begin());
        if(isHit(ele,cache_map)) {
            update(ele,cache, cache_map);
            hit++;
        }
        else{
            miss++;
            if(cache.size()< cache_size){
                insert_tail(ele,cache, cache_map);
            }
            else{
                delete_head(cache, cache_map);
                insert_tail(ele,cache, cache_map);
            }
        }
    }
    printf("Hit ratio = %.2f%%\n",hit/(double)page_size*100);
}


void SecondChance_algorithm(){
    while(!pages.empty()){
        int ele = pages.front();
        pages.erase(pages.begin());
        if(isHit(ele,fifo_cache_map)){
            hit++;
//            printf("hit fifo\n");
//            printlist(fifo_cache);
//            printlist(lru_cache);
//            printf("--------------------\n");
        }
        else if(isHit(ele,lru_cache_map)){
//            printf("hit lru\n");
            hit++;
            lru_cache.erase(lru_cache_map[ele]);
            lru_cache_map.erase(ele);
            int tail = fifo_cache.front();
            fifo_cache_map.erase(tail);
            fifo_cache.pop_front();
            insert_tail(ele,fifo_cache, fifo_cache_map);
            insert_tail(tail,lru_cache, lru_cache_map);
//            printlist(fifo_cache);
//            printlist(lru_cache);
//            printf("--------------------\n");
        }
        else{
            miss++;
            if(fifo_cache.size()< fifo_size){
//                printf("fifo not full\n");
                insert_tail(ele, fifo_cache, fifo_cache_map);
//                printlist(fifo_cache);
//                printlist(lru_cache);
//                printf("------------------\n");

            }
            else{

                int tail = fifo_cache.front();
                fifo_cache.pop_front();
                fifo_cache_map.erase(tail);
                if(lru_cache.size() == lru_size) {
//                    printf("lru full\n");
                    delete_head(lru_cache, lru_cache_map);
                }
                insert_tail(tail,lru_cache, lru_cache_map);
                insert_tail(ele,fifo_cache, fifo_cache_map);
//                printlist(fifo_cache);
//                printlist(lru_cache);
//                printf("------------------\n");
            }
        }
    }
    printf("Hit ratio = %.2f%%\n",hit/(double)page_size*100);
}

void MIN_algorithm(){
    int count = 0;
    while(!pages.empty()){
        int ele = pages.front();
        pages.erase(pages.begin());
        if(isHit(ele,cache_map)) {
            update(ele,cache, cache_map);
            hit++;
        }
        else{
            miss++;
            if(cache.size()< cache_size){
                insert_tail(ele,cache, cache_map);
            }
            else{
                int item = -1;
                int index = -1;
                for(list<int>::iterator i = cache.begin(); i != cache.end(); ++i){
                    int cur = *i;
//                    printf("cur cache node:%d\n",cur);
//                    int front = page_info[cur].front();
                    while(!page_info[cur].empty() && page_info[cur].front()<count){
//                        front = page_info[cur].front();
//                        printf("cur top index:%d\tcount:%d\n",front,count);
                        page_info[cur].pop_front();
                    }
                    if(page_info[cur].empty()){
                        item = cur;
                        break;
                    }
                    if(index < page_info[cur].front()){
                        index = page_info[cur].front();
                        item = cur;
                    }
                }
//                printf("item:%d\n",item);
                if(item == -1) delete_head(cache, cache_map);
                else{
                    cache.erase(cache_map[item]);
                    cache_map.erase(item);
                }
                insert_tail(ele,cache, cache_map);
//                printlist();
            }
        }
        count++;
    }
    printf("Hit ratio = %.2f%%\n",hit/(double)page_size*100);
}

void Clock_algorithm(){
    int queue[cache_size];
    int valid[cache_size];
    for(int i = 0;i<cache_size;i++) {
        queue[i] = -1;
        valid[i] = 0;
    }
    int hand = 0;
    int count = 0;
    while(!pages.empty()){
        int ele = pages.front();
        pages.erase(pages.begin());
        count++;
        if(queue_map.count(ele) > 0) {
            int p = queue_map[ele];
            valid[p] = 1;
            hit++;

        }
        else{
            miss++;

            if(count < cache_size){
                queue[hand] = ele;
                valid[hand] = 1;
                if(queue_map.count(ele) > 0) queue_map[ele] = hand;
                else queue_map.insert(make_pair(ele,hand));
                hand = (hand+1)%cache_size;
            }
            else{
                int pnt = hand;
                while(valid[pnt]) {
                    valid[pnt] = 0;
                    pnt = (pnt+1)%cache_size;
                }
                queue_map.erase(queue[pnt]);
                queue[pnt] = ele;
                valid[pnt] = 1;
                hand = (pnt+1)%cache_size;
                if(queue_map.count(ele) > 0) queue_map[ele] = pnt;
                else queue_map.insert(make_pair(ele,pnt));
            }

        }
    }
    printf("Hit ratio = %.2f%%\n",hit/(double)page_size*100);
}



