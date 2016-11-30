#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


#ifdef LY_DEBUG	
#define ly_printf(format, args...) printf(format, ## args)
#else
#define ly_printf(format, args...)
#endif



#define HUAWEI_KEY_1 "huawei_key"
#define HUAWEI_KEY_2 "huawei_pages"
#define HUAWEI_KEY_3 "Unlock_pages"

	
#define WEBS_PATH    "/webs"
#define DOC_PATH     "/doc"


#define EQUIP_LARGE    256
#define EQUIP_MAX      512

#define ERR            -1
#define MNT_FORCE      1
#define MNT_DETACH    2 // w0135358 add for two users switch 20080722
/*****************************************************************************
 �� �� ��  : main
 ��������  : ��docĿ¼�µ�ҳ�����webs��ҳ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��5��11��
    ��    ��   : liuyang 65130
    �޸�����   : �����ɺ���

*****************************************************************************/
int main(int argc, char* argv[])
{

    int ret = ERR;
    DIR* pDocDir = NULL;
    struct dirent* pNext = NULL;
    char pFileName[EQUIP_LARGE];
    char pNewPath[EQUIP_LARGE];
    
    
    if (argc < 3)
    {
        return ret;
    }

    if (!strcmp(argv[1], HUAWEI_KEY_1) && !strcmp(argv[2], HUAWEI_KEY_2))
    {
        ly_printf("Welcome to visit huawei webs for more configuration\r\n");

        /*mount*/
        if (mount(NULL, WEBS_PATH, "tmpfs", 0, NULL))
        {
            ly_printf("Can't mount webs.\r\n");
            return ret;
        }

        /*open old-pages dir*/
        pDocDir = opendir(DOC_PATH);
        if (!pDocDir)
        {
            ly_printf("Can't open dir.\r\n");
            return ret;
        }

        while ((pNext = readdir(pDocDir)) != NULL) 
        {
            /* ���³�ʼ�� */
            memset(pFileName, 0, sizeof(pFileName));
            memset(pNewPath, 0, sizeof(pNewPath));

            /* ��/proc�й��˵�".."*/
            if (strcmp(pNext->d_name, "..") == 0)
            {
                continue;
            }
            if (strcmp(pNext->d_name, ".") == 0)
            {
                continue;
            }

            /*creat soft link for each html*/
            //sprintf(pCmd, "ln -sf %s/%s %s/%s", DOC_PATH, pNext->d_name, WEBS_PATH, pNext->d_name);
            //ly_printf("%s\r\n", pCmd);
            sprintf(pFileName, "%s/%s", DOC_PATH, pNext->d_name);
            sprintf(pNewPath, "%s/%s", WEBS_PATH, pNext->d_name);
            //system(pCmd);
            symlink(pFileName, pNewPath);
         }

         closedir(pDocDir);
         ly_printf("Ready to Go\r\n");
    }
    else if (!strcmp(argv[1], HUAWEI_KEY_1) && !strcmp(argv[2], HUAWEI_KEY_3))
    {
	/*modified by w00135358 for two users swtich 20080722*/
        //if (umount2(WEBS_PATH, MNT_FORCE))
        if (umount2(WEBS_PATH, MNT_DETACH))
        {
            fprintf(stderr,"[%s:%d]umount failed,errno: %s\n",__FILE__,__LINE__,strerror(errno));
        }
    }
    else
    {
        fprintf(stderr,"[%s:%d]unknown command,argv[1]:%s,argv[2]:%s\n",__FILE__,__LINE__,argv[1],argv[2]);
        return ERR;
    }

    return 0;
}


