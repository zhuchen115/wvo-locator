#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __GNUC__
#define SHARE_DOCUMENT_DIR "/usr/local/share/pprobot/"

#define CONTOUR_CACHE_FILE "/var/local/lib/pprobot/contours.xml"

#define VAR_DOCUMENT_DIR "/var/local/lib/pprobot/"
#else
#define SHARE_DOCUMENT_DIR "D:/code/vsproj/wvo-locator/var_share/"

#define CONTOUR_CACHE_FILE "D:/code/vsproj/wvo-locator/var_share/contours.xml"

#define VAR_DOCUMENT_DIR "D:/code/vsproj/wvo-locator/var_share/"
#endif
/// The thresold to find the HSV mask threshold
#define ADAPTIVE_THRESHOLD 0.007
/// The sum of comers in comparing the Hu Moments
#define CONTOUR_COMERS_THRESHOLD 1.7 

#ifdef linux
#define GEN_LOG 1
#else
#define GEN_LOG 0
#endif
#if GEN_LOG
#include <syslog.h>
#define LOG_PRINT(level,msg,...) syslog(level,msg,##__VA_ARGS__);\
									if(Config::showmsg) printf(msg,##__VA_ARGS__);\
									printf("\n")
#define LOG_PRINT_ERR(level,msg,...)  syslog(level,msg,##__VA_ARGS__);\
									if(Config::showmsg) \
										fprintf(stderr,msg,##__VA_ARGS__)
#else
#define LOG_PRINT(level,msg,...) if(Config::showmsg) \
									printf(msg,##__VA_ARGS__);\
									printf("\n")
#define LOG_PRINT_ERR(level,msg,...)  fprintf(stderr,msg,##__VA_ARGS__);\
										fprintf(stderr,"\n")

#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

#endif
class Config {
public:
	/// Print the message
	static bool showmsg;

};

/** The Displacement Sensor to centimeter value in float*/
#define MICE_X_COEF 331.38
#define MICE_Y_COEF 322.13

/// Using /dev/spidev0.0
#define SPI_CHANNEL 0
/// Set the spi speed tp 500K
#define SPI_CLOCK 500000
/// The Slave need to answer
#define NEED_ACK 1

/** The Mechanical Hook max h range*/
#define HOOK_MAX_OFFSET 60


#endif
