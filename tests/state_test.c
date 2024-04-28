//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"


///// Βοηθητικές συναρτήσεις ////////////////////////////////////////
//
// Ελέγχει την (προσεγγιστική) ισότητα δύο double
// (λόγω λαθών το a == b δεν είναι ακριβές όταν συγκρίνουμε double).
static bool double_equal(double a, double b) {
	return fabs(a-b) < 1e-6;
}

// Ελέγχει την ισότητα δύο διανυσμάτων
static bool vec2_equal(Vector2 a, Vector2 b) {
	return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}
/////////////////////////////////////////////////////////////////////


void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

	// Προσθέστε επιπλέον ελέγχους

	TEST_ASSERT(vec2_equal(info->spaceship->speed, (Vector2){0,0}) == 1);
	TEST_ASSERT(vec2_equal(info->spaceship->orientation, (Vector2){0,1}) == 1);
	TEST_ASSERT(vec2_equal(info->spaceship->position, (Vector2){0,0}) == 1);
	
	//Με τους παρακάτω ελέγχους κοιτάω άμα έχουν φτιαχτεί σωστά οι 6 αστεροειδείς και όχι
	//παραπάνω. Το καταφέρνω αυτό με δύο ελέγχους. Πρώτα ελέγχο ότι έχουν φτιαχτεί
	//σε σωστή απόσταστη από το αρχικό σημείο του διαστημόπλοιου (0,0) και δεύτερον ότι
	//έχουν φτιαχτεί μόνο 6 (αφού ASTEROID_NUM = 6)

	List objTest1 = state_objects(state, (Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, (Vector2){-SCREEN_WIDTH,-SCREEN_HEIGHT});
	List objTest2 = state_objects(state, (Vector2){ASTEROID_MAX_DIST,ASTEROID_MAX_DIST}, (Vector2){-ASTEROID_MAX_DIST,-ASTEROID_MAX_DIST});

	TEST_ASSERT(list_size(objTest1) == list_size(objTest2));
	TEST_ASSERT(list_size(objTest2) == 6);
	
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το διαστημόπλοιο παραμένει σταθερό με μηδενική ταχύτητα
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,0}) );

	// Με πατημένο το πάνω βέλος, η ταχήτητα αυξάνεται ενώ το διαστημόπλοιο παραμένει για την ώρα ακίνητο
	keys.up = true;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,SPACESHIP_ACCELERATION}) );

	// Προσθέστε επιπλέον ελέγχους
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
