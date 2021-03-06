#include "ps4.h"
#include "patch.h"




int nthread_run;
char notify_buf[1024];


void systemMessage(char* msg) {
 	char buffer[512]; 
 	sprintf(buffer, "%s\n\n\n\n\n\n\n", msg);
 	sceSysUtilSendSystemNotificationWithText(0x81, buffer);
}


void copy_File(char *sourcefile, char* destfile)
{
    int src = open(sourcefile, O_RDONLY, 0);
    if (src != -1)
    {
        int out = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (out != -1)
        {
             size_t bytes;
             char *buffer = malloc(4194304);
             if (buffer != NULL)
             {
                 while (0 < (bytes = read(src, buffer, 4194304)))
                     write(out, buffer, bytes);
                     free(buffer);
             }
             close(out);
         }
         else {
         }
         close(src);
     }
     else {
     }
}


void copy_Dir(char*sourcedir, char* destdir) //char
{
    DIR *dir;
    struct dirent *dp;
    struct stat info;
    char src_path[1024], dst_path[1024];
    dir = opendir(sourcedir);
    if (!dir)
        return;
        mkdir(destdir, 0777);
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {}
        else
        {
            sprintf(src_path, "%s/%s", sourcedir, dp->d_name);
            sprintf(dst_path, "%s/%s", destdir  , dp->d_name);

            if (!stat(src_path, &info))
            {
                if (S_ISDIR(info.st_mode))
                {
                  copy_Dir(src_path, dst_path);
                }
                else
                if (S_ISREG(info.st_mode))
                {
                  copy_File(src_path, dst_path);
                }
            }
        }
    }
    closedir(dir);
}



void *nthread_func(void *arg)
{
        time_t t1, t2;
        t1 = 0;
	while (nthread_run)
	{
		if (notify_buf[0])
		{
			t2 = time(NULL);
			if ((t2 - t1) >= 15)
			{
				t1 = t2;
                systemMessage(notify_buf);
			}
		}
		else t1 = 0;
		sceKernelSleep(1);
	}

	return NULL;
}



int _main(struct thread *td) {
      
	  initKernel();
      initLibc();
      initPthread();
      syscall(11,patcher,td);
      initSysUtil();
      nthread_run = 1;
		
      notify_buf[0] = '\0';
      ScePthread nthread;
      scePthreadCreate(&nthread, NULL, nthread_func, NULL, "nthread");
     
	  systemMessage("Welcome to Avatar Injector by Red-J"); 
	
	

	int usbdir = open("/mnt/usb0/.dirtest", O_WRONLY | O_CREAT | O_TRUNC, 0777);
         if (usbdir == -1)
            {
                usbdir = open("/mnt/usb1/.dirtest", O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (usbdir == -1)
                {
                       	
                        systemMessage("You must insert USB key...\n\n Bye");
                        nthread_run = 0;
                        return 0;
                }
                else
                {
                        close(usbdir);
                     
          		}
			}
			
		
            else
            {           
                        close(usbdir);
                        //
						DIR * reps = opendir("/mnt/usb0/Avatar_Inject");
     	
						 if (reps != NULL)
    					{
        				struct dirent * ents;
						char srcpath1[256];
						char srcpath2[256];
						char srcpath3[256];
						char srcpath4[256];
						char srcpath5[256];
							
      									while ((ents = readdir(reps)) != NULL)
										{

											if (strcmp(ents->d_name, ".") != 0 && strcmp(ents->d_name, "..") != 0) 
												{	
           										sprintf(srcpath1, "/mnt/usb0/Avatar_Inject/%s/avatar.png",ents->d_name);
												sprintf(srcpath2, "/mnt/usb0/Avatar_Inject/%s/avatar64.dds",ents->d_name);
												sprintf(srcpath3, "/mnt/usb0/Avatar_Inject/%s/avatar128.dds",ents->d_name);
												sprintf(srcpath4, "/mnt/usb0/Avatar_Inject/%s/avatar260.dds",ents->d_name);
												sprintf(srcpath5, "/mnt/usb0/Avatar_Inject/%s/avatar440.dds",ents->d_name);
												}
						
										}
						
							
						
						DIR * repd = opendir("/mnt/usb0/Avatar_Inject");
     	
						 if (repd != NULL)
    					{
        				struct dirent * entd;
						char destpath1[256];
						char destpath2[256];
						char destpath3[256];
						char destpath4[256];
						char destpath5[256];
      								
									while ((entd = readdir(repd)) != NULL)
										{

											if (strcmp(entd->d_name, ".") != 0 && strcmp(entd->d_name, "..") != 0) 
												{	
           										sprintf(destpath1, "/system_data/priv/cache/profile/%s/avatar.png",entd->d_name);
												sprintf(destpath2, "/system_data/priv/cache/profile/%s/avatar64.dds",entd->d_name);
												sprintf(destpath3, "/system_data/priv/cache/profile/%s/avatar128.dds",entd->d_name);
												sprintf(destpath4, "/system_data/priv/cache/profile/%s/avatar260.dds",entd->d_name);
												sprintf(destpath5, "/system_data/priv/cache/profile/%s/avatar440.dds",entd->d_name);
												}
						
										}
						
    				     
						systemMessage("Reading from USB0");
                        unlink("/mnt/usb0/.dirtest");
			
						 
						copy_File(srcpath1,destpath1);
                        copy_File(srcpath2,destpath2);
						copy_File(srcpath3,destpath3);
						copy_File(srcpath4,destpath4);
						copy_File(srcpath5,destpath5);
			       
						closedir(reps);
						closedir(repd);
						
						sprintf(notify_buf, "Copying: User Data\nPlease wait.");
							
                        
                        notify_buf[0] = '\0';
                        nthread_run = 0;
                        systemMessage("USB Injection Completed ;)");
						
						}
            			}
			}
    
   return 0; 
}

            




