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
#include <sys/wait.h>

int line_count = 5;

typedef struct
{
  int inaltime;
  int lungime;
} BMP_header;

void drepturi_acces(char *nume_fisier, struct stat *file_info, int fd)
{
  char buffer[10000];
  sprintf(buffer, "Drepturi de acces user: %s%s%s\n",
         (file_info->st_mode & S_IRUSR) ? "R" : "-",
         (file_info->st_mode & S_IWUSR) ? "W" : "-",
         (file_info->st_mode & S_IXUSR) ? "X" : "-");
  write(fd, buffer, strlen(buffer));

  sprintf(buffer, "Drepturi de acces grup: %s%s%s\n",
         (file_info->st_mode & S_IRGRP) ? "R" : "-",
         (file_info->st_mode & S_IWGRP) ? "W" : "-",
         (file_info->st_mode & S_IXGRP) ? "X" : "-");
  write(fd, buffer, strlen(buffer));

  sprintf(buffer, "Drepturi de acces altii: %s%s%s\n",
         (file_info->st_mode & S_IROTH) ? "R" : "-",
         (file_info->st_mode & S_IWOTH) ? "W" : "-",
         (file_info->st_mode & S_IXOTH) ? "X" : "-");
  write(fd, buffer, strlen(buffer));
}

void printare_detalii_director(char *director_intrare, struct stat *dir_info, int fd)
{
  char buffer[10000];
  char *nume_director = strrchr(director_intrare, '/');
  if (nume_director != NULL)
    {
      nume_director++;
    } else
    {
      nume_director = director_intrare;
    }

  sprintf(buffer, "Nume director: %s\n", nume_director);
  write(fd, buffer, strlen(buffer));
  
  uid_t user_id = getuid();
  sprintf(buffer, "Identificatorul utilizatorului: %d\n", user_id);
  write(fd, buffer, strlen(buffer));
  line_count=5;
  drepturi_acces(director_intrare, dir_info, fd);
}

void printare_detalii(char *fisier_intrare, struct stat *file_info, int fd)
{
  BMP_header header;

  int bmp_fd = open(fisier_intrare, O_RDONLY);
  if (bmp_fd == -1)
    {
      perror("Eroare la deschiderea fisierului BMP");
      return;
    }
  
  char buffer[10000];
  char *fis = strrchr(fisier_intrare, '/');
  if (fis != NULL)
    {
      fis++;
    }
  else
    {
      fis = fisier_intrare;
    }
  sprintf(buffer, "Nume fisier BMP: %s\n", fis);
  write(fd, buffer, strlen(buffer));

  lseek(bmp_fd, 18, SEEK_SET);
  read(bmp_fd, &header.lungime, sizeof(header.lungime));

  lseek(bmp_fd, 22, SEEK_SET);
  read(bmp_fd, &header.inaltime, sizeof(header.inaltime));

  sprintf(buffer, "Inaltime: %d\n", header.inaltime);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Lungime: %d\n", header.lungime);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Dimensiune: %ld bytes\n", file_info->st_size);
  write(fd, buffer, strlen(buffer));
  
  uid_t user_id = getuid();
  sprintf(buffer, "Identificatorul utilizatorului: %d\n", user_id);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&file_info->st_mtime));
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Contorul de legaturi: %ld\n", file_info->st_nlink);
  write(fd, buffer, strlen(buffer));
  line_count=10;
  close(bmp_fd);
}

void gri(char *fisier_intrare, struct stat *file_info, int fd)
{
  BMP_header header;

  int bmp_fd = open(fisier_intrare, O_RDONLY);
  if (bmp_fd == -1)
    {
      perror("Eroare la deschiderea fisierului BMP");
      return;
    }

  lseek(bmp_fd, 18, SEEK_SET);
  read(bmp_fd, &header.lungime, sizeof(header.lungime));

  lseek(bmp_fd, 22, SEEK_SET);
  read(bmp_fd, &header.inaltime, sizeof(header.inaltime));

  unsigned char rgb[3];
  unsigned char aux;

  lseek(bmp_fd,54,SEEK_SET);
  int i=0, j=0;

  while(i<header.inaltime)
    {
      while(j<header.lungime)
	{
	  read(bmp_fd,rgb,sizeof(rgb));
	  lseek(bmp_fd,-3,SEEK_CUR);
	  aux=0.299*rgb[0]+0.587*rgb[1]+0.114*rgb[2];
	  write(bmp_fd,&aux,sizeof(unsigned char));
	  write(bmp_fd,&aux,sizeof(unsigned char));
	  write(bmp_fd,&aux,sizeof(unsigned char));
	  j++;
	}
      i++;
    }

  close(bmp_fd);
}

