#include "../utils.h"
#include "../settings.h"
#include "mz.h"
#include "pe.h"
#include "ne.h"
#include "le.h"
#include <stdio.h>

void scan_mz() {
	struct mz_hdr h;

	if (_osseek(0x3c, SEEK_SET)) {
		report_data();
		return;
	}

	uint32_t p;
	_osread(&p, 4);

	if (p) {
		uint16_t sig;
		if (_osseek(p, SEEK_SET)) goto _MZ;
		_osread(&sig, sizeof(sig));
		switch (sig) {
		case 0x4550: // "PE"
			scan_pe();
			return;
		case 0x454E: // "NE"
			scan_ne();
			return;
		case 0x454C: // "LE"
		case 0x584C: // "LX"
			scan_le(sig == 0x454C);
			return;
		}
	}

_MZ:
	_osseek(0, SEEK_SET);
	_osread(&h, sizeof(h));

	reportf("MZ executable for MS-DOS (Overlay: %u)\n", h.e_ovno);

	if (More) {
		printf(
			"e_cblp    : %Xh\n"
			"e_cp      : %Xh\n"
			"e_crlc    : %Xh\n"
			"e_cparh   : %Xh\n"
			"e_minalloc: %Xh\n"
			"e_maxalloc: %Xh\n"
			"e_ss      : %Xh\n"
			"e_sp      : %Xh\n"
			"e_csum    : %Xh\n"
			"e_ip      : %Xh\n"
			"e_cs      : %Xh\n"
			"e_lfarlc  : %Xh\n"
			"e_ovno    : %Xh\n"
			"e_lfanew  : %Xh\n",
			h.e_cblp, h.e_cp, h.e_crlc, h.e_cparh,
			h.e_minalloc, h.e_maxalloc, h.e_ss,
			h.e_sp, h.e_csum, h.e_ip, h.e_cs,
			h.e_lfarlc, h.e_ovno, h.e_lfanew
		);
	}
}