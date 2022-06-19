#ifndef _SCANFILE_
#define _SCANFILE_
struct scanList{
   char **namesFiles;
   char **namesDirs;
   char **namesPwd;
   char **namesVolumes;
   char *directory;
   int countFiles;
   int countDirs;
   int countPwd;
   int countVol;
};

struct scanList *scan_dir(char *dir,char *filter);
int free_dir(struct scanList *scan);
#endif
