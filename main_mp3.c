#include<stdio.h>
#include<string.h>
#include"mp3_header.h"

/*main function*/
int main(int argc,char *argv[])
{
    /*declaring variable mpeg and empeg*/
    Mpeg mpeg;
    Editmpeg empeg;
    /*check_type function call*/
    type op = check_type(argc,argv);
    /*checking the return value from the check_type function*/
    if(op==view)
    {
        /*readval_v function call*/ 
        status vi = readval_v(argc,argv,&mpeg);
        if(vi==failure)
        {
            return failure;
        }
        /*fun_view function call*/ 
        status ret = fun_view(&mpeg);
        if(ret==failure)
        {
            return failure;
        }
    }
    else if(op==edit)
    {
        /*readval_e function call*/
        status ed = readval_e(argc,argv,&empeg);
        if(ed==failure)
        {
            return failure;
        }
        /*fun_edit function call*/
        status ret1 = fun_edit(&empeg);
        if(ret1==failure)
        {
            return failure;
        }
    }
    else if(op==help)
    {
        /*help_menu function call*/
        help_menu();
    }
    else
    {
        /*returning invalid when invalid argument is invalid*/
        return invalid;
    }

}

/*check_type function call*/
type check_type(int argc,char *argv[])
{
    /*checking argument count, if it is less than 1 print error message*/
    if(argc<=1)
    {
        printf("\033[1;31m");
        printf("---------------------------------------------------------------------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\nUSAGE :\n");
        printf("To view please pass like: ./a.out -v mpegfilename\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mpegfilename\n");
        printf("To get help pass like: ./a/out --help\n");
        printf("---------------------------------------------------------------------\n");
        printf("\033[0m");
        return invalid;
    }
    else
    {
        /*check the argument 2 to check the type of operation and return particular operation*/
        if(!strcmp(argv[1],"-v"))
        {
            return view;
        }
        if(!strcmp(argv[1],"-e"))
        {
            return edit;
        }
        if(!strcmp(argv[1],"--help"))
        {
            return help;
        }
        /*returning invalid when invalid argument is invalid*/
        return invalid;
    }
}

/*help_menu function*/
void help_menu()
{
    /*prompting help menu*/
    printf("\033[1;35m");
    printf("-------------------------------------HELP MENU--------------------------------------\n");
    printf("1. -v ->to view mp3 file contents\n");
    printf("2. -e ->to edit mp3 file contents\n");
    printf("\t2.1. -t -> to edit song title\n");
    printf("\t2.2. -a -> to edit artist name\n");
    printf("\t2.3. -A -> to edit album name\n");
    printf("\t2.4. -y -> to edit year\n");
    printf("\t2.5. -m -> to edit content\n");
    printf("\t2.6. -c -> to edit comment\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("\033[0m");
}

/*readval_v function*/
status readval_v(int argc,char *argv[],Mpeg *mpeg)
{
    /*checking argument count*/
    if(argc==3)
    {
        /*condition to check 3rd argument is mpeg file or not*/
        if(strstr(argv[2],".mpeg")==NULL)
        {
            /*printing error message if file is not of mpeg type*/
            printf("\033[1;31m");
            printf("Invalid mpeg file!!\n");
            printf("\033[0m");
            return failure;
        }
        /*storing the file name in variable*/
        mpeg->vfile=argv[2];
    }
    /*if argument count is not more than 3 print error message*/
    else
    {
        printf("\033[1;31m");
        printf("---------------------------------------------------------------------\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\nUSAGE :\n");
        printf("To view please pass like: ./a.out -v mpegfilename\n");
        printf("---------------------------------------------------------------------\n");
        printf("\033[0m");
        return failure;
    }
}

/*readval_e function*/
status readval_e(int argc,char *argv[],Editmpeg *empeg)
{
    /*checking argument count*/
    if(argc==5)
    {
        /*array of edit operation*/
        char *valid_ops[] = {"-t", "-a", "-A", "-m", "-y", "-c"};
        int valid = 0;
        /*loop to check the presence any edit operation*/
        for (int i = 0; i < 6; i++)
        {
            if (strcmp(argv[2], valid_ops[i]) == 0)
            {
                valid = 1;
                break;
            }
        }
        
        /*if no edit opration is mentioned print error message and return*/
        if (!valid)
        {
            printf("\033[1;31m");
            printf("---------------------------------------------------------------------\n");
            printf("ERROR: ./a.out : INVALID ARGUMENTS\nUSAGE :\n");
            printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mpegfilename\n");
            printf("---------------------------------------------------------------------\n");
            printf("\033[0m");
            return failure;
        }

        /*condition to check 5th argument is mpeg file or not*/
        if(strstr(argv[4],".mpeg")==NULL)
        {
            printf("\033[1;31m");
            printf("Invalid mpeg file!!\n");
            printf("\033[0m");
            return failure;
        }
        /*storing 2nd,4th,5th argument in variable*/
        empeg->efile=argv[4];
        empeg->oper=argv[2];
        empeg->dest=argv[3];
        return success;
    }
    /*printing error message if argument number is less than 5*/
    else
    {
        printf("\033[1;31m");
        printf("---------------------------------------------------------------------\n");
        printf("ERROR: ./a.out : INVALID NO. OF ARGUMENTS\nUSAGE :\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mpegfilename\n");
        printf("---------------------------------------------------------------------\n");
        printf("\033[0m");
        return failure;
    }
}