#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    success,
    failure
} status;

/*type will be used to identify the type of operation*/
typedef enum
{
    view,
    edit,
    help,
    invalid
} type;

/*structure having variables used for viewing*/
typedef struct _Mpeg
{
    /*view source file*/
    char *vfile;
    FILE *fptr_view;

    /*title info*/
    uint tit_size;
    char title[100];

    /*artist info*/
    uint art_size;
    char artist[100];

    /*album info*/
    uint alb_size;
    char album[100];

    /*year info*/
    uint y_size;
    char year[5];

    /*content info*/
    uint con_size;
    char content[100];

    /*comment info*/
    uint com_size;
    char comment[100];
}Mpeg;

/*structure having variables used for editing*/
typedef struct editfile
{
    /*new file info*/
    char *newfile;
    FILE *fptr_dest;
    /*file to be edited info*/
    char *efile;
    FILE *fptr_edit;
    /*edited text*/
    char *dest;
    /*opeartion type*/
    char *oper;
}Editmpeg;

/* Check operation type */
type check_type(int argc,char *argv[]);

/*to print help menu*/
void help_menu();

/* Read and validate view args from argv */
status readval_v(int argc,char *argv[],Mpeg *mpeg);

/* Read and validate edit args from argv */
status readval_e(int argc,char *argv[],Editmpeg *mpeg);

/*viewing function prototype*/

/*viewing function*/
status fun_view(Mpeg *mpeg);

/*opening files needed for viewing*/
status open_vfile(Mpeg *mpeg);

/*checking head of the file*/
status check_head(Mpeg *mpeg);

/*viewing title*/
status v_title(Mpeg *mpeg);

/*viewing artist*/
status v_artist(Mpeg *mpeg);

/*viewing album*/
status v_album(Mpeg *mpeg);

/*viewing year*/
status v_year(Mpeg *mpeg);

/*viewing content*/
status v_content(Mpeg *mpeg);

/*viewing comment*/
status v_comment(Mpeg *mpeg);

/*printing the information*/
void print_view(Mpeg *mpeg);

/*editing function prototype*/

/*editing function*/
status fun_edit(Editmpeg *empeg);

/*opening files needed for editing*/
status open_efile(Editmpeg *empeg);

/*checking head of the file*/
status check_ehead(Editmpeg *empeg);

/*editing title*/
status title_edit(Editmpeg *empeg);

/*converting endian of the system*/
void endian(void *ptr);

/*copying remaining data other than editing*/
status copy_remain(Editmpeg *empeg);

/*editing artist*/
status artist_edit(Editmpeg *empeg);

/*editing album*/
status album_edit(Editmpeg *empeg);

/*editing year*/
status year_edit(Editmpeg *empeg);

/*editing content*/
status content_edit(Editmpeg *empeg);

/*editing comment*/
status comment_edit(Editmpeg *empeg);

#endif
