//
// Created by Administrator on 2018/5/2.


#include <iostream>
using namespace std;
int
main(void){

    int line;
    int cache[64];
    for(int i =0;i<64;i++) cache[i] = 0;
    int valid[64];
    for(int i =0;i<64;i++) valid[i] = 0;
    int hit = 0;
    int miss = 0;

    while(cin >> hex >> line){
        if(line!=14){
            int tag = line >> 10;
            int cache_addr = line ^ (tag << 10);
            int cache_index = cache_addr >> 4;

            if(cache[cache_index] != tag){
                miss++;
                printf("Miss\n");
                cache[cache_index] = tag;
                valid[cache_index] = 1;
            }
            else if(cache[cache_index] == tag && valid[cache_index] == 1){
                hit++;
                printf("Hit\n");
            }
        }
    }
    double ratio = hit/(double)(hit+miss) *100;
    printf("Hit ratio = %.2f%%\n",ratio);
    return 0;
}
