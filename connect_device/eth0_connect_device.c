#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


#define path_iplist "/tmp/eth0_dev_list"
#define path_dhcpd_lease "/var/lib/dhcp/dhcpd.leases"

typedef struct
{
	char ip[20];
	char mac[20];
	char name[64];
}_eth0_dev_list;

int counter_eth0_device(void)
{
	FILE *fp;
	int cap=0;
	int size=0;
	struct stat statbuf;
	stat(path_iplist,&statbuf);
	size=statbuf.st_size;
	if(size == 0)
		return -1;
	if((fp=fopen(path_iplist,"r"))==NULL)
	{
		printf("Can not open this file.\n");
		fclose(fp);
		return -1;
	}
	while(!feof(fp))
	{
		if(getc(fp)!=EOF)
		{
			if(getc(fp)=='\n')
				cap++;
		}
	}
	cap++;
	fclose(fp);
	return cap;
}

int get_eth0_device(_eth0_dev_list *eth0_dev_list,int cap)
{ 
	FILE *fp;
	FILE *fp_dhcpd_lease;
	char *point,*end_point;
	char buf[40];
	char *dhcpd_lease;
	int length;
	int i=0;
	struct stat statbuf;
	stat(path_dhcpd_lease,&statbuf);

	if((fp=fopen(path_iplist,"r"))==NULL)
	{
		printf("Can not open this file.\n");
		fclose(fp);
		return -1;
	}
	fseek(fp,0,0);
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		memset(eth0_dev_list[i].ip,0,sizeof(eth0_dev_list[i].ip));
		memset(eth0_dev_list[i].ip,0,sizeof(eth0_dev_list[i].mac));
		point=strstr(buf," ");
		if(point==NULL){
			fclose(fp);
			return -1;
		}
		printf("%d %d\n",strlen(buf),strlen(point));
		memcpy(eth0_dev_list[i].ip,buf,strlen(buf)-strlen(point));
		memcpy(eth0_dev_list[i].mac,buf+strlen(buf)-strlen(point)+1,strlen(point)-2);
		printf("mac %s\n",eth0_dev_list[i].mac);
		printf("ip %s\n",eth0_dev_list[i].ip);
		i++;
	}
	fclose(fp);
	if((fp_dhcpd_lease=fopen(path_dhcpd_lease,"r"))==NULL)
	{
		printf("Can not open this file.\n");
		//free(eth0_dev_list);
		fclose(fp_dhcpd_lease);
		return -1;
	}

	//fseek(fp_dhcpd_lease, 0, SEEK_END);
	//length = ftell(fp_dhcpd_lease);
	length = statbuf.st_size;
	dhcpd_lease=malloc(length);
	fseek(fp_dhcpd_lease, 0, SEEK_SET);
	fread(dhcpd_lease,1,length,fp_dhcpd_lease);
	//printf("%s",dhcpd_lease);
	for(i=0;i<cap;i++){
		point=strstr(dhcpd_lease,eth0_dev_list[i].mac);
		if(point==NULL){
			free(dhcpd_lease);
			//free(eth0_dev_list);
			fclose(fp_dhcpd_lease);
			return -1;
		}
		point=strstr(point,"client-hostname");
		if(point==NULL){
			free(dhcpd_lease);
			//free(eth0_dev_list);
			fclose(fp_dhcpd_lease);
			return -1;
		}
		end_point=strstr(point,";");
		if(point==NULL){
			free(dhcpd_lease);
			//free(eth0_dev_list);
			fclose(fp_dhcpd_lease);
			return -1;
		}
		strncpy(eth0_dev_list[i].name,point+17,end_point-point-18);
	}
	free(dhcpd_lease);
	//free(eth0_dev_list);
	fclose(fp_dhcpd_lease);
	return 0;
}

#if 1
int main(void)
{
	_eth0_dev_list *eth0_dev_list;
	int cnt;
	int ret;
	int i;
	cnt=counter_eth0_device();
	if(cnt<0){
		printf("not eth0 device connect");
		return 0;
	}
	eth0_dev_list=malloc(sizeof(_eth0_dev_list)*cnt);
	ret=get_eth0_device(eth0_dev_list,cnt);
	if(ret!=-1){
		for(i=0;i<cnt;i++){
			printf("ip %s end\n",eth0_dev_list[i].ip);
			//printf("mac %s cnt:%d\n",eth0_dev_list[i].mac,strlen(eth0_dev_list[i].mac));
			printf("name %s\n",eth0_dev_list[i].name);
		}
	}
	free(eth0_dev_list);
	return cnt;
}
#endif

