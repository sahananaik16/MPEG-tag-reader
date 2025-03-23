#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mp3_header.h"

/*fun_edit function*/
status fun_edit(Editmpeg *empeg)
{
    /*calling open_efile function*/
    status ret = open_efile(empeg);
    if(ret==failure)
    {
        return failure;
    }
    /*calling check_ehead function*/
    status ret1 = check_ehead(empeg);
    if(ret1==failure)
    {
        return failure;
    }
    /*declaring charater buffer*/
    char buff[10];
    /*redaing head from source file writing it to new file*/
    fread(buff,10,1,empeg->fptr_edit);
    fwrite(buff,10,1,empeg->fptr_dest);
    /*calling title_edit function*/
    status ret2 = title_edit(empeg);
    if(ret2==success)
    {
        printf("\033[1;32m");
        printf("Title updated succesfully..\n");
        printf("\033[0m");
        return failure;
    }
    /*calling artist_edit function*/
    status ret3 = artist_edit(empeg);
    if(ret3==success)
    {
        printf("\033[1;32m");
        printf("Artist updated succesfully...\n");
        printf("\033[0m");
        return failure;
    }
    /*calling album_edit function*/
    status ret4 = album_edit(empeg);
    if(ret4==success)
    {
        printf("\033[1;32m");
        printf("Album updated succesfully...\n");
        printf("\033[0m");
        return failure;
    }
    /*calling year_edit function*/
    status ret5 = year_edit(empeg);
    if(ret5==success)
    {
        printf("\033[1;32m");
        printf("Year updated succesfully...\n");
        printf("\033[0m");
        return failure;
    }
    /*calling content_edit function*/
    status ret6 = content_edit(empeg);
    if(ret6==success)
    {
        printf("\033[1;32m");
        printf("Content updated succesfully...\n");
        printf("\033[0m");
        return failure;
    }
    /*calling comment_edit function*/
    status ret7 = comment_edit(empeg);
    if(ret7==success)
    {
        printf("\033[1;32m");
        printf("Comment updated succesfully...\n");
        printf("\033[0m");
        return failure;
    }
    return success;
}

/*open_efile function*/
status open_efile(Editmpeg *empeg)
{
    /*opening the file to be editing*/
    empeg->fptr_edit= fopen(empeg->efile, "rb");

    /*validating the opened file*/
    if (empeg->fptr_edit == NULL)
    {
        /*printing error message when file is not found*/
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n",empeg->efile);
        printf("\033[0m");
    	return failure;
    }

    /*storing the new file name*/
    empeg->newfile="new.mpeg";
    /*opening the file where all the contenyt should be copied*/
    empeg->fptr_dest=fopen(empeg->newfile, "wb");
    return success;
}

/*check_ehead function*/
status check_ehead(Editmpeg *empeg)
{
    /*declaring str string*/
    char str[3];
    /*reading 3 bytes from the source file*/
    fread(str,3,1,empeg->fptr_edit);
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
    fread(str1, 2, 1, empeg->fptr_edit);  
    /*checking the version of the file in the head of the file*/
    if (str1[0] != 0x03 || str1[1] != 0x00)
    {
        printf("\033[1;31m");
        printf("Invalid file version\nIt should be version 2(03 00)\n");
        printf("\033[0m");
        return failure;
    }
    /*bringing offset to the start position*/
    rewind(empeg->fptr_edit);
    return success;
}

/*endian function*/
void endian(void *ptr)
{
    /*storingting void pointer in char pointer*/
    char *ptr1=(char *)ptr;
    /*decalring temporary variable*/
    char temp;
    int i=0;
    /*converting the endian*/
    while(i<2)
    {
        temp=ptr1[i];
        ptr1[i]=ptr1[3-i];
        ptr1[3-i]=temp;
        i++;
    }
}

/*copy_remain function*/
status copy_remain(Editmpeg *empeg)
{
    /*declaring ch*/
    char ch;
    /*reading character from the file and storing in ch until EOF is obtained*/
    while(fread(&ch,1,1,empeg->fptr_edit))
    {
        /*writing read character to the new file*/
        fwrite(&ch,1,1,empeg->fptr_dest);
    }
    /*closing the opened files*/
    fclose(empeg->fptr_edit);
    fclose(empeg->fptr_dest);

    /*removing the old file*/
    int i=remove(empeg->efile);
    /*validating the file removal*/
    if(i==-1)
    {
        printf("\033[1;31m");
        printf("Original file is not deleted..\n");
        printf("\033[0m");
    }
    
    /*renaming the new file name with old file name*/
    int j=rename(empeg->newfile,empeg->efile);
    /*validating the file renaming*/
    if(j==-1)
    {
        printf("\033[1;31m");
        printf("Original file is not renamed..\n");
        printf("\033[0m");
    }
    return success;
}