void printare_detalii_fisier_obisnuit(char *fisier_intrare, struct stat *file_info, int fd)
{
  int reg_fd = open(fisier_intrare, O_RDONLY);
  if (reg_fd == -1)
    {
      perror("Eroare la deschiderea fisierului BMP");
      return;
    }
  
  char buffer[10000];
  char *fis = strrchr(fisier_intrare, '/');
  if (fis != NULL)
    {
      fis++;
    }
  else
    {
      fis = fisier_intrare;
    }
  sprintf(buffer, "Nume fisier: %s\n", fis);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Dimensiune: %ld bytes\n", file_info->st_size);
  write(fd, buffer, strlen(buffer));
  
  uid_t user_id = getuid();
  sprintf(buffer, "Identificatorul utilizatorului: %d\n", user_id);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&file_info->st_mtime));
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Contorul de legaturi: %ld\n", file_info->st_nlink);
  write(fd, buffer, strlen(buffer));

  line_count=8;
  close(reg_fd);
}

void afisare_legatura_simbolica(char *nume_legatura, struct stat *file_info, int fd)
{ 
  char buffer[10000];
  char *leg = strrchr(nume_legatura, '/');
  if (leg != NULL)
    {
      leg++;
    }
  else
    {
      leg = nume_legatura;
    }
  sprintf(buffer, "Nume legatura: %s\n", leg);
  write(fd, buffer, strlen(buffer));
  
  sprintf(buffer, "Dimensiune: %ld bytes\n", file_info->st_size);
  write(fd, buffer, strlen(buffer));

  char target_path[1024];
  ssize_t target_size = readlink(nume_legatura, target_path, sizeof(target_path) - 1);
  if (target_size != -1)
    {
      target_path[target_size] = '\0';
      snprintf(buffer, sizeof(buffer), "Dimensiune fisier target: %ld bytes\n", file_info->st_size);
      write(fd, buffer, strlen(buffer));
    }

  drepturi_acces(nume_legatura, file_info, fd);

  line_count=8;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    {
      printf("Usage: %s <director_intrare> <director_iesire>\n", argv[0]);
      return 1;
    }
  
  char *director_intrare = argv[1];
  char *director_iesire = argv[2];

  DIR *dir = opendir(director_intrare);
  DIR *dir2=opendir(director_iesire);
  if (dir == NULL)
    {
      perror("Eroare la deschiderea directorului");
      return 4;
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
	  
	  pid_t child_pid = fork();
	  
	  if (child_pid == -1)
	    {
	      perror("Eroare la fork");
	      exit(1); 
	    }
	  
	  else if (child_pid == 0)
	    {
	      char output_filename[1024];
	      snprintf(output_filename, sizeof(output_filename), "%s/%s_statistica.txt", director_iesire, fisier_intrare);
	      
	      int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	      if (fd == -1)
		    {
		      perror("Eroare la deschiderea fisierului de iesire");
		      exit(EXIT_FAILURE);
		    }
	      
	      if (S_ISLNK(file_info.st_mode))
		{
		  afisare_legatura_simbolica(path, &file_info, fd);
		}
	      else if (S_ISDIR(file_info.st_mode))
		{
		  printare_detalii_director(path, &file_info, fd);
		  break;
		}
	      else if (S_ISREG(file_info.st_mode))
		{
		  if (strstr(fisier_intrare, ".bmp") != NULL)
		    {
		      pid_t pid2=fork();
		      if(pid2<0)
			{
			  printf("eroare la deschiderea fork2");
			  exit(1);
			}
		      else if(pid2==0)
			{
			  gri(path, &file_info,fd);
			  exit(99);
			}
		      else if(pid2>0)
			{
			  int status2;
			  waitpid(pid2,&status2,0);
			  printf("Procesul copil cu PID %d s-a incheiat cu codul %d\n",pid2 , WEXITSTATUS(status2));
			}
		      printare_detalii(path, &file_info, fd);
		      drepturi_acces(path, &file_info, fd);
		    }
		  else
		    {
		      printare_detalii_fisier_obisnuit(path, &file_info, fd);
		      drepturi_acces(path, &file_info, fd);
		    }
		  exit(line_count);  
		}
	    }
	  
	  else
	    {
	      int status1;
	      waitpid(child_pid,&status1,0);
	      printf("Procesul copil cu PID %d s-a incheiat cu codul %d\n",child_pid , WEXITSTATUS(status1));
	    }
	}
    }
  
  
  closedir(dir);
  closedir(dir2);

 return 0;
}
