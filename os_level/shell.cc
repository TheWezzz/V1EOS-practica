#include "shell.hh"

int main()
{ std::string input;
  std::string prompt = "";

{ int fd = syscall(SYS_open, "prompt", O_RDONLY, 0755);
  char byte[1];
  while(syscall(SYS_read, fd, byte, 1))
    prompt.append(byte); }

  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit



void new_file() // ToDo: Implementeer volgens specificatie.
{ std::string filename = "";
  std::string line = "";
  std::string text = "";

  std::cout << "Geef de bestandsnaam: " << std::endl;
  std::getline(std::cin, filename);
  
  std::cout << "Geef de tekst voor in " << filename << std::endl;
  while(std::getline(std::cin, line))
  { if(line == "<EOF>") break;
    else {text.append(line + "\n");} }
  
  const char* file = filename.c_str();
  const char* ctext = text.c_str();

  int fd = syscall(SYS_creat, file, 0755);
  //char byte(text.size() = {text};
  syscall(SYS_write, fd, ctext, text.size()); }



void list() // ToDo: Implementeer volgens specificatie.
{ int child = syscall(SYS_fork);
  if(child > 0){
    syscall(SYS_wait4, child, NULL, NULL, NULL);
  }else if(child == 0){
    const char* argumenten[4] = {"/bin/ls", "-l", "-a", NULL};
    syscall(SYS_execve, argumenten[0], argumenten, NULL); } }



void find() // ToDo: Implementeer volgens specificatie.
{ int fd[2];
  syscall(SYS_pipe, &fd);
  
  int child = syscall(SYS_fork);
  if(child > 0){
    int child2 = syscall(SYS_fork);
    
    if(child2 > 0){
      syscall(SYS_close, fd[0]);
    syscall(SYS_close, fd[1]);
      syscall(SYS_wait4, child2, NULL, NULL, NULL);
    }else if (child2 == 0){

      std::cout << "Geef een string: ";
      std::string input = "";
      std::getline(std::cin, input);
      const char* cinput = input.c_str();

      syscall(SYS_close, fd[1]);
      syscall(SYS_dup2, fd[0], 0);

      const char* argumenten[3] = {"/bin/grep", cinput, NULL};
      syscall(SYS_execve, argumenten[0], argumenten, NULL);
      }
    
    syscall(SYS_wait4, child, NULL, NULL, NULL);

  }else if(child == 0){
    syscall(SYS_close, fd[0]);
    syscall(SYS_dup2, fd[1], 1);

    const char* argumenten[3] = {"/usr/bin/find", ".", NULL};
    syscall(SYS_execve, argumenten[0], argumenten, NULL); } }



void seek() // ToDo: Implementeer volgens specificatie.
{ std::string seek = "seek";
  const char* cseek = seek.c_str();
  int fd = syscall(SYS_creat, cseek, 0755);
  
  char x[1] = {'x'};
  char niks[1] = {'\0'};

  syscall(SYS_write, fd, x, 1);
  syscall(SYS_lseek, fd, 5000000, SEEK_CUR);
  syscall(SYS_write, fd, x, 1);
  
  std::string loop = "loop";
  const char* cloop = loop.c_str();
  int fd2 = syscall(SYS_creat, cloop, 0755);
  
  syscall(SYS_write, fd2, x, 1);
  for(unsigned int i=0; i<5000000; i++){
    syscall(SYS_write, fd2, niks, 1);}
  syscall(SYS_write, fd2, x, 1); } 



void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
