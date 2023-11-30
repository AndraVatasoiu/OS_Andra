#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct {
  int inaltime;
  int lungime;
} BMP_header;

void drepturi_acces(char *nume_fisier)
{
  struct stat file_info;

  if(stat(nume_fisier, &file_info)!=0)
    {

      perror("Eroare la obtinmerea informatiilor despre fisier");
      return;
    }

  printf("Drepturi de acces user: ");
  // sprintf(buffer + strlen(buffer),"Drepturi de acces user: ");
  printf((file_info.st_mode & S_IRUSR) ? "R" : "-");
  // sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IRUSR) ? "R" : "-");
  printf((file_info.st_mode & S_IWUSR) ? "W" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWUSR) ? "W" : "-");
  printf((file_info.st_mode & S_IXUSR) ? "X" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXUSR) ? "X" : "-");
  printf("\n");
  //sprintf(buffer + strlen(buffer), "\n");

  printf("Drepturi de acces grup: ");
  //sprintf(buffer + strlen(buffer),"Drepturi de acces grup: ");
  printf((file_info.st_mode & S_IRGRP) ? "R" : "-");
  printf((file_info.st_mode & S_IWGRP) ? "W" : "-");
  printf((file_info.st_mode & S_IXGRP) ? "X" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IRGRP) ? "R" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWGRP) ? "W" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXGRP) ? "X" : "-");
  //sprintf(buffer + strlen(buffer), "\n");
  printf("\n");

  //sprintf(buffer + strlen(buffer), "Drepturi de acces altii: ");
  printf("Drepturi de acces altii: ");
  printf((file_info.st_mode & S_IROTH) ? "R" : "-");
  printf((file_info.st_mode & S_IWOTH) ? "W" : "-");
  printf((file_info.st_mode & S_IXOTH) ? "X" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IROTH) ? "R" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWOTH) ? "W" : "-");
  //sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXOTH) ? "X" : "-");
  //sprintf(buffer + strlen(buffer), "\n");
  printf("\n");

  //printf("%s",buffer);
}

void printare_detalii(char *fisier_intrare)
{
  BMP_header header;

  int fd=open(fisier_intrare,O_RDONLY);
	  if(fd==-1)
	    {
	      perror("Eroare la deschiderea fisierului BMP");
	      //continue;
	    }
  struct stat file_info;

  if(stat(fisier_intrare, &file_info)!=0)
    {

      perror("Eroare la obtinerea informatiilor despre fisier");
      return;
    }
          lseek(fd,18,SEEK_SET);
	  read(fd,&header.lungime,sizeof(header.lungime));

	  lseek(fd,22,SEEK_SET);
	  read(fd,&header.inaltime,sizeof(header.inaltime));

          printf("Inaltime: %d\n",header.inaltime);
          printf("Lungime: %d\n",header.lungime);
	  printf("Dimensiune: %ld bytes\n",file_info.st_size);
	  uid_t user_id=getuid();
	  printf("Identificatorul utilizatorului: %d\n", user_id);
	  printf("Timpul ultimei modificari: %s",ctime(&file_info.st_mtime));
	  printf("Contorul de legaturi: %ld\n", file_info.st_nlink);
}


int main(int argc, char *argv[]) 
{
  if (argc != 2)
  {
    printf("Usage ./p <fisier_intrare\n");
    return 1;
  }

  char *director_intrare = argv[1];
  struct stat dir_info;

  if (stat(director_intrare, &dir_info) == 0)
  {
    if (S_ISDIR(dir_info.st_mode))
    {
      printf("%s este un director\n\n", director_intrare);
    }
    else
    {
      printf("%s nu este un director\n", director_intrare);
    }
  }
  else
  {
    perror("Eroare la obtinerea informatiilor despre director");
    return 2;
  }

  DIR *dir = opendir(director_intrare);
  if (dir == NULL)
  {
    perror("Eroare la deschiderea directorului");
    return 3;
  }

  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL)
  {
    char *fisier_intrare = entry->d_name;
    if (strcmp(fisier_intrare, ".") != 0 && strcmp(fisier_intrare, "..") != 0)
    {
      char path[1024];
      snprintf(path, sizeof(path), "%s/%s", director_intrare, fisier_intrare);

      struct stat file_info;
      if (stat(path, &file_info) != 0)
      {
        perror("Eroare la obtinerea datelor despre fisier");
        continue;
      }

      if (S_ISDIR(file_info.st_mode))
      {
        printf("\n Director: %s\n", fisier_intrare);
	
	
	//	drepturi_acces(fisier_intrare);
      }
      else if (S_ISREG(file_info.st_mode))
      {
        if (strstr(fisier_intrare, ".bmp") != NULL)
	{
          printf("\n Fisier BMP: %s\n", fisier_intrare);

	  /*int fd=open(path,O_RDONLY);
	  if(fd==-1)
	    {
	      perror("Eroare la deschiderea fisierului BMP");
	       continue;
	       }*/

	  // BMP_header header;
	  /* if(read(fd, &header, sizeof(BMP_header))!=sizeof(BMP_header))
	    {
	      perror("Eroare la citirea headerului BMP");
	      close(fd);
	      continue;
	      }*/
	  /*lseek(fd,18,SEEK_SET);
	  read(fd,&header.lungime,sizeof(header.lungime));

	  lseek(fd,22,SEEK_SET);
	  read(fd,&header.inaltime,sizeof(header.inaltime));

          printf("Inaltime: %d\n",header.inaltime);
          printf("Lungime: %d\n",header.lungime);
	  printf("Dimensiune: %ld bytes\n",file_info.st_size);
	  uid_t user_id=getuid();
	  printf("Identificatorul utilizatorului: %d\n", user_id);
	  printf("Timpul ultimei modificari: %s",ctime(&file_info.st_mtime));
	  printf("Contorul de legaturi: %ld\n", file_info.st_nlink);*/
	  printare_detalii(fisier_intrare);
	  drepturi_acces(fisier_intrare);
	       
        }
	else
	{
          printf("\n Fisier: %s\n", fisier_intrare);
	  printare_detalii(fisier_intrare);
	  drepturi_acces(fisier_intrare);
	       
        }
      }
      else if (S_ISLNK(file_info.st_mode))
      {
        printf("\n Legatura simbolica: %s\n", fisier_intrare);
	drepturi_acces(fisier_intrare);
      }
    }
  }

  closedir(dir);

  return 0;
}
