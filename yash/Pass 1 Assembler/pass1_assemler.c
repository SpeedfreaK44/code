#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//struct for Mnemonic Opcode Table (MOT)
typedef struct 
{
	char mnemonic[5];
	int length;
} mot;

//struct for pseudo opcode table
typedef struct 
{
	char pseudo[10];
} pot;

//struct for Symbol Table
typedef struct 
{
	char symbol[5];
	int lc_val;
	char relocation;
} st;

mot m_table[10];
pot p_table[10];
st s_table[10];

FILE *ptr;
char *line = NULL;
ssize_t read;
size_t len = 0;
char buffer [15][5][10];
int lc = 0;
int sym_count = 0;
int i, j, val;

int getIndex(char *str)
{
	int k;
	for(k = 0; k < sym_count; k++)
	{
		if(strcmp(s_table[k].symbol, str) == 0)
		{
			return k;
		}
	}
}

int potSearch(char *str)
{
	int k;
	for(k = 0;k < 7; k++)
	{
		if (strcmp(str, p_table[k].pseudo) == 0)
		{
			if (strcmp(str, "DC") == 0 || strcmp(str, "DS") == 0)
			{
				if (strcmp(buffer[i][j+1], "F") == 0)
				{
					lc += 4;
				}
				else if (strcmp(buffer[i][j+1], "H") == 0)
				{
					lc += 2;
				}
				else if (buffer[i][j+1][1] == 'F')
				{
					lc += ((buffer[i][j+1][0] - '0') * 4);
				}
				else
				{
					lc += ((buffer[i][j+1][0] - '0') * 2);	
				}
			}

			if (strcmp(str, "EQU") == 0)
			{
				int index = getIndex(buffer[i][j-1]);
				s_table[index].lc_val = buffer[i][j+1][0] - '0';
				s_table[index].relocation = 'A';
			}
			return 1;
		}
	}
	return 0;
}

void printTables()
{
	int k;
	printf("\nPOT:\n\n");
	printf("PSEUDO:\n");
	for(k=0;k<7;k++)
	{
		printf("%s\n", p_table[k].pseudo);
	}

	printf("\nMOT:\n\n");
	printf("MNEMONIC  LENGTH\n");
	for(k=0;k<5;k++)
	{
		printf("%s\t    %d\n", m_table[k].mnemonic, m_table[k].length);
	}

	printf("\nST:\n\n");
	printf("SYMBOL  LC  RELOATION\n");
	for(k=0;k<sym_count;k++)
	{
		printf("%s\t%d\t%c\n", s_table[k].symbol, s_table[k].lc_val, s_table[k].relocation);
	}

	printf("BUFFER\n");
	for (i = 0; i < 12; i++)
	{
		for(j = 0; j < 5; j++)
		{
			printf("%s\t", buffer[i][j]);
		}
		printf("\n");
	}
}

int motSearch(char *str)
{
	int k;
	for(k=0; k<5; k++)
	{
		if (strcmp(str, m_table[k].mnemonic) == 0)
		{
			lc += m_table[k].length;
			return 1;
		}
	}
	return 0;
}

void main()
{
	strcpy(m_table[0].mnemonic, "L");
	m_table[0].length = 4;

	strcpy(m_table[1].mnemonic, "A");
	m_table[1].length = 4;

	strcpy(m_table[2].mnemonic, "ST");
	m_table[2].length = 4;

	strcpy(m_table[3].mnemonic, "BNE");
	m_table[3].length = 4;

	strcpy(m_table[4].mnemonic, "BR");
	m_table[4].length = 4;

	strcpy(p_table[0].pseudo, "START");
	strcpy(p_table[1].pseudo, "BALR");
	strcpy(p_table[2].pseudo, "USING");
	strcpy(p_table[3].pseudo, "EQU");
	strcpy(p_table[4].pseudo, "DC");
	strcpy(p_table[5].pseudo, "DS");
	strcpy(p_table[6].pseudo, "END");

	ptr = fopen("program.txt", "r");
	char delim[] = " ";
	int line_num = 0;
	int word_num = 0;

	while( (read = getline(&line, &len, ptr)) != -1 )
	{
		word_num = 0;
		char *ptr = strtok(line, delim);
		while( ptr != NULL )
		{
			strcpy(buffer[line_num][word_num], ptr);
			word_num++;
			ptr = strtok(NULL, delim);
		}
		line_num++;
	}

	for(i = 0; i < line_num; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if( !isalpha(buffer[i][j][0]) )
			{
				break;
			}
			val = potSearch(buffer[i][j]);
			if(val)
			{
				//break when found in POT
				break;
			}
			else
			{
				val = motSearch(buffer[i][j]);
				if (val)
				{
					//break when found in MOT
					break;
				}
				else
				{
					strcpy(s_table[sym_count].symbol, buffer[i][j]);
					s_table[sym_count].lc_val = lc;
					s_table[sym_count].relocation = 'R';
					sym_count++;
				}
			}
		}
	}
	printTables();
	fclose(ptr);
}