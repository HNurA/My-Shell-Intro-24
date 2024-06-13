[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ZM_4BPDj)


# BIL 222 Project-1: My shell Intro 24


## Bilinmesi gerekenler



* File descriptors
* open/close/read/write system calls
* Process management
* Exec family system calls
* wait/fork system calls


## Öğrenme hedefleri

* Understanding child - parent process interactions, file descriptors, exec/wait cycles


## Detaylar (Single Shell)

Aşağıda tarif edildiği şekilde çalışan ve istenilen mesajları istenildiği şekilde yazdıran bir C programı yazınız:

**Not**: başlangıç olarak derste yaptığımız örneği alabilirsiniz.


1. Program başladıktan sonra sadece **$ göstermelidir (başka mesajlar yazdırmayınız.):**

    ```
    $ 
    ```

2. Sonra kullanıcının girmiş olduğu komutu yeni bir processi **fork** ile başlatarak** exec family** system call’la çalıştırınız ve bu process bitinceye kadar parent processin beklemesini sağlayınız.  
    1. Komutu çalıştırırken eğer kullanıcın girdiği komut bulunamadıysa yani exec fonksiyonu hata verirse:
        1. Bu hata path’ten kaynaklı olabileceği için, yine exec family fonksiyonu kullanarak aşağıdaki komutlardan birisi ile bu komutun pathini bulunuz. Mesela ‘ls’ bulunmak istenirse
           <code>
            $ which ls
            $ whereis ls
            $ command -v ls (not: alias oldugunda pathi vermiyor)
           </code>
        2. Eğer verilen program ismi current path de ise o zaman current path kullanılarak yine programı çalıştırınız.
           <code>
           $ pwd
           $ locate -w path/name 
           $ locate -b basename | fgrep -w path
           </code>
    2. Eğer current path de değilse ve sistem pathinde de tanımlı değilse yani her ikiside hata verdiyse, o zaman aşağıdaki şekilde hata mesajı yazdırınız. Mesela kullanıcının girdiği metin **name** ise ekran görüntüsü şu şekilde olmalıdır (satır sayılarına vesaire dikkat ediniz):

   ```
    $ name 
    name: command not found
    $
    ```


1. **Logging:T]m verilen komutlari** “log.txt” isminde bir dosyada time stamplerle loglayiniz. 

    Time1\t command1

    Time2\t command2

    Zaman için<code> [gettimeofday()](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) </code>system call kullanabilirsiniz.



## Derecelendirme

**Dosya okuma ve yazma işlemlerinde system calları kullanılması zorunludur. Yani fopen vs kullanamazsınız. Programınızın yukarıda istenilenlere göre olması gerekmektedir. **

1. 20% 
2. 50%
3. ve 4 30%

-%10a kadar kodlama standartları


## Teslim

**Hem github hemde classroom üzerinden teslim ediniz.**

# ENGLISH
# BIL 222 Project-1: Single shell 


## Bilinmesi gerekenler
* File descriptors
* open/close/read/write system calls
* Process management
* Exec family system calls
* wait/fork system calls


## Learning objectives
* Practicing fork/exec relationships. Understanding the multi-process environment and how they can communicate through a shared object, understanding the details of a shell program. Understanding the need for inter process communication mechanisms which will be taught in the coming weeks.


## Details (Single Shell)
Write a shell program that works as follows: (You can start with the examples we have done in the class)

1. Program should start with the sign **$  (for autograding purposes, your program should work and print as indicated in the instructions. The points are given based on autograded test results.).**

    **Example output:** <code>$ </code>

2. Then, it should run the given command by using fork and exec family functions (DO NOT USE system()).
    1. If the exec gives an error because the command may not be found in the current or system path,
        1. Find the path of the command by using one of the followings or a similar: If we want to find ‘ls’ 
            <code>
            $ which ls
            $ whereis ls
            $ command -v ls (note: if ls is an alias, this does not give the path)
           </code>

        3. If the command is the program in our current path, then use any of the followings to find the current path and run the program:
           <code>
           $ pwd
           $ locate -w path/name 
           $ locate -b basename | fgrep -w path
           </code>

    2. If it is not in the current path and not found in the system path (both give errors), then your  program should print the following message (**name** is the given command):
    <code>
    $ name 
    name: command not found
    $
   </code>


3. **Logging: Log all the given commands in** “log.txt” with time stamps. 

    Time1\t command1
    Time2\t command2


    To get the time you can use<code> [gettimeofday()](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) </code>system call



## Grading
1. 20%
2. 50%
3. and 4 30%

Up to -10% coding standards


## Submission

Through github and classroom
