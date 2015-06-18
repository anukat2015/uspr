// TODO: basic data structures
// TODO: mAF iteration
// TODO: mEAF iteration
// TODO: USPR distance


// includes
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <ctime>
#include <cstdlib>
#include "utree.h"
#include "unode.h"
#include "uforest.h"
#include "tbr.h"
#include "uspr.h"

using namespace std;

// constants
//
string USAGE =
"uspr, version 0.0.1\n";

bool DEFAULT_OPTIMIZATIONS = true;
bool PRINT_mAFS = false;
bool COUNT_mAFS = false;

bool ALL_DISTANCES = true;
bool COMPUTE_TBR_APPROX = false;
bool COMPUTE_TBR = false;
bool COMPUTE_REPLUG = false;
bool COMPUTE_USPR = false;



// function prototypes


int main(int argc, char *argv[]) {
	int max_args = argc-1;
	while (argc > 1) {
		char *arg = argv[--argc];
		if (strcmp(arg, "--print-mAFs") == 0) {
			PRINT_mAFS = true;
		}
		if (strcmp(arg, "--count-mAFs") == 0) {
			COUNT_mAFS = true;
		}
		else if (strcmp(arg, "--keep-labels") == 0) {
			KEEP_LABELS = true;
				cout << "KEEP_LABELS=true" << endl;
		}
		else if (strcmp(arg, "--no-opt") == 0) {
			DEFAULT_OPTIMIZATIONS = false;
			cout << "NO OPTIMIZATIONS" << endl;
		}
		else if (strcmp(arg, "--no-protect-b") == 0) {
			OPTIMIZE_PROTECT_B = false;
		}
		else if (strcmp(arg, "--tbr-approx") == 0) {
			COMPUTE_TBR_APPROX = true;
			ALL_DISTANCES = false;
		}
		else if (strcmp(arg, "--tbr") == 0) {
			COMPUTE_TBR = true;
			ALL_DISTANCES = false;
		}
		else if (strcmp(arg, "--replug") == 0) {
			COMPUTE_REPLUG = true;
			ALL_DISTANCES = false;
		}
		else if (strcmp(arg, "--uspr") == 0) {
			COMPUTE_USPR = true;
			ALL_DISTANCES = false;
		}
		else if (strcmp(arg, "--no-approx-estimate") == 0) {
			USE_TBR_APPROX_ESTIMATE = false;
		}
		else if (strcmp(arg, "--no-tbr-estimate") == 0) {
			USE_TBR_ESTIMATE = false;
		}
		else if (strcmp(arg, "--no-replug-estimate") == 0) {
			USE_REPLUG_ESTIMATE = false;
		}
		else if (strcmp(arg, "--help") == 0 ||
				strcmp(arg, "-h") == 0 ||
				strcmp(arg, "-help") == 0) {
			cout << USAGE;
			return 0;
		}
	}

	if (DEFAULT_OPTIMIZATIONS == false) {
		OPTIMIZE_2B = false;
		OPTIMIZE_PROTECT_A = false;
		OPTIMIZE_PROTECT_B = false;
		OPTIMIZE_BRANCH_AND_BOUND = false;
		cout << "NO OPTIMIZATIONS" << endl;
	}

	if (ALL_DISTANCES) {
		COMPUTE_TBR_APPROX = true;
		COMPUTE_TBR = true;
		COMPUTE_REPLUG = true;
		COMPUTE_USPR = true;
	}

	// label maps to allow string labels
	map<string, int> label_map= map<string, int>();
	map<int, string> reverse_label_map = map<int, string>();

	// set random seed
	srand(unsigned(time(0)));

	// read input trees
	string T1_line = "";
	string T2_line = "";
	while (getline(cin, T1_line) && getline(cin, T2_line)) {
		// load into data structures
		uforest F1 = uforest(T1_line, label_map, reverse_label_map);
		F1.normalize_order();
		uforest F2 = uforest(T2_line, label_map, reverse_label_map);
		F2.normalize_order();
		cout << "T1: " << F1.str(false, &reverse_label_map) << endl;
		cout << "T2: " << F2.str(false, &reverse_label_map) << endl;
		// compute TBR distance
		if (COMPUTE_TBR_APPROX) {
		cout << "a_TBR: " << tbr_high_lower_bound(F1, F2) << " <= d_TBR <= " << tbr_low_upper_bound(F1, F2) << endl;
		}
		if (COMPUTE_TBR) {
			uforest *MAF1 = NULL;
			uforest *MAF2 = NULL;
			int distance = tbr_distance(F1, F2, false, &MAF1, &MAF2);
			cout << "d_TBR = " << distance << endl;
			if (MAF1 != NULL) {
				cout << "F1: " << MAF1->str(false, &reverse_label_map) << endl;
				delete MAF1;
			}
			if (MAF2 != NULL) {
				cout << "F2: " << MAF2->str(false, &reverse_label_map) << endl;
				delete MAF2;
			}
		}
		int count;
		if (PRINT_mAFS) {
			count = tbr_print_mAFs(F1, F2);
			cout << count << " mAFs" << endl;
		}
		else if (COUNT_mAFS) {
			count = tbr_count_mAFs(F1, F2);
			cout << count << " mAFs" << endl;
		}

		if (COMPUTE_REPLUG) {
			uforest *MAF1 = NULL;
			uforest *MAF2 = NULL;
			int d_replug = replug_distance(F1, F2, false, &MAF1, &MAF2);
			cout << "d_R = " << d_replug << endl;
			if (MAF1 != NULL) {
				cout << "F1: " << MAF1->str(false, &reverse_label_map) << endl;
				delete MAF1;
			}
			if (MAF2 != NULL) {
				cout << "F2: " << MAF2->str(false, &reverse_label_map) << endl;
				delete MAF2;
			}
		}

		if (COMPUTE_USPR) {
			int d_uspr = uspr_distance(F1, F2);
			cout << "d_USPR = " << d_uspr << endl;
		}

	}
}
