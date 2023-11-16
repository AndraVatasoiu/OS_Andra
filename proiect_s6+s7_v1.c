#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct{
    uint32_t fileSize;
    int32_t  imageWidth;
    int32_t  imageHeight;
    uint32_t imageSize;

}BMP;

DIR *opendirector(char *dirname) 
{
  DIR *director = opendir(dirname);
  if (director == NULL)
  {
    printf("Eroare la deschidere director!\n");
    exit(EXIT_FAILURE);
  }
  return director;
}
BMP *BMP_header;


int main(int argc, char *argv[])
{
  if(argc!=2)
    {
      printf("Usage ./p <fisier_intrare\n");
    }
  char *fisier_intrare=argv[1];
  struct stat file_info;

  if(stat(fisier_intrare, &file_info)==0)
    {
      char *file_extension=strchr(fisier_intrare,'.');
      if(file_extension!=NULL)
	{
	  if(strcmp(file_extension, ".bmp")==0)
	    {
	      printf("%s este un fisier bmp\n",fisier_intrare);
	    }
	  else
	    {
	      printf("%s nu este un fisier bmp\n",fisier_intrare);
	    }
     	}
      else
	{
	  printf("Nu se poate determina tipul fisierului\n");
	}
    }
  else
    {
      printf("Eroare la obtinerea informatiilor despre fisier\n");
    }
  /* int fd=open(fisier_intrare, O_RDONLY);
  if(fd==-1)
    {
      perror("Eroare la deschiderea fisierului de intrare\n");
    }
  else
  {
    printf("Nume fisier: %s\n",fisier_intrare);
    }*/

    
    DIR *director =opendirector(argv[1]);
    struct dirent *entry;


	struct stat info;
	stat(entry->d_name, &info);
	char sir[100];
	strcpy(sir,argv[1]);
	strcat(sir,"/");
	strcat(sir,entry->d_name);
	int file_descr=open(sir, O_RDONLY);
	if(strstr(entry->d_name,".bmp")!=NULL)
	  {
	    if(file_descr==-1)
	      {
		printf("Nu putem deschide fisierul\n");
	      }
	    else
	      {
		printf("Nume fisier: %s\n", entry->d_name); 

      lseek(file_descr, 18, SEEK_SET);                   
      read(file_descr, &BMP_header->imageWidth, sizeof(BMP_header->imageWidth)); 
      printf("Latime :%d pixeli\n", BMP_header->imageWidth);            

      lseek(file_descr, 22, SEEK_SET);                     
      read(file_descr, &BMP_header->imageHeight, sizeof(BMP_header->imageHeight)); 
      printf("Inaltime :%d pixeli\n", BMP_header->imageHeight);          

      printf("Dimensiune: %lu\n", info->st_size);

      printf("Identificatorul utilizatorului: %d\n",info->st_uid);

      printf("Contorul de legaturi: %lu\n", info->st_nlink);

      printf("Timpul ultimei modificari: %s", ctime(&info->st_mtime));

      printf("\n");
	      }
    
	  }

    

    //drepturi

    printf("Drepturi de acces user:");

  if (info->st_mode & S_IRUSR) 
  {
    printf(" R ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IWUSR) 
  {
    printf(" W ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IXUSR) 
  {
    printf(" X\n");
  }
  else
  {
    printf(" -\n");
  }
  printf("Drepturi de acces grup:");

  if (info->st_mode & S_IRGRP) 
  {
    printf(" R ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IWGRP) 
  {
    printf(" W ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IXGRP) 
  {
    printf(" X\n");
  }
  else
  {
    printf(" -\n");
  }
  printf("Drepturi de acces altii:");

  if (info->st_mode & S_IROTH) 
  {
    printf(" R ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IWOTH) 
  {
    printf(" W ");
  }
  else
  {
    printf(" - ");
  }
  if (info->st_mode & S_IXOTH) 
  {
    printf(" X\n");
  }
  else
  {
    printf(" -\n");
  }

    close(director);
   
  }

  return 0;
}
