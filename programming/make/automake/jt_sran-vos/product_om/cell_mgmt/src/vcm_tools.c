/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: vcm_tools.c
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/**
 *  init ip_create_flag is              000000...000
 *  when you create ipv4, it's turn to  000000...001 
 *                                   -> 000000...011 
 *                                   -> 000000...111
 *  when it's turn to 111111...111, 
 *                          reset it to 000000...000
 */
vcm_set     ip_create_flag; 

ipv4_t      IPv4_table[VCM_IF_IP_NUM];


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: remove_dir("d/b/c");
 **********************************************************************************************************************/
INT32 remove_dir(CONST CHAR *dir)
{   
#ifndef S_ISDIR
#define S_ISDIR(mode)   __S_ISTYPE((mode), __S_IFDIR)
#endif
#ifndef S_ISREG
#define	S_ISREG(mode)   __S_ISTYPE((mode), __S_IFREG)
#endif
    if(dir == NULL)
    {
        return VCM_ERR;
    }
    CHAR cur_dir[] = ".";   
    CHAR up_dir[] = "..";   
    CHAR dir_name[VCM_STR_LEN]; 
    DIR *dirp;  
    struct dirent *dp;  
    struct stat dir_stat;   
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(VCM_OK != Access(dir, F_OK))  
    {       
        return VCM_OK;   
    }   
    /**
     * Get attribution error
     */   
    if(0 > Stat(dir, &dir_stat))    
    {       
        Fprintf(STDERR, "Get directory stat error.");        
        return VCM_ERR;  
    }   
    /**
     *  If regular file, delete it.
     */    
    if(S_ISREG(dir_stat.st_mode))   
    {       
        Remove(dir);    
    }   
    else if(S_ISDIR(dir_stat.st_mode))  
    {       
        dirp = Opendir(dir);        
        while((dp = Readdir(dirp))!= NULL)      
        {           
            if((0 == Strcmp(cur_dir, dp->d_name)) ||               
                (0 == Strcmp(up_dir, dp->d_name)) )          
            {               
                continue;           
            }           
            Sprintf(dir_name, "%s/%s", dir, dp->d_name);            
            remove_dir(dir_name);       
        }       
        Closedir(dirp);     
        Rmdir(dir); 
    }   
    else    
    {       
        Fprintf(STDERR, "Unknow file type!");    
    }
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: create_dir("b/b/c/d/e/f/", 0777);
 **********************************************************************************************************************/
INT32 create_dir(CONST CHAR *dir, mode_t mode)
{
    if(dir == NULL)
    {
        return VCM_ERR;
    }
    char Dir[VCM_STR_LEN];
    Memset(Dir, 0, VCM_STR_LEN);
    Strcpy(Dir, dir);
    
    /**
     *  If directory not exist, return 0.
     */ 
    if(VCM_OK == Access(Dir, F_OK))  
    {       
        DIR *dirp;
        if((dirp = Opendir(Dir)) == NULL)
        {
            Fprintf(STDERR, "File already exist, but it's not directory.");
            return VCM_ERR;
        }
        Closedir(dirp);
        return VCM_OK;   
    }
    else
    {
        #if 0
        #ifdef S_IRUSR
        #define MKDIR_UMASK (S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)
        #else
        #define MKDIR_UMASK 0755
        #endif
        #endif
        Mkdir(Dir, mode);
        Dirname(Dir);
        create_dir(Dir, mode);
        Mkdir(Dir, mode);
    }

    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
char *current_dir()
{
    char *dir = (char*)Malloc(VCM_STR_LEN);
    getcwd(dir, VCM_STR_LEN);
    return dir;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: System("mkdir %s", "directory")
 **********************************************************************************************************************/
INT32 System(CONST CHAR *cmd, ...)
{
    va_list arg;
    va_start(arg, cmd);
    CHAR Cmd[VCM_STR_LEN];
    Memset(Cmd, 0, VCM_STR_LEN);
    Vsprintf(Cmd, cmd, arg);
    
    struct passwd *pwd;
    pwd = getpwuid(getuid());

    if(Strlen(Cmd) <= 0)
    {
        goto Error_flag;
    }
    Fprintf(STDOUT, "\033[31m[%s:%s %s]%c\033[m %s\n", 
            pwd->pw_name, 
            Basename(pwd->pw_shell), 
            Basename(current_dir()), 
            Strcmp(pwd->pw_name, "root")==0?'#':'$',
            Cmd);
            
    va_end(arg);
    return system(Cmd);
    
Error_flag:
    va_end(arg);
    return VCM_ERR;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: 
 *    ```c
 *    INT32 ret = vcm_yes_or_no("Are you OK?");
 *    if(ret == VCM_YES)
 *    {
 *        Printf("I'm OK.\n");
 *    }
 *    else if(ret == VCM_NO)
 *    {
 *        Printf("I'm not OK.\n");
 *    }
 *    ```
 **********************************************************************************************************************/
INT32 Q_A(CONST CHAR* question)
{
    if(question == NULL)
    {
        return VCM_NO;
    }
    
    CHAR *line = Malloc(VCM_STR_LEN);
    SIZE_T len;
    
    /* Use a safe default. */
    INT32 answer = VCM_NO;
    
    Fputs (gettext (question), STDOUT);
    
    Fflush (STDOUT);
    /* Prepare the getline call. */
    line = NULL;
    len = 0;
    
    while (getline (&line, &len, STDIN) >= 0)
    {
        /* Check the response. */
        int ret = rpmatch (line);
        if (ret >= 0)
        {
            /* We got a defnitive answer. */
            if (ret > 0)
                answer = VCM_YES;
            break;
        }
    }
    /* Free what getline allocated. */
    Free(line);
    
    //Fprintf(stdout, "%s\n", doit==true?"Yes, you select YES":"No, You select NO");
    
    return answer;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: 
 ```c
CHAR ipv4[256] = {"125.3.265.0"};
is_ipv4_valid(VCM_NETMASK, ipv4, VCM_Correct);
log_vcm_warn("%s\n", ipv4);
 ```
 OR
 ```c
int ret = is_ipv4_valid(VCM_NETMASK, "125.3.265.0", VCM_DoNothing);
log_vcm_warn("ret = %d\n", ret);

 ```
 **********************************************************************************************************************/
INT32 is_ipv4_valid(VCM_IPv4_TYPE type, CHAR *ipv4, VCM_OPERATOR_TYPE opr)
{
    /**
     *  Check parameters, if NULL pointer or string length lower than "0.0.0.0", return ERROR
     */
    if(ipv4 == NULL || Strlen(ipv4) < 7)
    {
        log_vcm_err("NULL pointer or invalid Ipv4 .\n");
        return VCM_ERR;
    }

    /**
     *  regular ipv4, netmask, broadcast is support. else return ERROR.
     */
    if(type <= VCM_IPv4_TYPE_MIN || type >= VCM_IPv4_TYPE_MAX)
    {
        log_vcm_err("No such kind of IPv4 type.\n");
        return VCM_ERR;
    }

    INT32   ret = VCM_ERR, modify = 0;
    /** 
     *  Decode the IPv4 to 4 unsigned int
     */
    UINT32 u1, u2, u3, u4;
    
    Sscanf(ipv4, "%u.%u.%u.%u", &u1, &u2, &u3, &u4);

    /* switch: regular ipv4, netmask, broadcast */
    switch(type)
    {
        /**
         *  regular ipv4 valid check: 0.0.0.0 - 255.255.255.255 
         *  if 256.257.0.0, will be alter to 255.2.0.0
         */
        case VCM_REGULAR_IP:
            /* determine correct this ipv4 or not */
            switch(opr)
            {
                case VCM_Correct:
                    if(u1>0xff){u1%=0xff;modify=1;}
                    if(u2>0xff){u2%=0xff;modify=1;}
                    if(u3>0xff){u3%=0xff;modify=1;}
                    if(u4>0xff){u4%=0xff;modify=1;}
                    ret = VCM_OK;
                    break;
                case VCM_DoNothing:
                    ret = VCM_OK;
                    if(u1>0xff){ret = VCM_ERR;}
                    if(u2>0xff){ret = VCM_ERR;}
                    if(u3>0xff){ret = VCM_ERR;}
                    if(u4>0xff){ret = VCM_ERR;}
                    break;
                default:
                    break;
            }
            
            break; //VCM_REGULAR_IP
            
        /**
         *  netmask valid check: 0.0.0.0 - 255.255.255.255 
         *  if 256.257.0.0, will be alter to 255.255.0.0
         */
        case VCM_NETMASK:
            /* determine correct this ipv4 or not */
            switch(opr)
            {
                case VCM_Correct:
                    if(u1>0xff){u1=0xff;modify=1;}
                    if(u2>0xff){u2=0xff;modify=1;}
                    if(u3>0xff){u3=0xff;modify=1;}
                    if(u4>0xff){u4=0xff;modify=1;}
                    ret = VCM_OK;
                    break;
                case VCM_DoNothing:
                    ret = VCM_OK;
                    if(u1>0xff){ret = VCM_ERR;}
                    if(u2>0xff){ret = VCM_ERR;}
                    if(u3>0xff){ret = VCM_ERR;}
                    if(u4>0xff){ret = VCM_ERR;}
                    break;
                default:
                    break;
            }
            break; //VCM_NETMASK

        case VCM_BROADCAST:
            //TODO: not support right now
            break; //VCM_BROADCAST
            
        default:
            //TODO nothing
            break;
    }

    if(modify == 1)
    {
        Sprintf(ipv4, "%u.%u.%u.%u", u1, u2, u3, u4);
    }

    return ret;
}


/***********************************************************************************************************************
 * Description: create a virtual ipv4 
 *
 * Input: cell_virt_id
 *
 * Output:  ipv4
 *          return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ***********************************************************************************************************************/
INT32 create_ipv4(UINT8 *ipv4)
{
    /**
     *  make sure all cell's mac IPv4 different, include already were deleted cells 
     */
    UINT32 iIPv4 = 0;
    static UINT32 counter = 0;
    
    for(iIPv4 = 0; iIPv4 < VCM_IF_IP_NUM; iIPv4++)
    {
        /**
         *  mac_used_IPv4_table = "11110000" means first four already used,
         *  and begin with "VCM_IF_IP_BASE".
         */
        if(IPv4_table[iIPv4].flag == 1) //already used
        {
            continue;
        }
        else if(IPv4_table[iIPv4].flag == 0) //not used
        {
            IPv4_table[iIPv4].flag = 1;
            VCM_SET(iIPv4, &ip_create_flag);
            counter++;
            break;
        }
    }
    
    if(iIPv4 == VCM_IF_IP_NUM)
    {
        iIPv4 = 0;
        counter = 0;
        VCM_ZERO(&ip_create_flag);
        Memset(IPv4_table, 0, VCM_IF_IP_NUM*sizeof(ipv4_t));
    }

    /* get virtual IPv4 */
    struct in_addr addr1;
    int l1, validflag = 0;
    l1 = inet_addr(VCM_IF_IP_BASE) + htonl(iIPv4);
    Memcpy(&addr1, &l1, 4);
    Strcpy(ipv4 , inet_ntoa(addr1));

    /* check if valid or not */
    unsigned int u1, u2, u3, u4;
    Sscanf(ipv4, "%u.%u.%u.%u", &u1, &u2, &u3, &u4);

    if(u1>255)
    {
        u1 %= 255;
        validflag = 1;
        log_vcm_warn("virtual ipv4(%s) error\n", ipv4);
    }
    if(u2>255)
    {
        u2 %= 255;
        validflag = 1;
        log_vcm_warn("virtual ipv4(%s) error\n", ipv4);
    }
    if(u3>255)
    {
        u3 %= 255;
        validflag = 1;
        log_vcm_warn("virtual ipv4(%s) error\n", ipv4);
    }
    if(u4>255)
    {
        u4 %= 255;
        validflag = 1;
        log_vcm_warn("virtual ipv4(%s) error\n", ipv4);
    }

    if(validflag == 1)
    {
        Sprintf(ipv4, "%u.%u.%u.%u", u1, u2, u3, u4);
    }

    is_ipv4_valid(VCM_REGULAR_IP, (CHAR*)ipv4, VCM_Correct);

    Memcpy(IPv4_table[iIPv4].ipv4, ipv4, VCM_IP_LEN);
    
    return VCM_OK;
}



/***********************************************************************************************************************
 * Description: show memory bytes on screen
 *
 * Input: p     :memory address
 *        nbytes:number of byte(uint8_t)
 *        col   :colonm on screen show
 *
 * Output: return:  success: VCM_OK
 *                  error:   VCM_ERR
 *
 *  Example:
 Memdisplay(STDOUT, &all_cell_set, VCM_CELL_NUM, 10);
 Memdisplaycmp(STDOUT, &all_cell_set, &all_cell_set, VCM_CELL_NUM, 10);
 **********************************************************************************************************************/
INT32 Memdisplay(FILE *fp, CONST VOID *p, UINT32 nbytes, UINT8 col)
{
	if(p == NULL || nbytes <= 0 || col <= 0)
	{
		return VCM_ERR;
	}
	CONST UINT8 *byte;
	byte = p;

	INT ibyte;
	for(ibyte=0; ibyte<nbytes; ibyte++)
	{
		Fprintf(fp, " %02x", *byte);
		byte++;
		if((ibyte+1)%col == 0)
		{
			Fprintf(fp, " \n");
		}
	}
	Fprintf(fp, " \n");
    
	Fflush(fp);
	return VCM_OK;
}

/***********************************************************************************************************************
 * Description: show comparation memory bytes on screen
 *
 * Input: p1     :memory address
 *        p2     :memory address
 *        nbytes :number of byte(uint8_t)
 *        col    :colonm on screen show
 *
 * Output: return:  success: VCM_OK
 *                  error:   VCM_ERR
 *
 *  Example:
 Memdisplay(STDOUT, &all_cell_set, VCM_CELL_NUM, 10);
 Memdisplaycmp(STDOUT, &all_cell_set, &all_cell_set, VCM_CELL_NUM, 10);
 **********************************************************************************************************************/
INT32 Memdisplaycmp(FILE *fp, CONST VOID *p1, CONST VOID *p2, UINT32 nbytes, UINT8 col)
{
    if(p1 == NULL || p2 == NULL || nbytes <= 0 || col <= 0)
	{
		return -1;
	}
    CONST UINT8 *byte1, *byte2;
    byte1 = p1;
    byte2 = p2;
    
    INT i1, i2, i11 = 0, i22 = 0;
    do{
        for(i1=0; i1<col; i1++)
        {
            i11++;
            if(i11 > nbytes)
            {
                i11--;
                Fprintf(fp, "   ");
            }
            else
            {
                Fprintf(fp, "%02x ", *byte1);
                byte1++;
            }
        }
        Fprintf(fp, " | ");
        for(i2=0; i2<col; i2++)
        {
            i22++;
            if(i22 > nbytes)
            {
                i22--;
                Fprintf(fp, "   ");
            }
            else
            {
                Fprintf(fp, "%02x ", *byte2);
                byte2++;
            }
        }
        Fprintf(fp, " \n");
    }while(i11 < nbytes && i22 < nbytes);

    Fflush(fp);
    return 0;
}


/***********************************************************************************************************************
 * Description: cell management module perror function, like glibc's perror() interface
 *
 * Input: script: a string to descripte 
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 *
 *  Example:
         vcmPerror("perror");
         __vcm_set_errno(_VCM_EICMACETHER);
         vcmPerror("perror");
         __vcm_set_errno(_VCM_EICRRUSTATUS);
         vcmPerror("perror");
     example output:
        perror:Vcm Success
        perror:Invalid cell mac's ether interface
        perror:Invalid cell rru's status
 **********************************************************************************************************************/
INT32 vcm_perror(CONST CHAR *script)
{
    if(Strlen(__vcm_errno_string) <= 0)
    {
        Fprintf(STDERR, "vcm_perror error\n");
        return VCM_ERR;
    }
    if(script != NULL || Strlen(script) > 0)
    {
        Fprintf(STDERR, "%s: ", script);
    }
    Fprintf(STDERR, "%s\n", __vcm_errno_string);

    return VCM_OK;
}
weak_alias(vcm_perror, cell_mgmt_perror);

#if 0

void
/* $f err_ret $ */
err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error related to a system call.
 * Print a message and terminate. */

void
/* $f err_sys $ */
err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */

void
/* $f err_dump $ */
err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */

void
/* $f err_msg $ */
err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */

void
/* $f err_quit $ */
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Print a message and return to caller.
 * Caller specifies "errnoflag". */

static VOID err_doit(INT32 errnoflag, CONST CHAR *fmt, va_list ap)
{
	int		errno_save;
	char	buf[MAXLINE];

	errno_save = errno;		/* value caller might want printed */
	Vsprintf(buf, fmt, ap);
	if (errnoflag)
		Sprintf(buf+Strlen(buf), ": %s", strerror(errno_save));
	Strcat(buf, "\n");
	Fflush(STDOUT);		/* in case stdout and stderr are the same */
	Fputs(buf, STDERR);
	Fflush(STDERR);		/* SunOS 4.1.* doesn't grok NULL argument */
	return;
}
#endif


/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 net_if_create(CONST CHAR *ifname, CONST CHAR *ip, CONST CHAR *netmask)
{
    log_vcm_debug();

    if(ifname == NULL || ip == NULL || netmask == NULL)
    {
        log_vcm_err("NULL pointer Error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    /**
     *  Use ifconfig+system ??
     *  I cannot find SIOCIFCREATE/SIOCIFCREATE2/SIOCIFDESTROY in any where.
     *  thus, just use ifconfig command deal with it. Maybe someday i'll find.
     *  Note: 
     *      1.use ioctl(int fd, int cmd, ...);
     *      2.some other platform use devctl()/ioctl_sock()/...
     *  Rong Tao: 2019.02.25
     */
    if(Strncmp(VCM_IF_NAME, ifname, sizeof(VCM_IF_NAME)-1) != 0)
    {
        log_vcm_err("No this interface(%s - %s)\n", VCM_IF_NAME, ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        return VCM_ERR;
    }

    if(VCM_OK != is_ipv4_valid(VCM_REGULAR_IP, (CHAR *) ip, VCM_DoNothing))
    {
        log_vcm_err("ip is invalide. %s\n", ip);
        __vcm_set_errno(_VCM_EIIPERR);
        return VCM_ERR;
    }

    if(VCM_OK != is_ipv4_valid(VCM_NETMASK, (CHAR *) netmask, VCM_DoNothing))
    {
        log_vcm_err("ip is invalide. %s\n", ip);
        __vcm_set_errno(_VCM_EINETMASKERR);
        return VCM_ERR;
    }
    
    return System("ifconfig %s %s netmask %s", ifname, ip, netmask);
}

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 net_if_destroy(CONST CHAR *ifname)
{
    log_vcm_debug();

    if(ifname == NULL)
    {
        log_vcm_err("NULL pointer Error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    /**
     *  Use ifconfig+system ??
     *  I cannot find SIOCIFCREATE/SIOCIFCREATE2/SIOCIFDESTROY in any where.
     *  thus, just use ifconfig command deal with it. Maybe someday i'll find.
     *  Note: 
     *      1.use ioctl(int fd, int cmd, ...);
     *      2.some other platform use devctl()/ioctl_sock()/...
     *  Rong Tao: 2019.02.25
     */
    if(Strncmp(VCM_IF_NAME, ifname, sizeof(VCM_IF_NAME)-1) != 0)
    {
        log_vcm_err("No this interface(%s - %s)\n", VCM_IF_NAME, ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        return VCM_ERR;
    }
    
    return System("ifconfig %s down", ifname);
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 *  Example:
 *
```c
CHAR hdaddr[VCM_HDADDR_LEN];
CHAR ifname[VCM_STR_LEN] = {"virbr0"};
get_if_hardware_addr(hdaddr, ifname);
log_vcm_info("#########%s\n", hdaddr);
```
 **********************************************************************************************************************/
INT32 get_if_hardware_addr(CONST CHAR *ifname, CHAR hdaddr[VCM_HDADDR_LEN])
{
    if(ifname == NULL)
    {
        log_vcm_err("ifname NULL pointer error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    
    INT sockfd;
	struct ifreq ifr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        log_vcm_err("Socket create error. %s\n", ifname);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
	Strcpy(ifr.ifr_name, ifname);

	if(Ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
	{
        log_vcm_err("Ioctl SIOCGIFHWADDR error. %s\n", ifname);
	    __vcm_set_errno(_VCM_ENIFNOEXIST);
        Close(sockfd);
        return VCM_ERR;
	}

    Memset(hdaddr, 0, VCM_HDADDR_LEN);  
    /**
     *  "00:0c:29:73:ad:23" VS "000c2973ad23" 
     *  because 
     *  "%02x:%02x:%02x:%02x:%02x:%02x" VS "%02x%02x%02x%02x%02x%02x"
     */
    Sprintf(hdaddr, "%02x%02x%02x%02x%02x%02x",
                    (unsigned char)ifr.ifr_hwaddr.sa_data[0], 
                    (unsigned char)ifr.ifr_hwaddr.sa_data[1], 
                    (unsigned char)ifr.ifr_hwaddr.sa_data[2], 
                    (unsigned char)ifr.ifr_hwaddr.sa_data[3], 
                    (unsigned char)ifr.ifr_hwaddr.sa_data[4], 
                    (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    hdaddr[VCM_HDADDR_LEN-1] = '\0';
    Close(sockfd);

    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;

}


/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 get_if_inet_addr(CONST CHAR *ifname, CHAR inet_addr[VCM_IP_LEN])
{
    log_vcm_debug();
    if(ifname == NULL)
    {
        log_vcm_err("ifname NULL pointer error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    INT sockfd;
	struct ifreq ifr;
    struct sockaddr_in *addr;


	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        log_vcm_err("Socket create error. %s\n", ifname);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
	Strcpy(ifr.ifr_name, ifname);

    if(Ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
    {
        log_vcm_err("Get inet interface IP adderss error. %s\n", ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        Close(sockfd);
        return VCM_ERR;
    }

    addr = (struct sockaddr_in*)&(ifr.ifr_addr);

    Strcpy(inet_addr, Inet_ntoa(addr->sin_addr));


    Close(sockfd);

    __vcm_set_errno(_VCM_ESUCCESS);
    
    return VCM_OK;
}

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 set_if_inet_addr(CONST CHAR *ifname, CHAR inet_addr[VCM_IP_LEN])
{
    log_vcm_debug();
    if(ifname == NULL)
    {
        log_vcm_err("ifname NULL pointer error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    INT sockfd;
	struct ifreq ifr;
    struct sockaddr_in *addr = Malloc(sizeof(struct sockaddr_in));


	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        log_vcm_err("Socket create error. %s\n", ifname);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
	Strcpy(ifr.ifr_name, ifname);
    
    addr->sin_family = AF_INET;
    Inet_aton(inet_addr, &(addr->sin_addr));
    Memcpy(&(ifr.ifr_addr), addr, sizeof(struct sockaddr_in));

    if(Ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
    {
        log_vcm_err("Set inet interface IP adderss error. %s\n", ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        Close(sockfd);
        return VCM_ERR;
    }
    
    Close(sockfd);

    __vcm_set_errno(_VCM_ESUCCESS);
    
    return VCM_OK;    
}




/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 get_if_netmask(CONST CHAR *ifname, CHAR netmask[VCM_IP_LEN])
{
    log_vcm_debug();
    if(ifname == NULL)
    {
        log_vcm_err("ifname NULL pointer error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    INT sockfd;
	struct ifreq ifr;
    struct sockaddr_in *addr;


	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        log_vcm_err("Socket create error.%s\n", ifname);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
	Strcpy(ifr.ifr_name, ifname);

    if(Ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0)
    {
        log_vcm_err("Get netmask error. %s\n", ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        Close(sockfd);
        return VCM_ERR;
    }

    addr = (struct sockaddr_in*)&(ifr.ifr_addr);

    Strcpy(netmask, Inet_ntoa(addr->sin_addr));


    Close(sockfd);

    __vcm_set_errno(_VCM_ESUCCESS);
    
    return VCM_OK;
}

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 set_if_netmask(CONST CHAR *ifname, CHAR netmask[VCM_IP_LEN])
{
    log_vcm_debug();
    if(ifname == NULL)
    {
        log_vcm_err("ifname NULL pointer error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    INT sockfd;
	struct ifreq ifr;
    struct sockaddr_in *addr = Malloc(sizeof(struct sockaddr_in));


	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        log_vcm_err("Socket create error. %s\n", ifname);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
	Strcpy(ifr.ifr_name, ifname);
    
    addr->sin_family = AF_INET;
    Inet_aton(netmask, &(addr->sin_addr));
    Memcpy(&(ifr.ifr_addr), addr, sizeof(struct sockaddr_in));

    if(Ioctl(sockfd, SIOCSIFNETMASK, &ifr) < 0)
    {
        log_vcm_err("Set netmask error. %s\n", ifname);
        __vcm_set_errno(_VCM_ENIFNOEXIST);
        Close(sockfd);
        return VCM_ERR;
    }
    
    Close(sockfd);

    __vcm_set_errno(_VCM_ESUCCESS);
    
    return VCM_OK; 
}


/***********************************************************************************************************************
 * Description: reject character from string  
 *
 * Input:   str: string 
 *          ch: character you wanna reject from string
 * Output: return:  success: 0
 *                  error:   -1
 *         output: MAC address string
 **********************************************************************************************************************/
INT32 strstripc(CHAR *str, CONST CHAR ch)
{    
    if((VOID*)str == NULL)    
    {        
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;    
    }    

    if(Strlen(str) <= 1)
    {
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR; 
    }

    if(!Ischar(ch))
    {
        log_vcm_err("Can not strip char from %s\n", (CHAR*)str);
        __vcm_set_errno(_VCM_EICHAR);
        return VCM_ERR; 
    }
    
    INT32 ilen, len = Strlen(str);  
    
    for(ilen=0; ilen < len; ilen++)    
    {        
        if(str[ilen] == ch)        
        {            
            INT i;            
            for(i=ilen; i<len-1; i++)            
            {                
                str[i] = str[i+1];            
            }            
            str[len-1] = '\0';            
            len--;
        }    
    }    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/**********************************************************************************************************************
 * Description: joint some vars into a string  
 *
 * Input:   dst: destination string 
 *          fmt: arguments format string
 *          ...: arguments
 * Output: return:  success: dst address pointer
 *                  error:   NULL
 **********************************************************************************************************************/
char *strjoint(char *dst, const char *fmt, ...)
{
    if(dst == NULL || fmt == NULL)
        return NULL;

    va_list args;
    va_start(args, fmt);

    Vsprintf(dst, fmt, args);

    va_end(args);

    return (char*)dst;
}


/***********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_pid_alive(CONST pid_t pid)
{
    char cmd[VCM_STR_LEN], line[VCM_STR_LEN];
    Memset(cmd, 0, VCM_STR_LEN);
    Memset(line, 0, VCM_STR_LEN);
    
    Sprintf(cmd, "ps -ef | grep %d | awk '{print $2}'", pid);
    FILE *fp = Popen(cmd, "r");
    while(Fgets(line, VCM_STR_LEN, fp) != NULL)
    {
        int pid_tmp = Atoi(line);
        if(pid == pid_tmp)
        {
            Fclose(fp);
            return VCM_OK;
        }
    }
    Fclose(fp);
    return VCM_ERR;
}

/***********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_pname_alive(CONST CHAR *pname)
{
    if(pname == NULL || Strlen(pname) <= 1)
    {
        log_vcm_err("NULL pointer error.");
        return VCM_ERR;
    }

    char cmd[VCM_STR_LEN], line[VCM_STR_LEN];
    Memset(cmd, 0, VCM_STR_LEN);
    Memset(line, 0, VCM_STR_LEN);
    
    Sprintf(cmd, "ps -e | grep %s | awk '{print $4}'", pname);
    FILE *fp = Popen(cmd, "r");
    while(Fgets(line, VCM_STR_LEN, fp) != NULL)
    {
        strstripc(line, '\n');
        if(Strcmp(pname, line) == 0)
        {
            Fclose(fp);
            return VCM_OK;
        }
    }
    Fclose(fp);
    return VCM_ERR;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
Sem_t VCM_SemCreate()
{
    Sem_t ret = (Sem_t)Malloc(sizeof(sem_t));
    if(sem_init((sem_t *)ret, 0, 1) != 0)
	{
		log_vcm_err("sem_init error\n");
        return 0;
	}
    return (Sem_t)ret;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_set_display(FILE *fp, vcm_set set, int start, int end, CONST CHAR flag)
{
    /* we use VCM_CELL_NUM not VCM_SETSIZE to show the VCM_SET */

    UINT32 iset = 0, ret = 0, count = 0;
    Fprintf(fp, "[");
    for(iset = start; iset < end; iset++)
    {
        ret = VCM_ISSET(iset, &set); 
        
        if(ret == 1)
        {
            count++;
            Fprintf(fp, "%s%c%s",VCM_ANSI_RED, flag, VCM_ANSI_RESET);
        }
        else if(ret == 0)
            Fprintf(fp, "-");
        
    }  
    Fprintf(fp, "]");

    return count;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 log_vcm(FILE *fp, LOG_VCM_TYPE type, CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...)
{
    /**
     *  Check log type valiable
     */
    if(type <= VCM_LOG_TYPE_MIN || type >= VCM_LOG_TYPE_MAX)
    {
        va_list arg1;
        va_start(arg1, fmt);
        
        Vfprintf(fp, fmt, arg1);
        
        va_end(arg1);
        
        return VCM_WARN;
    }

    /**
     *  Switch the log type
     */
    switch(type)
    {
        case VCM_LOG_ERROR:
            Fprintf(fp, "  %s[Error][vcm]%s %s:%s:%d ", VCM_ANSI_RED, VCM_ANSI_RESET, __file, __func, __line);
            break;
        
        case VCM_LOG_INFO:
            Fprintf(fp, "   %s[Info][vcm]%s %s:%s:%d ", VCM_ANSI_BLUE, VCM_ANSI_RESET, __file, __func, __line);
            break;
            
        case VCM_LOG_SUCCESS:
            Fprintf(fp, "%s[Success][vcm]%s %s:%s:%d ", VCM_ANSI_GREEN, VCM_ANSI_RESET, __file, __func, __line);
            break;
        
        case VCM_LOG_WARNING:
            Fprintf(fp, "%s[Warning][vcm]%s %s:%s:%d ", VCM_ANSI_PURPLE, VCM_ANSI_RESET, __file, __func, __line);
            break;
        
        case VCM_LOG_DEBUG:
            Fprintf(fp, "  %s[Debug][vcm]%s %s:%s:%d ", VCM_ANSI_GRAY, VCM_ANSI_RESET, __file, __func, __line);
            break;

        case VCM_LOG_SIMPLE:
            //Do nothing
            break;
        default:
            //TODO
            break;
    }

    /**
     *  log
     */
    va_list arg;
    va_start(arg, fmt);

    Vfprintf(fp, fmt, arg);
    
    va_end(arg);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description: parse the doc.
 *
 * Input: szdocname: doc name;
 *
 * Output:  return -1,failed;
 *          return 0, success
 **********************************************************************************************************************/
xmlDocPtr xml_doc(CHAR *szDocName)
{
    xmlDocPtr  doc = xmlReadFile(szDocName, "utf-8", XML_PARSE_RECOVER);
    if (doc == NULL)
    {
    	log_vcm_err("doc is null\n");
        return NULL;
    }
    return doc;
}

/***********************************************************************************************************************
 * Description: get the context of the doc.
 *
 * Input: szdocname: doc name;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
xmlXPathContextPtr xml_context(xmlDocPtr doc)
{
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context ==  NULL)
    {
    	log_vcm_err("context is null\n");
        return NULL;
    }
    return context;
}

/***********************************************************************************************************************
 * Description: alter the node value in xml files.
 *
 * Input: context: file context;
 *          xpath: node path;
 *        content: the value of node;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
INT32 alter_xml_node_value(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *content)
{
    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath, context);
    if (result == NULL)
    {
    	log_vcm_err("NULL pointer\n");
        return VCM_ERR;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        log_vcm_err("nodeset:%s is empty\n",xpath);
        return VCM_ERR;
    }
    xmlNodeSetContent(result->nodesetval->nodeTab[0], content);
    
    return VCM_OK;
}


/************************************************************************************************************************
 * alter the node value in xml files.
 *
 * Input: context: file context;
 *        xpath: node path;
 *        content: the value of node;
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
INT32 alter_xml_node_value_next(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *content, INT32 index)
{
	xmlXPathObjectPtr result;
	result = xmlXPathEvalExpression(xpath, context);
	if (result == NULL)
	{
		log_vcm_err("result is null\n");
		return VCM_ERR;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
        xmlXPathFreeObject(result);
        log_vcm_err("nodeset is empty\n");
        return VCM_ERR;
	}
	xmlNodeSetContent(result->nodesetval->nodeTab[index], content);
	return VCM_OK;
}

/***********************************************************************************************************************
 * Description: alter the node value in xml files.
 *
 * Input: context: file context;
 *          xpath: node path;
 *        content: the value of node;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
xmlChar* get_xml_node_value(xmlXPathContextPtr context, CONST xmlChar *xpath)
{
    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath, context);
    if (result == NULL)
    {
    	log_vcm_err("result is null\n");
        return NULL;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        log_vcm_err("nodeset is empty\n");
        return NULL;
    }
    
    return xmlNodeGetContent(result->nodesetval->nodeTab[0]);;
}

/***********************************************************************************************************************
 * Description: 
 *
 * Input: 
 * 
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
INT32 alter_xml_node_attr(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *attr)
{
    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath,context);
    if (result == NULL)
    {
    	log_vcm_err("result is null\n");
        return VCM_ERR;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        log_vcm_err("nodeset:%s is empty\n",xpath);
        return VCM_ERR;
    }
    if (result->nodesetval->nodeTab[0]->children != NULL )
	{
		result->nodesetval->nodeTab[0]->children->name = attr;
	}
	else
	{
		result->nodesetval->nodeTab[0]->children->next->name = attr;
	}
    //result->nodesetval->nodeTab[0]->name = attr;

    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 * 
 * Output:  return -1,failed;
 *          return 0, success
 ***********************************************************************************************************************/
INT32 alter_xml_node_attr_next(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *attr)
{
	xmlXPathObjectPtr result;
	result = xmlXPathEvalExpression(xpath,context);
	if (result == NULL)
	{
		log_vcm_err("result is null\n");
		return VCM_ERR;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
  		xmlXPathFreeObject(result);
  		log_vcm_err("nodeset is empty\n");
		return VCM_ERR;
	}
    if (result->nodesetval->nodeTab[0]->children != NULL )
    {
    	result->nodesetval->nodeTab[0]->children->next->name = attr;
    }
	
	return VCM_OK;
}



/***********************************************************************************************************************
 * Description: save the  xml files.
 *
 * Input: szdocname: file name;
 *          doc: the pointer to doc name;
 *          context:the doc context;
 *
 * Output: return -1,failed;
 *          return 0, success
 *          
 ***********************************************************************************************************************/
INT32 save_xml(CHAR *szDocName, xmlDocPtr doc, xmlXPathContextPtr context)
{
    /* no return funcs */
    xmlXPathFreeContext(context);
    xmlSaveFile(szDocName, doc);
    
    return VCM_OK;
}


