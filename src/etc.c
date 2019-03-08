#include <stdio.h> // Mostly for *nix
#include <string.h>
#include "utils.h"
#include "ff.h"
#include "etc.h"
#include "archives/iso.h"
#include "archives/tar.h"
#include "documents/mobi.h"

/**
 * Start scanning offsets for signatures
 * Note: Start seeking from the lowest value!
 */
void scan_etc() {
	if (_ddseek(0x3c, SEEK_SET)) goto _END; // Palm Database Format
	char *mt;
	uint32_t b[2];
	_ddread(b, sizeof(b));
	switch (b[0]) { // LOW
	case ADOBE_LOW:
		mt = "Palm Database (Adobe Reader)";
		break;
	case BOOKMOBI_LOW:
		scan_mobi();
		return;
	case PALMDOC_LOW:
		switch (b[1]) {
		case TEALDOC_HIGH:
			mt = "Palm Database (TealDoc)";
			break;
		default: // PALMDOC_HIGH
			scan_palmdoc();
			return;
		}
		break;
	case BDICTY_LOW:
		mt = "Palm Database (BDicty)";
		break;
	case DB_LOW:
		mt = "Palm Database (DB)";
		break;
	case EREADER0_LOW: case EREADER1_LOW:
		switch (b[1]) {
		case PLUCKER_HIGH:
			mt = "Palm Database (Plucker)";
			break;
		case QUICKSHEET_HIGH:
			mt = "Palm Database (QuickSheet)";
			break;
		case TEALMEAL_HIGH:
			mt = "Palm Database (TealMeal)";
			break;
		case TEALPAINT_HIGH:
			mt = "Palm Database (TailPaint)";
			break;
		default: // EREADER0_HIGH/EREADER1_HIGH
			mt = "Palm Database (eReader)";
			break;
		}
		break;
	case FIREVIEWER_LOW:
		mt = "Palm Database (FireViewer)";
		break;
	case HANDBASE_LOW:
		mt = "Palm Database (HanDBase)";
		break;
	case INFOVIEW_LOW:
		switch (b[1]) {
		case TEALINFO_HIGH:
			mt = "Palm Database (TealInfo)";
			break;
		default:
			mt = "Palm Database (InfoView)";
			break;
		}
		break;
	case ISILO_LOW:
		mt = "Palm Database (iSilo)";
		break;
	case ISILO3_LOW:
		mt = "Palm Database (iSilo 3)";
		break;
	case JFILE_LOW:
		mt = "Palm Database (JFile)";
		break;
	case JFILEPRO_LOW:
		mt = "Palm Database (JFile Pro)";
		break;
	case LIST_LOW:
		mt = "Palm Database (LIST)";
		break;
	case MOBILEDB_LOW:
		mt = "Palm Database (MobileDB)";
		break;
	case SUPERMEMO_LOW:
		mt = "Palm Database (SuperMemo)";
		break;
	case THINKDB_LOW:
		mt = "Palm Database (ThinKDB)";
		break;
	case TIDES_LOW:
		mt = "Palm Database (Tides)";
		break;
	case TOMERAIDER_LOW:
		mt = "Palm Database (TomeRaider)";
		break;
	case WEASEL_LOW:
		mt = "Palm Database (Weasel)";
		break;
	case WORDSMITH_LOW:
		mt = "Palm Database (WordSmith)";
		break;
	default: goto MOBI_OUT;
	}
	reportn(mt);
	palmdb_name();
	return;
MOBI_OUT:

	if (_ddseek(0x101, SEEK_SET)) goto _END; // Tar files
	//char btar[sizeof(TAR)];
	//_ddread(btar, sizeof(TAR));
	uint32_t btar32[2];
	_ddread(btar32, 8);
	//if (strcmp(btar, TAR) == 0 || strncmp(btar, GNUTAR, 8) == 0) {
	switch (btar32[0]) {
	case TAR32L: case GNUTAR32L:
		switch (btar32[1]) {
		case TAR32H: case GNUTAR32H:
			scan_tar(); return;
		}
		break;
	}

	if (_ddseek(0x8001, SEEK_SET)) goto _END; // ISO files
	char biso[5];
	_ddread(biso, 5);
	if (strncmp(biso, ISO, 5) == 0) {
		scan_iso(); return;
	}

	if (_ddseek(0x8801, SEEK_SET)) goto _END; // ISO files
	_ddread(biso, 5);
	if (strncmp(biso, ISO, 5) == 0) {
		scan_iso(); return;
	}

	if (_ddseek(0x9001, SEEK_SET)) goto _END; // ISO files
	_ddread(biso, 5);
	if (strncmp(biso, ISO, 5) == 0) {
		scan_iso(); return;
	}

_END: // If file is smaller than our seeks and/or didn't find anything
	//TODO: Detect text?
	report_unknown();
}