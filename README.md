
### rms-cli ###
   
linux c++ command line program that output the RMS level of the selected audio input to the terminal console. Useful for testing if [RtAudio](https://github.com/thestk/rtaudio) is working well on a particular device. Press ENTER to stop it. Tested on debian jessie and stretch, on x86 and ARM (raspberry pi). `master` branch is for debian stretch, for debian jessie use `git checkout jessie`.    
   
It depends on [RtAudio](https://github.com/thestk/rtaudio) (instructions for debian-based systems):    
`sudo apt-get install librtaudio-dev librtaudio5`   
on an ARM system you need `rtaudio5a`   
`sudo apt-get install librtaudio-dev librtaudio5a`   
   
Compile with `make`, execute with `./rms-cli`.   
It accepts optional arguments:   
-d <deviceid> to set the audio device      
-sr <samplerate> to set the sample rate      
-b <buffersize> to set the buffer size   
-l just list the devices and then exit   
   
just run it without argument for default values ( `rms-cli -d 1, -sr 44100, -b 1024 ` )   

