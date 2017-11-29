
#include <iostream>
#include <math.h>
#include <cstring>

#include "RtAudio.h"

#define DEFAULT_DEVICE 0
#define DEFAULT_SAMPLERATE 44100
#define DEFAULT_BUFFERSIZE 1024

// This program open the selected devices and prints the average level to the 
// terminal as a list of stars. Press ENTER to stop the program.

int audioIn( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData );


int main( int argc, char *argv[] ) {

	RtAudio audio;
	RtAudio::StreamParameters parameters;
	RtAudio::StreamOptions options;

	parameters.deviceId = DEFAULT_DEVICE;
	unsigned int bufferFrames = DEFAULT_BUFFERSIZE;
	unsigned int sampleRate = DEFAULT_SAMPLERATE;
	bool bList = false;

	// get command line parameters	
	if(argc>1){		
		for(int i=1; i<argc; i+=2){
			if( ! strcmp( argv[i], "-l" ) ){
				bList = true;	
			}else
			if( ! strcmp( argv[i], "-d" ) ){
				int dev = std::strtol( argv[i+1], NULL, 10 );
				parameters.deviceId = dev;			
			}else
			if( ! strcmp( argv[i], "-sr" ) ){
				int srIn = std::strtol( argv[i+1], NULL, 10 );
				sampleRate = srIn;			
			}else
			if( ! strcmp( argv[i], "-b" ) ){
				int bIn  = std::strtol( argv[i+1], NULL, 10 );
				bufferFrames = bIn;			
			}else {
				std::cout<<"wrong usage!\n";
				std::cout<<"use -d <deviceid> to set the audio device\n";
				std::cout<<"use -sr <samplerate> to set the sample rate\n";
				std::cout<<"use -b <buffersize> to set the buffer size\n";
				std::cout<<"or just run it without argument for default values (-d 1, -sr 44100, -b 1024)\n";
				exit(0);
			}
		}
	}

	
	// -------------------- LIST DEVICES --------------------------------------
	// Determine the number of devices available
	unsigned int devices = audio.getDeviceCount();
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for ( unsigned int i=0; i<devices; i++ ) {
		info = audio.getDeviceInfo( i );
		if ( info.probed == true ) {
			// Print, for example, the maximum number of output channels for each device
			std::cout << "device = " << i << " | name = "<< info.name;
			std::cout << " | ins/outs = " << info.inputChannels <<"/"<< info.outputChannels << "\n";
		}
	}
	
	if(bList){ exit(0); }
	
	// ------------------- OPEN DEVICE INPUT ----------------------------------
	RtAudio adc;
	if ( adc.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}
	std::cout <<"selected device " << parameters.deviceId  << "\n";
	
	parameters.nChannels = 2;
	parameters.firstChannel = 0;

	options.numberOfBuffers = 3;
	options.priority = 99;
	options.flags = RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_SCHEDULE_REALTIME | RTAUDIO_HOG_DEVICE;
	
	try {
		adc.openStream( NULL, &parameters, RTAUDIO_FLOAT32,
						sampleRate, &bufferFrames, &audioIn, NULL, &options );
		adc.startStream();
	}
	catch ( RtAudioError& e ) {
		e.printMessage();
		exit( 0 );
	}

	char input;
	std::cout << "\nRecording ... press <enter> to quit.\n";
	std::cin.get( input );
	try {
		// Stop the stream
		adc.stopStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
	}
	if ( adc.isStreamOpen() ) adc.closeStream();

	return 0;
}


int audioIn( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData ) {
	
	float sum = 0.0f;
	float * in = (float*) inputBuffer;
	
	// calculate rms
	for( unsigned int i=0; i<nBufferFrames; ++i ){
		float value = in[i*2];
		value = value * value; 
		sum+=value; 
	}
	float rms = sum / (float) nBufferFrames;
	rms = sqrt( sum );
	
	// plotting routine
	int stars = rms*64;
	if(stars>68) stars = 58;
	
	std::string sRms = std::to_string( rms );
	int offset = 14-sRms.length();
	
	std::cout<<sRms;
	
	for( int i=0; i<offset; ++i ) {
		std::cout << " ";
	}
	std:: cout << " | ";
	
	for (int i=0; i<stars; ++i){
		std::cout<<"*";
	}
	
	std::cout<<"\n";
	
	return 0;
}
