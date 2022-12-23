# Pridekitty

An animation that flashes BLM in a LGBTQ+ color theme 
![pridekitty](https://user-images.githubusercontent.com/111705651/186676443-69e8513d-f0c5-4ab1-98c4-79f39b7de464.png)





## Setup with Linux

Open terminal 

    sudo apt install git-all
    git clone https://github.com/ChrisMion2022/Pridekitty

Then build the C application:

    make install

You can run the C application standalone.

    pridekitty 
   

To use the telnet server, you need to add a configuration that runs:

    pridekitty -t
    
    
    
    
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
    
    cd pridekitty && make && sudo mv ./src/pridekitty /usr/local/bin/
   
Run pridekitty   
    
    pridekitty
    
    
    
## Setup with Windows

Install Virtual Box and install Ubuntu, that's how it goes...
    

## Resources 

The original source of the nyancat animation :
[prguitarman](http://www.prguitarman.com/index.php?id=348)

Origin source code for nyancat :
https://github.com/klange/nyancat
