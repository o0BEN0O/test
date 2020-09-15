#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define lan_conf_date "/home/ben/ben/4g_router/test/lan_conf_date"

int stringFind(const char *string, const char *dest) {
	if (string == NULL || dest == NULL) return -1;

	int i = 0;
	int j = 0;
	while (string[i] != '\0') {
	    if (string[i] != dest[0]) {
	        i ++;
	        continue;
	    }
	    j = 0;
	    while (string[i+j] != '\0' && dest[j] != '\0') {
	        if (string[i+j] != dest[j]) {
	            break;
	        }
	        j ++;
	    }
	    if (dest[j] == '\0') return j;
	    i ++;
	}
	return -1;
}

int get_value_from_conf(char *file_name,char *key,char *value,char *identifier)
{
	char line_buffer[64]={0};
	char get_buffer[64]={0};
	char key_buffer[64]={0};
	int line_len=0;
	int value_len=0;
	int pos=0;
	FILE *fp = fopen(file_name, "r+");
	if(identifier != NULL)
		snprintf(key_buffer,64,"%s%s",key,identifier);
	else
		snprintf(key_buffer,64,"%s",key);
	if(fp == NULL)
	{
		printf("open error");
		return -1;
	}
	while(fgets(line_buffer, 64, fp))
	{
		line_len=strlen(line_buffer);
		pos += line_len;
		if(stringFind(line_buffer,key_buffer)==strlen(key_buffer)){
			pos -= strlen(line_buffer);
			pos += strlen(key_buffer);
			value_len = (strlen(line_buffer)-strlen(key_buffer));/*rm \n*/
			fseek(fp,pos,SEEK_SET);
			fread(value,1,value_len-1,fp);
		}
	}
	fclose(fp);
	return 0;
}

int set_value_to_conf(char *file_name,char *key,char *value,char *identifier)
{
	char line_buffer[64]={0};
	char set_buffer[64]={0};
	char key_buffer[64]={0};
	char *clean_buf;
	int line_len=0;
	int set_len=0;
	int pos=0;
	int i=0;
	FILE *fp = fopen(file_name, "r+");
	if(identifier != NULL)
		snprintf(key_buffer,64,"%s%s",key,identifier);
	else
		snprintf(key_buffer,64,"%s",key);
	if(fp == NULL)
	{
		printf("open error");
		return -1;
	}
	while(fgets(line_buffer, 64, fp))
	{
		line_len=strlen(line_buffer);
		pos += line_len;
		if(stringFind(line_buffer,key_buffer)==strlen(key_buffer)){
			pos -= line_len;
			fseek(fp,pos,SEEK_SET);
			if(strlen(set_buffer)<line_len){
				snprintf(set_buffer,64,"%s=%s\n",key,value);
				set_len=strlen(set_buffer);
				fwrite(set_buffer,1,set_len,fp);
				//fprintf(fp,"\n");
				clean_buf = (char *)malloc(sizeof(char)*(line_len-set_len));
				for(i=0;i<line_len-set_len;i++)
					strncpy(clean_buf+i,"\0",1);
				//bzero(clean_buf,line_len);
				fseek(fp,pos+set_len,SEEK_SET);
				fwrite(clean_buf,1,line_len-set_len,fp);
				free(clean_buf);
				break;
			}
			snprintf(set_buffer,64,"%s=%s",key,value);
			fprintf(fp,"%s",set_buffer);
			fprintf(fp,"\n");
			break;
		}
	}
	fclose(fp);
	return 0;
}
void main(void)
{
	char value[64]={0};
	set_value_to_conf(lan_conf_date,"BOOTMODE","11111","=");
	get_value_from_conf(lan_conf_date,"BOOTMODE",value,"=");
	printf("%s %d",value,(int)strlen(value));
}
