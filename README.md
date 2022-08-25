# pridekitty CLI

![pridekitty](https://user-images.githubusercontent.com/111705651/186676443-69e8513d-f0c5-4ab1-98c4-79f39b7de464.png)


## Setup

Open terminal 

    sudo apt install git-all
    git clone https://github.com/ChrisMion2022/Pridekitty

Then build the C application:

    make && cd src
    sudo cp pridekittty /usr/bin/pridekitty

You can run the C application standalone.

    pridekitty 
   

To use the telnet server, you need to add a configuration that runs:

    pridekitty -t


## References, etc.

The original source of the nyancat animation :
[prguitarman](http://www.prguitarman.com/index.php?id=348)

Origin source code for nyancat :
https://github.com/klange/nyancat
