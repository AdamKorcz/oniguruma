#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "oniguruma.h"
#include "regint.h"

static int
scan_callback(int n, int r, OnigRegion* region, void* arg){
	return 0;
}


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	if(size<3){
		return 0;
	}

	char *new_str = (char *)malloc(size+1);
	if (new_str == NULL){
		return 0;
	}
	memcpy(new_str, data, size);
	new_str[size] = '\0';

	regex_t* reg;
	UChar* str = (UChar* )new_str;
	UChar* end = str + strlen(new_str);
	OnigOptionType options = ONIG_OPTION_NONE;
	OnigErrorInfo einfo;

	int new_onig = onig_new(&reg, str, end,
			        ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII,
				ONIG_SYNTAX_DEFAULT, &einfo);

	if (new_onig != ONIG_NORMAL) {
		free(new_str);
		return 0;
	}

	OnigRegion *region = onig_region_new();
	(reg)->extp = NULL;
	RegexExt* r3 = onig_get_regex_ext(reg);
	reg->extp = r3;

	int fuzzer = onig_scan(reg, str, NULL,
			       region, ONIG_OPTION_NONE,
			       scan_callback, NULL);

	onig_region_free(region, 1);
	onig_free(reg);
	free(new_str);
	return 0;
}
