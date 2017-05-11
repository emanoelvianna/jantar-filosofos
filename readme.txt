##  jantar dos filósofos: 

comando de compilação:
gcc jantar_dos_filosofos.c -o main -lpthread
comando de execução:
./main <quantidade_filosofos> <quantidade_tempo>

##  cliente e servidor: 

comando de compilação:
gcc servidor.c -o servidor -lpthread
comando de execução:
./servidor

comando de compilação:
gcc cliente.c -o cliente -lpthread
comando de execução:
./cliente <mensagem_string>
exemplo de comando de execução:
./cliente "Hello. How are you?"
