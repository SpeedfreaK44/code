#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char mnemonic[10];
    int length;
}mot;

typedef struct{
    char pseudo[20];
}pot;

typedef struct{
    char symbol[20];
    int lc_val;
}st;

st symbol_table[10];
mot m_table[10];
pot p_table[10];

int lc = 0;
FILE *ptr;
char c;
int n = 0;

int motSearch(char str[]){
    int i=0;
    for(i=0;i<3;i++){
        if(strcmp(m_table[i].mnemonic,str) == 0){
            lc += m_table[i].length;
            return 1;
        }
    }

    return 0;
}

int potSearch(char str[]){
    int i=0;
    for(i=0;i<5;i++){
        if(strcmp(p_table[i].pseudo,str) == 0){
            if(strcmp(p_table[i].pseudo,"DC") == 0 || strcmp(p_table[i].pseudo,"DS") == 0){
                fscanf(ptr,"%s",str);
                if((str[0] >= 'A' && str[0] <='Z')){
                    if(strcmp(str,"F") == 0){
                        lc += 4;
                    }
                    else{
                        lc += 2;
                    }
                }
                else{
                    fscanf(ptr,"%s",str);
                    if(strcmp(str,"F") == 0){
                        lc += 4;
                    }
                    else{
                        lc += 2;
                    }
                }
            }
            return 1;
        }
    }

    return 0;
}

void skipLine(){
    while( (c = getc(ptr)) != '\n' ){
        if(c == EOF){
            break;
        }
    }
    return;
}

typedef struct basetable{
    int basereg;
    int lc_value;
}bt;
bt reg;

int btentry(char str[]){
        if(strcmp(str,"USING")==0){
            reg.lc_value=0;
            reg.basereg=15;
            return 1;
        }
        return 0;
}

void main(){
    char str[1000];
    char words[50];
    int val;
    int flag = 0;
    int j;

    ptr = fopen("Program.txt","r");

    if(ptr == NULL){
        printf("error");
        exit(1);
    }
    strcpy(p_table[0].pseudo,"START");
    strcpy(p_table[1].pseudo,"USING");
    strcpy(p_table[2].pseudo,"DC");
    strcpy(p_table[3].pseudo,"DS");
    strcpy(p_table[4].pseudo,"END");

    strcpy(m_table[0].mnemonic,"L");
    m_table[0].length = 4;

    strcpy(m_table[1].mnemonic,"A");
    m_table[1].length = 4;

    strcpy(m_table[2].mnemonic,"ST");
    m_table[2].length = 4;

    printf("----Symbol table----\n");
    printf("Symbol\tLC Value\n");

    while(fscanf(ptr,"%s",str)==1){
        flag = 0;
        for(j=0;j<(int)(sizeof(str)/sizeof(str[0]));j++){
            if((str[0] >= 'A' && str[0] <='Z')){
                flag = 1;
            }
        }
        if(flag == 1){
            val = potSearch(str);
            if (val == 1){
                skipLine();
            }
            else{
                val = motSearch(str);
                if (val == 1){
                    skipLine();
                }
                else{
                    strcpy(symbol_table[n].symbol,str);
                    symbol_table[n].lc_val = lc;
                    fscanf(ptr,"%s",str);
                    val = potSearch(str);
                    if(val == 0){
                        val = motSearch(str);
                    }
                    skipLine();
                    printf("%s\t%d\n",symbol_table[n].symbol,symbol_table[n].lc_val);
                    n++;
                }
            }
        }
    }
    fclose(ptr);

    ptr=fopen("Program.txt","r");
    while(fscanf(ptr,"%s",str)==1){
        val = btentry(str);
        if (val == 1) {
            printf("\n----Base Table----\n");
            printf("BaseReg\tValue\n");
            printf("%d\t%d\n", reg.basereg, reg.lc_value);
        }
    }
    printf("----Machine Code----\n");
    int offset[20];
    for(int i=0;i<3;i++){
        offset[i]=symbol_table[i+1].lc_val-reg.lc_value;
        printf("%s",m_table[i].mnemonic);
        printf(" 1, ");
        printf("%d",offset[i]);
        printf("(%d,%d)\n",reg.lc_value,reg.basereg);
    }

}
