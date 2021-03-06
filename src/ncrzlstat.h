/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                             Tobias Rehbein
 *
 *  Ported to Linux with curl instead of fetch by <DonMarco42@gmail.com>
 *  Marco "don" Kaulea. He prefers Single Malt Scotch.
 */

#define REFRESH	10

struct model {
	/* status */
	bool door;
	int devices;
	int present;
	time_t time;
	char **presentnames;
	/* cosm */
	int members;
	double balance;
	double temperature;
	double drain;
	double latency;
	double download;
	double upload;
};

