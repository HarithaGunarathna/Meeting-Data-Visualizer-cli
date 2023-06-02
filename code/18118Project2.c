/*
   CO222 Project 2 - Meeting Data Visualizer
   Author :E/18/118
   Submitted :10/08/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define area() printf("\u2591") //Function to shade a simple 1x1 box 
#define hLine() printf("\u2500") //Function to print a horizontal line
#define vLine() printf("\u2502") //Function to print a vertical line
#define edge() printf("\u2514") //Funtion to print an edge

//Structure made for keeping entries
typedef struct _{
    char Name[100];
    int number;
    int time;
    double timeSum;
    int meetingNo;
    struct _* next;
} entry;

//Function signatures
entry* Readfile(char*,entry*,int*);
int getTime(char*);
double getTimeSum(char*);
entry* sortTime(entry*,entry*,int);
entry* sortParticipant(entry*,entry*,int);
entry* sortMeetNo(entry*,entry*,int);
void plotGraph(entry*,int,int,int,int);
int type_num(entry,int);
int MaxLen_pos(entry*,int);
int get_calc(int,int,int,int,int);
int digitSpace(int);
int charCheck(char*);
int nameSpace(char* arr);

//Global variables
int i=0;


int main(int argc,char**argv){
    
        int type=0;
        int scaled=0;
        int length=10;
        entry* first=NULL;

//Error handling for 1 argument
        if(argc==1){
            printf("No input files were given\n");
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
            return 0;  
        }
//Error handling for 2 arguments
        else if(argc==2){
            if(!strcmp(argv[1],"-p") || !strcmp(argv[1],"-m") || !strcmp(argv[1],"-t") || !strcmp(argv[1],"--scaled")){
                printf("No input files were given\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0; 
            }
                else if(!strcmp(argv[1],"-l")){
                printf("Not enough options for [-l]\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
                else if(argv[1][0]=='-'){
                printf("Invalid option [%s]\n",argv[1]);
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else if(strstr(argv[1],".csv")!=NULL){
                type=0;
                scaled=0;
                first= Readfile(argv[1],first,&i);
                if(i==-1){
                        printf("Cannot open one or more given files\n");
                        return 0;
                    }
                entry* sorted= (entry*)malloc(sizeof(entry)*i);
                sorted=sortMeetNo(first,sorted,i);
                plotGraph(sorted,i,type,scaled,length);
            }
            else{
                printf("Only .csv files should be given as inputs.\n");
                return 0;
            }
        }
//Error handling for multiple arguments
        else{
            int para_count_m=0,para_count_p=0,para_count_tm=0;
            int lcondition=0,invalarg=0,invalfile=0,valfile=0,filecorrupt=0,lfinal=0;
            char inval[1024];

            for(int a=1;a<argc;a++){
                if(!strcmp(argv[a],"-l")){
                    if(a!=argc-1){
                        if(charCheck(argv[a+1])!=-1){
                            length=atoi(argv[a+1]);
                            a++;
                        }
                        else{
                            lcondition=1; // there is no valid argument following -l
                            a++;
                        }
                    }
                    else
                        lfinal=1; // there are no arguments following -l
                }
                else if(!strcmp(argv[a],"-m")){
                    type=0;
                    para_count_m=1; //there is an -m argument
                }
                else if(!strcmp(argv[a],"--scaled")){
                    scaled=1;
                }
                else if(!strcmp(argv[a],"-p")){
                    type=1;
                    para_count_p=1; //there is an -p argument
                }
                else if(!strcmp(argv[a],"-t")){
                    type=2;
                    para_count_tm=1; //there is an -t argument
                }
                    else if(argv[a][0]=='-'){
                    invalarg=1; //there is an argument starting from [-] excluding -m -l -p -t --scaled
                    strcpy(inval,argv[a]);
                }   
                    else if(strstr(argv[a],".csv")!=NULL){ //there is a file which have .csv extention
                        first= Readfile(argv[a],first,&i);
                        valfile++;
                        if(i==-1){
                            filecorrupt=1; //file caanot be read
                        }
                    }   
                else{
                    invalfile=1; //there is anargument which doent have [-] at front 0r which is not an integer or which doesnt have .csv extention 
                } 
            }

            if(lcondition==1){
                printf("Invalid options for [-l]\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else if(lfinal==1){
                printf("Not enough options for [-l]\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else if(invalarg==1){
                printf("Invalid option [%s]\n",inval);
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else if(invalfile==1){
                printf("Only .csv files should be given as inputs.\n");
                return 0;
            }
            else if((para_count_m && para_count_p) || (para_count_m && para_count_tm) || (para_count_p && para_count_tm)){
                printf("Cannot plot multiple parameters in the same graph.\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else if(valfile==0){
                printf("No input files were given\n");
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }
            else{
                if(filecorrupt==1){
                    printf("Cannot open one or more given files\n");
                    return 0;
                }
            
            //sorting the linked list and keeling it in an dynamic allocation
                entry* sorted= (entry*)malloc(sizeof(entry)*i);
                if(type==1)
                    sorted=sortParticipant(first,sorted,i); 
                else if(type==2) 
                    sorted=sortTime(first,sorted,i); 
                else    
                    sorted=sortMeetNo(first,sorted,i);

            //plot the graph
              plotGraph(sorted,i,type,scaled,(int)length);
 
                return 0;
            }

        }

    return 0;
    
}

//Funtion to check whether the char is a number
int charCheck(char* arr){
    for(int j=0;arr[j] !='\0';j++){
        if(!isdigit(arr[j]))
            return -1;
    }
    return 0;
}
//Function to get the length of the name of a string
int nameSpace(char* arr){
    int m;
    for(m=0;arr[m] != '\0';m++){
    }
    return m;
}

//The function to plot the graph 
void plotGraph(entry* arr,int count,int type,int scaled,int length){
    int lines,hLinePos,halt,sum=0;
    int sum_time=0;
    if(count>length){
        lines=length;
        hLinePos=MaxLen_pos(arr,lines);

    }
    else{
        lines=count;
        hLinePos=MaxLen_pos(arr,lines);
    }
    if(type!=2){
        for(int n=0;n<i;n++){
            sum=type_num(arr[n],type)+sum;
        }  
    }
    else{
        for(int n=0;n<i;n++){
            sum_time=(int)arr[n].timeSum+sum_time;
        }
        sum=sum_time;

    }
        
        halt=strlen(arr[hLinePos].Name)+3;

        printf("\n");

        for(int n=0;n<lines;n++){

            for(int y=0;y<halt-1;y++){
                printf(" ");
            }
            vLine();

            for(int j=0;j<get_calc(type_num(arr[n],type),type_num(arr[0],type),80-(halt),scaled,sum);j++){
                area();
   
            }
            printf("\n");

            printf(" ");
            printf("%s",arr[n].Name);
            for(int y=0;y<(halt-1)-(nameSpace(arr[n].Name)+1);y++){
                printf(" ");
            }
            vLine();
            for(int j=0;j<get_calc(type_num(arr[n],type),type_num(arr[0],type),80-(halt),scaled,sum);j++){
                area();
            }
            printf("%d",type_num(arr[n],type));
            printf("\n");

            for(int y=0;y<halt-1;y++){
                printf(" ");
            }
            vLine();
            for(int j=0;j<get_calc(type_num(arr[n],type),type_num(arr[0],type),80-(halt),scaled,sum);j++){
                area();
            }
       
            printf("\n");
            for(int y=0;y<halt-1;y++){
                printf(" ");
            }
            vLine();
            printf("\n");
    
        }
      
        for(int y=0;y<halt-1;y++){
                printf(" ");
        }
        edge();
        for(int j=0;j<80-halt;j++){
            hLine();
        }
     
        printf("\n");
 
}

// Function to calculate the number of units needed to be colored in the graph
int get_calc(int var,int max,int space,int scaled,int sum){
    int ans;
    float ans_sum;
    if(scaled==0){

        ans_sum=(var*(space-digitSpace(max)))/(float)sum;

      
        ans=(int)ans_sum;
    }
    else if(scaled==1){
        ans=(var*(space-digitSpace(max)))/max;
    }
    return ans;
}

//Function which returns space needed to print the maximum number(digit space)
int digitSpace(int num){
    if((num/10)==0)
        return 1;
    else 
        return 1+digitSpace(num/10);
}

//Function which returns which of the following types we want to use(either meetingNo,the number of participants or time)
int type_num(entry var,int type){
    if(type==0)
        return var.meetingNo;
    else if(type==1)
        return var.number;
    else if(type==2)
        return var.time;
}   

//Function which determines the postition of the element which contains the maximum ammount of letters in their name
int MaxLen_pos(entry *arr,int count){
    int n,pos,j,a;
    for (n = 0; n < count; n++) {
        a=strlen(arr[0].Name);
        pos=0;
        for (j = 1; j < count; j++) {
            if (a < strlen(arr[j].Name)) {
                a=strlen(arr[j].Name);
                pos=j;
            }
        }
        return pos;
    }
}

//Function which reads the entries of a file and put them on a linked list and returns the address of the first node of the link
entry* Readfile(char* filename,entry* first,int* i){

    char fname[2560];
    char s[2]=",";
    char str[2048];
    int duplicate=0;
    char *token;
    char arrtime[256];
    entry* point;
    entry* present;
    entry* temp;

    snprintf(fname, sizeof(fname), "%s", filename);

    FILE *fp;
    fp=fopen(fname,"r");
    if(fp==NULL){
        *i=-1;
        return first;
    }
    while(fgets(str,2048,fp)!=NULL){
        int temp_m=0,t=0;
        double time_sum=0;
  //timeSum
        token =strtok(str,s);

        for(point=first;point!=NULL;point=point->next){
            if(!strcmp(point->Name,token)){
                duplicate=1;
                temp=point;
                break;
            }
        }

        if(duplicate==1){

            temp->meetingNo=temp->meetingNo+1;
            temp_m=temp->number;
            t=temp->time;
            time_sum=temp->timeSum; 
            token=strtok(NULL,s);

            temp->number=atoi(token)+ temp_m;
            token=strtok(NULL,s);
            strcpy(arrtime,token);
            temp->time=getTime(token) + t;
            temp->timeSum=getTimeSum(arrtime) + time_sum;

            *i=*i-1;
        }
        else{
            if(first==NULL){
                first=(entry*)malloc(sizeof(entry));
                present=first;
                first->next=NULL;
            }
            else{
                for(present = first; present->next != NULL; present=present->next){
                }
                present->next=(entry*)malloc(sizeof(entry));
                present=present->next;
                present->next=NULL;
            }
            strcpy(present->Name,token);
            token=strtok(NULL,s);

            present->number=atoi(token);
            token=strtok(NULL,s);

            strcpy(arrtime,token);
            present->time=getTime(token);
            present->timeSum=getTimeSum(arrtime);

            present->meetingNo=1;


        }
        *i=*i+1;
        duplicate=0;
    }

    fclose(fp);

    return first;

}

//Function which returns the total number of minutes a host held a meeting as an integer, when the time is provided as hh:mm:ss 
int getTime(char arr[]){

    char s[2]=":";
    char* token;
    int sec,min,hour;

    token=strtok(arr,s);
    hour=atoi(token);
    token=strtok(NULL,s);
    min=atoi(token);
    token=strtok(NULL,s);
    sec=atoi(token);

    return min+(sec/60)+hour*60;

}
//Function which returns the total number of minutes a host held a meeting as a double, when the time is provided as hh:mm:ss 
double getTimeSum(char arr[]){

    char s[2]=":";
    char* token;
    int sec,min,hour;

    token=strtok(arr,s);
    hour=atoi(token);
    token=strtok(NULL,s);
    min=atoi(token);
    token=strtok(NULL,s);
    sec=atoi(token);

    return min+(sec/60.0)+hour*60;;
    return 0;
}

//Fuction to sort the link list according to the total number of time a host held their meetings 
entry* sortTime(entry* first,entry* sorted,int i){
    int n;
    entry* pos;
    entry *point;
    entry* previous;
    int a;
    for (n = 0; n < i; n++) {
        a=first->time;
        pos=first;
        for (point = first; point != NULL; point=point->next) {
            if (a < point->time) {
                a=point->time;
                pos=point;
            }
            if(point->next != NULL){
                if(a<point->next->time)
                    previous=point;
            }
        }
        sorted[n].time=a;
        sorted[n].timeSum=pos->timeSum;
        sorted[n].number=pos->number;
        strcpy(sorted[n].Name,pos->Name);
        sorted[n].meetingNo=pos->meetingNo;
        if(pos==first){
            first=pos->next;
            free(pos);
        }
        else{
            previous->next=pos->next;
            free(pos);
        }
    }

    return sorted;
}

//Fuction to sort the link list according to the total number participants partipated in certain hosts' meetings 
entry* sortParticipant(entry* first,entry* sorted,int i){
    int n,a;
    entry* pos;
    entry* point;
    entry* previous;
    for (n = 0; n < i; n++) {
        a=first->number;
        pos=first;
        for (point = first; point != NULL; point=point->next) {
            if (a < point->number) {
                a=point->number;
                pos=point;
            }
            if(point->next != NULL){
                if(a< point->next->number)
                    previous=point;
            }
        }
        sorted[n].number=a;
        sorted[n].time=pos->time;
        sorted[n].timeSum=pos->timeSum;
        strcpy(sorted[n].Name,pos->Name);
        sorted[n].meetingNo=pos->meetingNo;
        if(pos==first){
            first=pos->next;
            free(pos);
        }
        else{
           previous->next=pos->next;
            free(pos);
        }
        
    }

    return sorted;
}

//Fuction to sort the linked list according to the total number meetings in certain hosts' meetings 
entry* sortMeetNo(entry* first,entry* sorted,int i){
    int n,a;
    entry* pos;
    entry *point;
    entry* previous;
    for (n = 0; n < i; n++) {
        a=first->meetingNo;
        pos=first;
        for (point = first; point != NULL; point=point->next) {
            if (a < point->meetingNo) {
                a=point->meetingNo;
                pos=point;
            }
            if(point->next != NULL){
                if(a< point->next->meetingNo)
                    previous=point;
            }
        }
        sorted[n].meetingNo=a;
        sorted[n].time=pos->time;
        sorted[n].timeSum=pos->timeSum;
        strcpy(sorted[n].Name,pos->Name);
        sorted[n].number=pos->number;
        if(pos==first){
            first=pos->next;
            free(pos);
        }
        else{
            previous->next=pos->next;
            free(pos);
        }

    }

    return sorted;
}
