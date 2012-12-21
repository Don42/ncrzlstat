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

#define _GNU_SOURCE

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "ncrzlstat.h"
#include "ui.h"
#include "fetch.h"
#include "parse.h"

#define STATUSURL	"http://status.raumzeitlabor.de/api/full.json"
#define COSMURL		"http://api.cosm.com/v2/feeds/42055.json?key=%s"

void	usage(void);

int
main(int argc, char *argv[])
{
	enum fetch_ipversion ipresolve = FETCH_IPVANY;

	int ch;
	while ((ch = getopt(argc, argv, "h46")) != -1) {
		switch (ch) {
		case '4':
			if (ipresolve == FETCH_IPV6ONLY) {
				usage();
				exit(EXIT_FAILURE);
			}
			ipresolve = FETCH_IPV4ONLY;
			break;
		case '6':
			if (ipresolve == FETCH_IPV4ONLY) {
				usage();
				exit(EXIT_FAILURE);
			}
			ipresolve = FETCH_IPV6ONLY;
			break;
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
		default:
			usage();
			exit(EXIT_FAILURE);
		}
	}

	char *cosmkey = getenv("RZLCOSMKEY");
	if (cosmkey == NULL) {
		fprintf(stderr,
		    "Environment variable RZLCOSMKEY is not set.\n");
		exit(EXIT_FAILURE);
	}
	char *cosmurl;
	asprintf(&cosmurl, COSMURL, cosmkey);

	ui_init();
	atexit(&ui_deinit);

	struct model *model = NULL;
	time_t last = 0;
	bool loop = true;
	while (loop) {
		if (last == 0 || last <= time(NULL) - REFRESH) {
			last = time(NULL);

			char *status = fetch_data_string(STATUSURL, ipresolve);
			assert(status != NULL);

			char *cosm = fetch_data_string(cosmurl, ipresolve);
			assert(cosm != NULL);

			if (model != NULL)
				parse_free_model(model);
			model = parse_fill_model(status, cosm);
			assert(model != NULL);

			free(cosm);
			free(status);
		}

		ch = ui_display(model);

		switch (ch) {
		case 'q':
		case 'Q':
			loop = false;
			break;
		}
	}

	if (model != NULL)
		parse_free_model(model);
	free(cosmurl);

	return (0);
}

void
usage(void)
{
	fprintf(stderr, "usage: ncrzlstat [-h] [-4|-6]\n");
}
