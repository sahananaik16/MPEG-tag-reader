#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mp3_header.h"

/*fun_view function*/
status fun_view(Mpeg *mpeg)
{
    /*calling open_vfile function*/
    status ret = open_vfile(mpeg);
    if(ret==failure)
    {
        return failure;
    }
    /*calling check_head function*/
    status ret1 = check_head(mpeg);
    if(ret1==failure)
    {
        return failure;
    }
    /*skipping header file*/
    fseek(mpeg->fptr_view,10,SEEK_SET);
    /*calling v_title function*/
    status ret2 = v_title(mpeg);
    if(ret2==failure)
    {
        return failure;
    }
    /*calling v_artist function*/
    status ret7 = v_artist(mpeg);
    if(ret7==failure)
    {
        return failure;
    }
    /*calling v_album function*/
    status ret3 = v_album(mpeg);
    if(ret3==failure)
    {
        return failure;
    }
    /*calling v_year function*/
    status ret4 = v_year(mpeg);
    if(ret4==failure)
    {
        return failure;
    }
    /*calling v_content function*/
    status ret5 = v_content(mpeg);
    if(ret5==failure)
    {
        return failure;
    }
    /*calling v_comment function*/
    status ret6 = v_comment(mpeg);
    if(ret2==failure)
    {
        return failure;
    }
    /*calling print_view function*/
    print_view(mpeg);
    return success;
}

/*open_vfile function*/
status open_vfile(Mpeg *mpeg)
{
    /*opening the file to be viewed*/
    mpeg->fptr_view= fopen(mpeg->vfile, "r");
    
    /*validating the opened file*/
    if (mpeg->fptr_view == NULL)
    {
        /*printing error message when file is not found*/
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n",mpeg->vfile);
        printf("\033[0m");
    	return failure;
    }
    return success;
}

/*check_head function*/
status check_head(Mpeg *mpeg)
{
    /*declaring str string*/
    char str[3];
    /*reading 3 bytes from the source file*/
    fread(str,3,1,mpeg->fptr_view);
    /*checking the file identifier in the head of the file*/
    if(strcmp(str,"ID3")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid file identifier\nIt should be ID3\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring str1 string*/
    char str1[2] = {0};  
    /*reading 2 bytes from the source file*/
    fread(str1, 1, 2, mpeg->fptr_view);  
    /*checking the version of the file in the head of the file*/
    if (str1[0] != 0x03 || str1[1] != 0x00)
    {
        printf("\033[1;31m");
        printf("Invalid file version\nIt should be version 2(03 00)\n");
        printf("\033[0m");
        return failure;
    }
    /*bringing offset to the start position*/
    rewind(mpeg->fptr_view);
    return success;
}

/*v_title function*/
status v_title(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating title tag*/
    if(strcmp(buff,"TIT2")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid title tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading title size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*converting size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing title size*/
    mpeg->tit_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the title*/
    char str[size];
    /*reading title*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying title and storing*/
    strcpy(mpeg->title,str);
    return success;
}

/*v_artist function*/
status v_artist(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating artist tag*/
    if(strcmp(buff,"TPE1")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid artist tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading artist size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*convering size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing artist size*/
    mpeg->art_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the artist*/
    char str[size];
    /*reading artist*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying artist and storing*/
    strcpy(mpeg->artist,str);
    return success;
}

/*v_album function*/
status v_album(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 byte from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating album tag*/
    if(strcmp(buff,"TALB")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid album tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading album size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*convering size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing album size*/
    mpeg->alb_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the album*/
    char str[size];
    /*reading album*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying album and storing*/
    strcpy(mpeg->album,str);
    return success;
}

/*v_year function*/
status v_year(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 byte from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating year tag*/
    if(strcmp(buff,"TYER")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid year tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading year size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*converting size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing year size*/
    mpeg->y_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the year*/
    char str[size];
    /*reading year*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying year and storing*/
    strcpy(mpeg->year,str);
    return success;
}

/*v_content function*/
status v_content(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating content tag*/
    if(strcmp(buff,"TCON")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid content tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading content size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*converting size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing content size*/
    mpeg->con_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the content*/
    char str[size];
    /*reading content*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying content and storing*/
    strcpy(mpeg->content,str);
    return success;
}

/*v_comment function*/
status v_comment(Mpeg *mpeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 byte from the source file*/
    fread(buff,4,1,mpeg->fptr_view);
    /*validating comment tag*/
    if(strcmp(buff,"COMM")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid comment tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*declaring variable size and storing its address in pointer*/
    int size;
    void *ptr=&size;
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    /*reading comment size*/
    fread(&size,4,1,mpeg->fptr_view);
    int i=0;
    /*converting size to little endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
    /*storing comment size*/
    mpeg->com_size=size;
    /*skipping flag of 3 bytes*/
    fseek(mpeg->fptr_view,3,SEEK_CUR);
    /*string to store the comment*/
    char str[size];
    /*reading comment*/
    fread(str,size-1,1,mpeg->fptr_view);
    /*ending string with NULL character*/
    str[size-1]='\0';
    /*copying comment and storing*/
    strcpy(mpeg->comment,str);
    return success;
}

/*print_view function*/
void print_view(Mpeg *mpeg)
{
    printf("\033[1;36m");
    /*printing the details*/
    printf("-------------------------------------------SELECTED VIEW DETAIL-------------------------------------------------\n\n");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    printf("                                 MPEG TAG READER AND EDITOR FOR ID3v2\n");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    printf("TITLE\t:\t%s\n",mpeg->title);
    printf("ARTIST\t:\t%s\n",mpeg->artist);
    printf("ALBUM\t:\t%s\n",mpeg->album);
    printf("YEAR\t:\t%s\n",mpeg->year);
    printf("CONTENT\t:\t%s\n",mpeg->content);
    printf("COMMENT\t:\t%s\n",mpeg->comment);
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------DETAILS DISPLAYED SUCCESSFULLY---------------------------------------------\n");
    printf("\033[0m");
}