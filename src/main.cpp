#include <Arduino.h>
#include "pitches.h"

#define MP_IDLE     0
#define MP_PLAYING  1

#define TS_44 4     // 4/4 time signature
#define TS_34 3     // 3/4 time signature
#define TS_22 2     // 2/2 time signature

class MelodyPlayer
{
	private:
    int (*melody_data)[2];
		int pin;
    int length;
		int state;
    int current_note;
    int tempo;
    int time_signature;
		unsigned long current_ms;
		unsigned long pause_time;

	public:
		MelodyPlayer(int pin);
    void loop(void);
		void play(int (*nd_pair)[2], int l, int ts, int t);
    void stop(void);
		int getState(void);
};

MelodyPlayer::MelodyPlayer(int p) {
	pin = p;
	state = MP_IDLE;
	current_ms = 0;
	length = 0;
	current_note = 0;
	pause_time = 0;
  tempo = 0;
  time_signature = 0;
	pinMode(pin, OUTPUT);
}

void MelodyPlayer::loop(void) {

	switch(state) {
		case MP_IDLE:
			break;

		case MP_PLAYING:
      if(current_note < length) {
        if(!pause_time) {
          /*
            To calculate the note duration, multiply the number of beats per measure by 60,000
            then divide that by the tempo. To get the note duration in milliseconds, divide the
            result by the note's duration.
          */ 
          int duration = (60000 * time_signature) / tempo / melody_data[current_note][1];
          if (melody_data[current_note][1] < 0)
          {
            /*
              Dotted notes are represented by negative numbers.
              If the note is a dotted note, increase the duration by half.
            */ 
            duration = abs(duration) * 1.5;
          }
          
          tone(pin, melody_data[current_note][0], duration * 0.9);

          // To distinguish the notes, set a minimum interval between them with multiplier.
          pause_time = duration;
          current_ms = millis();
        }

        if ((unsigned long)(millis() - current_ms) >= pause_time) {
          noTone(pin); // stop the tone playing:
          pause_time = 0;
          current_note++;
        }
      } else {
        noTone(pin);
				state = MP_IDLE;
			}

			break;

		default:
			break;
	}
}

void MelodyPlayer::play(int (*nd_pair)[2], int l, int ts, int t) {
	melody_data = nd_pair;
	length = l;
  time_signature = ts;
  tempo = t;
	current_note = 0;
	pause_time = 0;
	state = MP_PLAYING;
	current_ms = millis();
}

void MelodyPlayer::stop(void) {
	noTone(pin);
	digitalWrite(pin, LOW);
  state = MP_IDLE;
}

int MelodyPlayer::getState(void) {
	return state;
}