/*title_edit function*/
status title_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating title tag*/
    if(strcmp(buff,"TIT2")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid title tag!!\n");
        printf("\033[0m");
        return failure;
    }

    /*writing title tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the title edit operator is present or not*/
    if((strcmp(empeg->oper,"-t"))==0)
    {
        /*finding the size of the new title name and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old title size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new title name to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading title info when title editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading title from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

/*artist_edit function*/
status artist_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating artist tag*/
    if(strcmp(buff,"TPE1")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid artist tag!!\n");
        printf("\033[0m");
        return failure;
    }

    /*writing artist tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the artist edit operator is present or not*/
    if((strcmp(empeg->oper,"-a"))==0)
    {
        /*finding the size of the new artist name and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old artist size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new artist name to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading artist info when artist editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading artist from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

/*album_edit function*/
status album_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating album tag*/
    if(strcmp(buff,"TALB")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid album tag!!\n");
        printf("\033[0m");
        return failure;
    }

    /*writing album tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the album edit operator is present or not*/
    if((strcmp(empeg->oper,"-A"))==0)
    {
        /*finding the size of the new album name and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old album size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new album name to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading album info when album editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading album from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

/*year_edit function*/
status year_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating year tag*/
    if(strcmp(buff,"TYER")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid year tag!!\n");
        printf("\033[0m");
        return failure;
    }

    /*writing year tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the year edit operator is present or not*/
    if((strcmp(empeg->oper,"-y"))==0)
    {
        /*finding the size of the new year which is edited and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old year size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new year to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading year info when year editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fread(buff1,3,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading year from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

/*content_edit function*/
status content_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating content tag*/
    if(strcmp(buff,"TCON")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid content tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*writing content tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the content edit operator is present or not*/
    if((strcmp(empeg->oper,"-m"))==0)
    {
        /*finding the size of the new content and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old content size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new content to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading content info when content editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading content from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

/*comment_edit function*/
status comment_edit(Editmpeg *empeg)
{
    /*declaring buffer*/
    char buff[4];
    /*reading 4 bytre from the source file*/
    fread(buff,4,1,empeg->fptr_edit);
    /*validating comment tag*/
    if(strcmp(buff,"COMM")!=0)
    {
        printf("\033[1;31m");
        printf("Invalid content tag!!\n");
        printf("\033[0m");
        return failure;
    }
    /*writing comment tag to the new file*/
    fwrite(buff,4,1,empeg->fptr_dest);
    /*checking the comment edit operator is present or not*/
    if((strcmp(empeg->oper,"-c"))==0)
    {
        /*finding the size of the new comment and storing it in size*/
        int size=strlen(empeg->dest)+1;
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing size to the new file*/
        fwrite(&size,4,1,empeg->fptr_dest);
        int old_size;
        /*reading old comment size from the source file and storing*/
        fread(&old_size,4,1,empeg->fptr_edit);
        /*endian conversion using endian function call*/
        endian(&old_size);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*skipping old size*/
        fseek(empeg->fptr_edit,old_size-1,SEEK_CUR);
        /*endian conversion using endian function call*/
        endian(&size);
        /*writing the new comment to the new file*/
        fwrite(empeg->dest,size-1,1,empeg->fptr_dest);
        /*copying remaining data*/
        status ret = copy_remain(empeg);
        if(ret == failure)
        {
            printf("\033[1;31m");
            printf("copying failed!!\n");
            printf("\033[0m");
            return failure;
        }
    }
    /*reading comment info when comment editing operator is not found*/
    else
    {
        /*declaring size variable*/
        int size;
        /*reading size from the source file and writing it to new file*/
        fread(&size,4,1,empeg->fptr_edit);
        fwrite(&size,4,1,empeg->fptr_dest);
        /*declaring buffer*/
        char buff1[3];
        /*reading flag from the source file and writing it to new file*/
        fread(buff1,3,1,empeg->fptr_edit);
        fwrite(buff1,3,1,empeg->fptr_dest);
        /*endian conversion using endian function call*/
        endian(&size);
        /*declaring buffer*/
        char buff2[size];
        /*reading comment from the source file and writing it to new file*/
        fread(buff2, 1, size-1, empeg->fptr_edit);
        fwrite(buff2, 1, size-1, empeg->fptr_dest);
        return failure;
    }
}

