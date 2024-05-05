#include <alinix/init.h>
#include <alinix/kernel.h>



#define DEBUG_CAPTURES	 		1

#define RX_CAP_RAW_FILE			"/tmp/rx_cap_in.pcm"
#define TX_CAP_RAW_FILE			"/tmp/tx_cap_in.pcm"

#define	MIXER_PARAM_MIC_PLAYBACK_SW 	"Mic Playback Switch"
#define MIXER_PARAM_MIC_PLAYBACK_VOL 	"Mic Playback Volume"
#define	MIXER_PARAM_MIC_CAPTURE_SW 	"Mic Capture Switch"
#define	MIXER_PARAM_MIC_CAPTURE_VOL 	"Mic Capture Volume"
#define	MIXER_PARAM_MIC_BOOST 		"Auto Gain Control"
#define	MIXER_PARAM_SPKR_PLAYBACK_SW 	"Speaker Playback Switch"
#define	MIXER_PARAM_SPKR_PLAYBACK_VOL 	"Speaker Playback Volume"



#define	DELIMCHR ','
#define	QUOTECHR 34

#include "soundcard.h"

/* ALSA and serial stuff */
#define ALSA_INDEV		"default"
#define ALSA_OUTDEV		"default"
#define DESIRED_RATE		8000
#define SERIAL_DEV		"/dev/ttyS0"

/* Lets use 160 sample frames, just like GSM.  */
#define FRAME_SIZE 160
#define PERIOD_FRAMES 80		/* 80 Frames, at 2 bytes each */


#define MAX_BUFFER_SIZE 100

int sim_cor = 0;		/* used to simulate COR active */

/* ALSA and serial stuff end */

#define O_CLOSE	0x444

#define	NTAPS 31