int score[][2] = {

  // Never Gonna Give You Up - Rick Astley
  // Score available at https://musescore.com/chlorondria_5/never-gonna-give-you-up_alto-sax
  // Arranged by Chlorondria

  {NOTE_D5,-4}, {NOTE_E5,-4}, {NOTE_A4,4}, //1
  {NOTE_E5,-4}, {NOTE_FS5,-4}, {NOTE_A5,16}, {NOTE_G5,16}, {NOTE_FS5,8},
  {NOTE_D5,-4}, {NOTE_E5,-4}, {NOTE_A4,2},
  {NOTE_A4,16}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,8}, {NOTE_D5,16},
  {NOTE_D5,-4}, {NOTE_E5,-4}, {NOTE_A4,4}, //repeat from 1
  {NOTE_E5,-4}, {NOTE_FS5,-4}, {NOTE_A5,16}, {NOTE_G5,16}, {NOTE_FS5,8},
  {NOTE_D5,-4}, {NOTE_E5,-4}, {NOTE_A4,2},
  {NOTE_A4,16}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,8}, {NOTE_D5,16},
  {REST,4}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_D5,8}, {NOTE_E5,8}, {NOTE_CS5,-8},
  {NOTE_B4,16}, {NOTE_A4,2}, {REST,4},

  {REST,8}, {NOTE_B4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,4}, {NOTE_A4,8}, //7
  {NOTE_A5,8}, {REST,8}, {NOTE_A5,8}, {NOTE_E5,-4}, {REST,4}, 
  {NOTE_B4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, {NOTE_D5,8}, {NOTE_E5,8}, {REST,8},
  {REST,8}, {NOTE_CS5,8}, {NOTE_B4,8}, {NOTE_A4,-4}, {REST,4},
  {REST,8}, {NOTE_B4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, {NOTE_A4,4},
  {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,4}, {REST,4},
   
  {NOTE_D5,2}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_D5,8}, //13
  {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,4}, {NOTE_A4,4},
  {REST,2}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8},
  {REST,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},

  {NOTE_E5,-8}, {NOTE_E5,-8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,-8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16}, //18
  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,8}, {NOTE_A4,8}, {NOTE_A4,8}, 
  {NOTE_E5,4}, {NOTE_D5,2}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_A5,4}, {NOTE_CS5,8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},

  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,4}, {NOTE_A4,8},  //23
  {NOTE_E5,4}, {NOTE_D5,2}, {REST,4},
  {REST,8}, {NOTE_B4,8}, {NOTE_D5,8}, {NOTE_B4,8}, {NOTE_D5,8}, {NOTE_E5,4}, {REST,8},
  {REST,8}, {NOTE_CS5,8}, {NOTE_B4,8}, {NOTE_A4,-4}, {REST,4},
  {REST,8}, {NOTE_B4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, {NOTE_A4,4},
  {REST,8}, {NOTE_A5,8}, {NOTE_A5,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,8}, {NOTE_D5,8},
  
  {REST,8}, {NOTE_A4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, //29
  {REST,8}, {NOTE_CS5,8}, {NOTE_B4,8}, {NOTE_A4,-4}, {REST,4},
  {NOTE_B4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, {NOTE_A4,4}, {REST,8},
  {REST,8}, {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_FS5,4}, {NOTE_E5,-4}, 
  {NOTE_D5,2}, {NOTE_D5,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,4}, 
  {NOTE_E5,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,8}, {NOTE_A4,8}, {NOTE_A4,4},

  {REST,-4}, {NOTE_A4,8}, {NOTE_B4,8}, {NOTE_CS5,8}, {NOTE_D5,8}, {NOTE_B4,8}, //35
  {REST,8}, {NOTE_E5,8}, {NOTE_FS5,8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_E5,-8}, {NOTE_E5,-8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,4}, {NOTE_A4,8}, 

  {NOTE_E5,4}, {NOTE_D5,2}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16}, //40
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_A5,4}, {NOTE_CS5,8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,4}, {NOTE_A4,8},  
  {NOTE_E5,4}, {NOTE_D5,2}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
   
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16}, //45
  {NOTE_A5,4}, {NOTE_CS5,8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,4}, {NOTE_A4,8},  
  {NOTE_E5,4}, {NOTE_D5,2}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_FS5,-8}, {NOTE_FS5,-8}, {NOTE_E5,-4}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16}, //45
  
  {NOTE_A5,4}, {NOTE_CS5,8}, {NOTE_D5,-8}, {NOTE_CS5,16}, {NOTE_B4,8}, {NOTE_A4,16}, {NOTE_B4,16}, {NOTE_D5,16}, {NOTE_B4,16},
  {NOTE_D5,4}, {NOTE_E5,8}, {NOTE_CS5,-8}, {NOTE_B4,16}, {NOTE_A4,4}, {NOTE_A4,8}, 

  {NOTE_E5,4}, {NOTE_D5,2}, {REST,4}
};

int length = sizeof(score) / sizeof(score[0]);

MelodyPlayer music(8);                          // Create a MelodyPlayer object that plays on pin 8

const int Button = 7;
const int Buzzer = 8;

int button_state0 = HIGH;

void setup() {
  pinMode(Buzzer, OUTPUT);
  pinMode(Button, INPUT);
}

void loop() {

  music.loop();                                // Call the loop function of the MelodyPlayer object

  int button_reading = digitalRead(Button);

  if (button_state0 == HIGH && button_reading == LOW) {
    if (music.getState() == MP_PLAYING) {
      music.stop();                           // Stop the melody if it is playing
    } else {
      music.play(score, length, TS_44, 114);  // Play the melody in 4/4 time signature at 114 bpm
    }
  }

  button_state0 = button_reading;

}