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

//uint8_t header[54];

typedef struct{
  int inaltime;
  int lungime;
} BMP_header;



int main(int argc, char *argv[])
{
  if(argc!=2)
    {
      printf("Usage ./p <fisier_intrare\n");
      return 1;
    }
  char *fisier_intrare=argv[1];
  struct stat file_info;
  int ok_leg=0;
  //char *output="statistica.txt";
  char buffer[10000];

  if(stat(fisier_intrare, &file_info)==0)
    {

      ok_leg=1;
      char *file_extension=strchr(fisier_intrare,'.');
      if(file_extension!=NULL)
	{
	  if(strcmp(file_extension, ".bmp")==0)
	    {
	      sprintf(buffer,"%s este un fisier bmp\n\n",fisier_intrare);
	    }
	  else
	    {
	      sprintf(buffer,"%s nu este un fisier bmp\n",fisier_intrare);
	    }
     	}
      else
	{
	  sprintf(buffer,"Nu se poate determina tipul fisierului\n");
	}
    }
  else
    {
      sprintf(buffer,"Eroare la obtinerea informatiilor despre fisier\n");
    }
  
  int fd=open(fisier_intrare, O_RDONLY);
  if(fd==-1)
    {
      perror("Eroare la deschiderea fisierului de intrare\n");
      return 2;
    }
  else
    {
      sprintf(buffer + strlen(buffer), "Nume fisier: %s\n",fisier_intrare);
    }

  int fd_out=open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR| S_IRGRP | S_IROTH);
  if(fd_out==-1)
    {
      perror("Eroare la deschiderea fisierului de iesire");
      return 3;
    }

  BMP_header header;
  
    lseek(fd,18,SEEK_SET);
    read(fd, &header.lungime,sizeof(header.lungime));

    lseek(fd,22,SEEK_SET);
    read(fd,&header.inaltime,sizeof(header.inaltime));
  
    sprintf(buffer + strlen(buffer), "Inaltime: %d\n",header.inaltime);
    sprintf(buffer + strlen(buffer), "Lungime: %d\n",header.lungime);

    sprintf(buffer + strlen(buffer), "Dimensiune: %ld bytes\n",file_info.st_size);

  uid_t user_id=getuid();
  sprintf(buffer + strlen(buffer), "Identificatorul utilizatorului: %d\n",user_id);
  
  sprintf(buffer + strlen(buffer), "Timpul ultimei modificari: %s",ctime(&file_info.st_mtime));
  
  if(ok_leg==1)
    {
      sprintf(buffer + strlen(buffer), "Contorul de legaturi: %ld\n", file_info.st_nlink);
    }
 
  printf("Drepturi de acces user: ");
  sprintf(buffer + strlen(buffer),"Drepturi de acces user: ");
  printf((file_info.st_mode & S_IRUSR) ? "R" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IRUSR) ? "R" : "-");
  printf((file_info.st_mode & S_IWUSR) ? "W" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWUSR) ? "W" : "-");
  printf((file_info.st_mode & S_IXUSR) ? "X" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXUSR) ? "X" : "-");
  printf("\n");
  sprintf(buffer + strlen(buffer), "\n");

  sprintf(buffer + strlen(buffer),"Drepturi de acces grup: ");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IRGRP) ? "R" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWGRP) ? "W" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXGRP) ? "X" : "-");
  sprintf(buffer + strlen(buffer), "\n");

  sprintf(buffer + strlen(buffer), "Drepturi de acces altii: ");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IROTH) ? "R" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IWOTH) ? "W" : "-");
  sprintf(buffer + strlen(buffer), (file_info.st_mode & S_IXOTH) ? "X" : "-");
  sprintf(buffer + strlen(buffer), "\n");

  printf("%s",buffer);

  write(fd_out, buffer, strlen(buffer));

  /*if(write(fd_out, buffer, strlen(buffer))==-1)
    {
      perror("Eroare la scrierea in fisier");
      close(fd_out);
      return 4;
      }*/
  
  close(fd);
  close(fd_out);
  

  return 0;
}
