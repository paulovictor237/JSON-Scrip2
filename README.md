# Habilitar Linux para Windows 

1.  Abra o PowerShell como **administrador** e digite o código abaixo. Depois de digitar o código espere concluir o comando.
2.  Feche o PowerShell e reinicie o computador .

```
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

# Instalar a distribuição do Linux

1. Instale o [WGS](https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6#activetab=pivot:overviewtab) (Ubuntu para o Windows)
2. Abra o Ubuntu e aguarde terminar a instalação
3. Crie um nome de usuário e de enter ( de preferencia o mesmo do windows para você lembrar)
4. Crie uma senha e de enter ( de preferencia o mesmo do windows para você lembrar)
5. Digite os códigos abaixo e aguarde a instalação, em seguida pode fechar o Ubuntu. 
   (Clique com o botão direito para colar o comando)

```
sudo apt update #atualiza os pacotes do linux
```

```
sudo apt install g++ #instala o compilador c++
```

# Como usar o Script

1. Cole na pasta "file_in" os arquivos "My_Job.src" e "My_Job .dat"
2. Também é possível criar o mapa da garra no arquivo "Garra.csv"
3. Após a execução, os arquivos serão gerados na pasta "file_out"
4. Na pasta "relatório" será gerado o relatório da execução do script.

# Executando o Script

1. Na pasta do Script aperte `shift`+ `botão direito do mouse` e clique em "**abrir o shell do linux aqui**" 

2. Para executar o programa digite o comando abaixo.

```
 ./script_exe
```
