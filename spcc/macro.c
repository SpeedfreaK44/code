#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct mnt_table{
    char name[50];
    int motptr;
}mnt;
typedef struct mdt_table{
    char inst[50];
    char source[50];
}mdt;
typedef struct ala_table{
    char index[50];
    char args[50];
}ala;
mdt mdttable[50];
mnt mntentry[50];
ala alatable[50];

void main(){
    FILE *ptr;
    int i=0,j=1,mntc=0,m=0,q=0; //i and j for MDT and ALA Pass1, mntc for MNT, m for ALA Pass2,
    // q for Source Code before expansion
    char str[255];
    ptr=fopen("macro_input_modified.txt","r");
    if(ptr==NULL){
        printf("Error");
        exit(1);
    }
    // MACRO START
    fscanf(ptr,"%s",str);
    if (strcmp(str, "MACRO") == 0) {
        // MACRO DEFINITION
        fscanf(ptr,"%s",str);
        strcpy(mdttable[i].inst,str);
        // MNT Table
        strcpy(mntentry[mntc].name,str);
        mntentry[mntc].motptr = mntc;
        while (getc(ptr)!='\n'){
            // ALA Pass1 Table
            fscanf(ptr,"%s",str);
            strcpy(alatable[i].args,str);
            sprintf(str,"#%d",i);
            strcpy(alatable[i].index,str);
            i++;
        }
    }
    printf("\n----MNT----\nName\tMacroPtr\n");
    printf("%s\t%d\n\n",mntentry[mntc].name,mntentry[mntc].motptr);
    printf("----ALA Pass1----\nIndex\tDummyArgs\n");
    for(int k=0;k<i;k++) {
        printf("%s\t%s\n", alatable[k].index, alatable[k].args);
    }
    printf("\n----MDT----\n");
    for(int k=0;k<i;k++) {
        strcat(strcat(mdttable[0].inst, " "), alatable[k].args);
    }
    printf("%s\n",mdttable[0].inst);
    while(fscanf(ptr,"%s", str)==1){
        if(strcmp(str,"MEND")==0){
            break;
        }
        else {
            for(int k=0;k<i;k++) {
                if (strcmp(alatable[k].args, str) == 0) {
                    strcpy(str, alatable[k].index);
                }
            }
            strcpy(mdttable[j].inst, str);
            j++;
        }
    }
    for(int k=1;k<j;k+=3) {
        printf("%s  %s  %s\n", mdttable[k].inst, mdttable[k+1].inst, mdttable[k+2].inst);
    }
    strcpy(mdttable[j].inst,"MEND");
    printf("%s\n",mdttable[j].inst);
    while(fscanf(ptr,"%s",str)==1) {
        if (strcmp(mntentry[mntc].name, str) == 0) {
            while (fgetc(ptr) != '\n') {
                fscanf(ptr, "%s", str);
                strcpy(alatable[i].args, str);
                sprintf(str, "#%d", m);
                strcpy(alatable[m].index, str);
                i++;
                m++;
            }
            int n, p; // for replacing #0 in MDT with D1 from ALA Pass2
            for (p = 3, n = m; p <= (i + m) && n < i; p += 3, n++) {
                strcpy(mdttable[p].inst, alatable[n].args);
            }
            break;
        }
        else{
            strcpy(mdttable[q].source, str);
            q++;
        }
    }
    printf("\n----ALA Pass2----\nIndex\tActualArgs\n");
    int k,l; //k for index and l for argument
    for(k=0,l=m;k<m&&l<i;k++,l++) {
        printf("%s\t%s\n", alatable[k].index, alatable[l].args);
    }
    printf("\n----Expanded Source File----\n");
    for(int k=0;k<q;k+=3) {
        printf("%s  %s  %s\n", mdttable[k].source, mdttable[k+1].source, mdttable[k+2].source);
    }
    for(int k=1;k<j;k+=3) {
        printf("%s  %s  %s\n", mdttable[k].inst, mdttable[k+1].inst, mdttable[k+2].inst);
    }
    while (fgets(str, 255, ptr)) {
        printf("%s",str);
    }
    fclose(ptr);
}