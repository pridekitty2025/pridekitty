# Pridekitty

An animation that flashes BLM in a LGBTQ+ color theme 
![pridekitty](https://user-images.githubusercontent.com/111705651/186676443-69e8513d-f0c5-4ab1-98c4-79f39b7de464.png)



<br><br><br>



## Setup with MacOS

first Open Terminal

Get Homebrew first

    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   
Get additional apps (optional)   
    
    brew install git && brew install make && brew install automake
    xcode-select --install

Get PrideKitty
    
    git clone https://github.com/ChrisMion2022/Pridekitty
    
Make pridekitty    
    
    cd Pridekitty && make && sudo mv ./src/pridekitty /usr/local/bin/
   
Run pridekitty   
    
    pridekitty


<br><br><br>


## Setup with Windows

Open Command Prompt as Administrator & Install WSL if you don't have it yet

    wsl --install -d Ubuntu
    shutdown -f -r -t 00

At reboot WSL should start by itself, if not start as admin, set your username and password then run this 

    sudo passwd root

Now you are ready to start install the App

    sudo apt update
    sudo apt install make -y
    sudo apt install gcc -y
    sudo apt install tcc -y 
    git clone https://github.com/ChrisMion2022/Pridekitty
    cd Pridekitty
    make install
    cd ~
    pridekitty


<br><br><br>    


## Setup with Linux

Open terminal 

    sudo apt install git-all -y
    sudo apt install make -y
    sudo apt install gcc -y
    git clone https://github.com/ChrisMion2022/Pridekitty

Then build the C application:

    cd Pridekitty
    make install

You can run the C application standalone.

    pridekitty 
   

<br><br><br>

    
## Resources 

The original source of the nyancat animation :
[prguitarman](http://www.prguitarman.com/index.php?id=348)

Origin source code for nyancat :
https://github.com/klange/nyancat
