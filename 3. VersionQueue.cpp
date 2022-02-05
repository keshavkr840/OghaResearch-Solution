#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d", &n);
    int mainArray[n+1];
    int headArray[n+1];
    int tailArray[n+1];
    int currentInputIndex = 1;
    int currentVersionIndex = 1;
    for(int i=0; i<n+1; i++){
        mainArray[i] = 0;
        headArray[i] = 0;
        tailArray[i] = 0;
    }
    headArray[0] = 1;
    tailArray[0] = 0;

    for(int i=0; i<n; i++){
        char c;
        std::cout<<"press e for enqueue \n
                  press d for dequeue\n
                  press p for print"<<"\n";
        scanf(" %c", &c);
        if(c=='e'){
            scanf("%d", &mainArray[currentInputIndex]);
            currentInputIndex++;
            headArray[currentVersionIndex] = headArray[currentVersionIndex-1];
            tailArray[currentVersionIndex] = tailArray[currentVersionIndex-1] + 1;
            currentVersionIndex++;
        }else if(c=='d'){
            if(tailArray[currentVersionIndex] < headArray[currentVersionIndex]){ // Queue is empty
                headArray[currentVersionIndex] = headArray[currentVersionIndex-1];
                tailArray[currentVersionIndex] = tailArray[currentVersionIndex-1];
                currentVersionIndex++;
            }else{
                headArray[currentVersionIndex] = headArray[currentVersionIndex-1] + 1;
                tailArray[currentVersionIndex] = tailArray[currentVersionIndex-1];
                currentVersionIndex++;
            }
        }else if(c=='p'){
            int printVersion;
            scanf("%d", &printVersion);
            printf("printing version %d: ", printVersion);
            for(int j=headArray[printVersion]; j<=tailArray[printVersion]; j++){
                printf("%d ", mainArray[j]);
            }
            printf("\n");
        }
    }
    return 0;
}
