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
  }
  else
  {
    nume_director = director_intrare;
  }

  sprintf(buffer, "Nume director: %s\n", nume_director);
  write(fd, buffer, strlen(buffer));
  
  uid_t user_id = getuid();
  sprintf(buffer, "Identificatorul utilizatorului: %d\n", user_id);
  write(fd, buffer, strlen(buffer));
  
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
}

void link_verif(char *nume_fisier, int fd)
{
  struct stat file_info;

  if (lstat(nume_fisier, &file_info) != 0)
  {
    perror("Eroare la obtinmerea informatiilor despre fisier");
    return;
  }

  if (S_ISLNK(file_info.st_mode))
  {
    afisare_legatura_simbolica(nume_fisier, &file_info, fd);
  }
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

  int fd = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("Eroare la deschiderea fisierului statistica.txt");
    return 2;
  }

  if (stat(director_intrare, &dir_info) == 0)
  {
    if (S_ISDIR(dir_info.st_mode))
    {
      char buffer[10000];
      write(fd, buffer, strlen(buffer));
    }
    else
    {
      char buffer[10000];
      write(fd, buffer, strlen(buffer));
    }
  }
  else
  {
    perror("Eroare la obtinerea informatiilor despre director");
    close(fd);
    return 3;
  }

  DIR *dir = opendir(director_intrare);
  if (dir == NULL)
  {
    perror("Eroare la deschiderea directorului");
    close(fd);
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

      if (lstat(path, &file_info) == 0)
      {
        if (S_ISLNK(file_info.st_mode))
        {
          char buffer[10000];
          sprintf(buffer, "\n Legatura simbolica: %s\n", fisier_intrare);
          write(fd, buffer, strlen(buffer));
          link_verif(path, fd);
        }
        else if (S_ISDIR(file_info.st_mode))
        {
          char buffer[10000];
          sprintf(buffer, "\n Director: %s\n", fisier_intrare);
          write(fd, buffer, strlen(buffer));
          printare_detalii_director(path, &file_info, fd);
        }
        else if (S_ISREG(file_info.st_mode))
        {
          if (strstr(fisier_intrare, ".bmp") != NULL)
          {
            char buffer[10000];
            sprintf(buffer, "\n Fisier BMP: %s\n", fisier_intrare);
            write(fd, buffer, strlen(buffer));
            printare_detalii(path, &file_info, fd);
            drepturi_acces(path, &file_info, fd);
          }
          else
          {
            char buffer[10000];
            sprintf(buffer, "\n Fisier: %s\n", fisier_intrare);
            write(fd, buffer, strlen(buffer));
            printare_detalii_fisier_obisnuit(path, &file_info, fd);
            drepturi_acces(path, &file_info, fd);
          }
        }
      }
    }
  }

  closedir(dir);
  close(fd);

  return 0;
}
