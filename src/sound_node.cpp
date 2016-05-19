#include<ros/ros.h>
#include<stdio.h>
#include<sound_play/SoundRequest.h>
bool flag=true;
int main(int argc, char** argv){
	ros::init(argc, argv, "sound");
	ros::NodeHandle n_sound;
	ros::Publisher sound_pub = n_sound.advertise<sound_play::SoundRequest>("/robotsound",1);
	
	char f_name[]="/home/amsl/wav/level_up.wav";
	sound_play::SoundRequest sound;
	sound.sound=-2;
	sound.command=1;
	sound.volume=1.0;
	sound.arg=f_name;
	
	char say_text[]="QR has been saved";
	sound_play::SoundRequest say;
	say.sound=-3;
	say.command=1;
	say.volume=15.0;	
	say.arg=say_text;
	say.arg2="voice_kal_diphone";
	
	int play_rate=5;
	int count=0;

	ros::Rate loop_rate(1); // ループの周期
	while(ros::ok()){	
		//printf("\aBeep\n");
		printf("count:%d\n",count);
		
		if(flag && count%play_rate==0){
			sound_pub.publish(sound);
		}
		if(flag && count%play_rate==2 &&count>3){
			sound_pub.publish(say);
		}
		count++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
