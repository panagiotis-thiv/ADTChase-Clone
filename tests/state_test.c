//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"
#include "set_utils.h"


#define SPACESHIP_RADIUS SPACESHIP_SIZE/2

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

	//Έλεγχος δημιουργίας 6 αστεροειδών κοντά στο διαστημόπλοιο

	List objTest1 = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});
	List objTest2 = state_objects(state, (Vector2){-ASTEROID_MAX_DIST,ASTEROID_MAX_DIST}, (Vector2){ASTEROID_MAX_DIST,-ASTEROID_MAX_DIST});

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

	//Προσθέστε επιπλέον ελέγχους

	//Έλεγχος μετακίνησης διαστημόπλοιου

	keys.up = false;
	state_update(state, &keys);
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,SPACESHIP_ACCELERATION}) );

	state_update(state, &keys);
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,SPACESHIP_ACCELERATION+(SPACESHIP_ACCELERATION*SPACESHIP_SLOWDOWN)}) );

	//printf("Rotation is: %f and %f\n", state_info(state)->spaceship->orientation.x, state_info(state)->spaceship->orientation.y);
	//TODO Rotation check

	//Έλεγχος δημιουργία αστεροειδών γενικά
	List all_asteroids = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});
	while (list_size(all_asteroids) == 6) {
		state_update(state, &keys);
		all_asteroids = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});
	}
	TEST_ASSERT(list_size(all_asteroids) == 7);
	TEST_ASSERT(state_info(state)->score == 1);

	float spaceship_y = state_info(state)->spaceship->position.y;
	float spaceship_x = state_info(state)->spaceship->position.x;

	//Έλεγχος δημιουργίας σφαίρας
	List bullets = state_objects(state, (Vector2){-10+spaceship_x, 10+spaceship_y}, (Vector2){10+spaceship_x, -10+spaceship_y});
	TEST_ASSERT(list_size(bullets) == 0);

	keys.space = true;
	state_update(state, &keys); // B_D(Bullet_Delay) = 1
	
	bullets = state_objects(state, (Vector2){-10+spaceship_x, 10+spaceship_y}, (Vector2){10+spaceship_x, -10+spaceship_y});
	TEST_ASSERT(list_size(bullets) == 1);

	//Η for εξηγείτε από κάτω αναλυτικά γιατί μέχρι 16. Όταν δημιουργείται η σφαίρα είμαστε frames 1, περνάνε 14 και στο 15ο
	//θα δημιουργηθεί η επόμενη σφαίρα
	for (int i = 0; i < 16; i++) 
		state_update(state, &keys);
	
	// state_update(state, &keys); // B_D = 2 | i = 0 
	// ...
	// state_update(state, &keys); // B_D = 14 | i = 12
	// state_update(state, &keys); // B_D = 15 | i = 13
	// state_update(state, &keys); // B_D = 0 | i = 14
	// state_update(state, &keys); // B_D = 1 + new bullet created | i = 15
	

	spaceship_y = state_info(state)->spaceship->position.y;
	spaceship_x = state_info(state)->spaceship->position.x;	

	bullets = state_objects(state, (Vector2){-10+spaceship_x, 10+spaceship_y}, (Vector2){10+spaceship_x, -10+spaceship_y});
	TEST_ASSERT(list_size(bullets) == 1); //Αφού είναι 1 σημαίνει ότι έχει φύγει η προηγούμενη σφαίρα οπότε δουλεύει και η ταχύτητα της
	keys.space = false;

	//Έλεγχος συγκρούσεων

	//Αστεροειδής - Διαστημόπλοιο
	int score = state_info(state)->score;
	int count_all_asteroids = list_size(state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000}));
	int count_close_asteroids = list_size(state_objects(state, (Vector2){-ASTEROID_MAX_DIST+spaceship_x,ASTEROID_MAX_DIST+spaceship_y}, (Vector2){ASTEROID_MAX_DIST+spaceship_x,-ASTEROID_MAX_DIST+spaceship_y}));

	int prev_score, prev_count_all_asteroids, prev_count_close_asteroids;
	
	while (score != prev_score/2) {
		state_update(state, &keys);

		prev_score = score;
		prev_count_all_asteroids = count_all_asteroids;
		prev_count_close_asteroids = count_close_asteroids;

		spaceship_y = state_info(state)->spaceship->position.y;
		spaceship_x = state_info(state)->spaceship->position.x;	

		score = state_info(state)->score;
		count_all_asteroids = list_size(state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000}));
		count_close_asteroids = list_size(state_objects(state, (Vector2){-ASTEROID_MAX_DIST+spaceship_x,ASTEROID_MAX_DIST+spaceship_y}, (Vector2){ASTEROID_MAX_DIST+spaceship_x,-ASTEROID_MAX_DIST+spaceship_y}));
	}
	//printf("\nall asteroids are: %d and score is %d and close asteroids are %d and \nprev_asteroids are %d and prev score is %d and prev close asteroids are %d\n", count_all_asteroids, score, count_close_asteroids, prev_count_all_asteroids, prev_score, prev_count_close_asteroids);
	TEST_ASSERT((count_all_asteroids == prev_count_all_asteroids-1) && ( score == prev_score/2) && (count_close_asteroids == prev_count_close_asteroids-1));

	//Αστεροειδής - Σφαίρα	
    keys.space = true;

	int count_all_bullets = list_size(state_objects(state, (Vector2){spaceship_x,1000000000000+spaceship_y}, (Vector2){spaceship_x, spaceship_y}));

	int prev_count_all_bullets = count_all_bullets;
	
	while ( ((score != prev_score-10) && (prev_score-10 > 0)) || count_all_bullets != prev_count_all_bullets-1) {
		state_update(state, &keys);

		prev_score = score;
		prev_count_all_asteroids = count_all_asteroids;
		prev_count_all_bullets = count_all_bullets;

		spaceship_y = state_info(state)->spaceship->position.y;
		spaceship_x = state_info(state)->spaceship->position.x;	

		score = state_info(state)->score;
		count_all_asteroids = list_size(state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000}));
		count_all_bullets = list_size(state_objects(state, (Vector2){spaceship_x,1000000000000+spaceship_y}, (Vector2){spaceship_x, spaceship_y}));
	}

	//printf("\nall asteroids are %d and score is %d and all bullets are %d and \nprev_asteroids are %d and prev score is %d and prev all bullets %d\n", 
	//count_all_asteroids, score, count_all_bullets, prev_count_all_asteroids, prev_score, prev_count_all_bullets);
	TEST_ASSERT((count_all_asteroids == prev_count_all_asteroids) && ( score == prev_score-10 || score == 0) && (count_all_bullets == prev_count_all_bullets-1));
	keys.space = false;

}

int* create_int(int value) {

	int* pointer = malloc(sizeof(int));
	*pointer = value;					
	return pointer;

}


int ComparePointer(Pointer a, Pointer b) {

	if (*(int*)a < *(int*)b) 
        return -1;
    else if (*(int*)a > *(int*)b) 
        return 1;
    else 
        return 0;

}


void test_set_find_eq_or_greater() {

	Set set = set_create(ComparePointer, NULL);

	//Προσθέτει 1,3,5,7,9,11,13 στο set
	for (int i = 1; i < 15; i += 2)
		set_insert(set, create_int(i));

	//Έλεγχος συνάρτησης
	TEST_ASSERT(*(int*)set_find_eq_or_greater(set, create_int(10)) == 11);
	TEST_ASSERT(*(int*)set_find_eq_or_greater(set, create_int(9)) == 9);
	TEST_ASSERT(set_find_eq_or_greater(set, create_int(14)) == NULL);

}

void test_set_find_eq_or_smaller() {

	Set set = set_create(ComparePointer, NULL);

	//Προσθέτει 1,3,5,7,9,11,13 στο set
	for (int i = 1; i < 15; i += 2)
		set_insert(set, create_int(i));

	//Έλεγχος συνάρτησης
	TEST_ASSERT(*(int*)set_find_eq_or_smaller(set, create_int(5)) == 5);
	TEST_ASSERT(*(int*)set_find_eq_or_smaller(set, create_int(2)) == 1);
	TEST_ASSERT(set_find_eq_or_smaller(set, create_int(0)) == NULL);

}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{ "test_set_find_eq_or_greater", test_set_find_eq_or_greater},
	{ "test_set_find_eq_or_smaller", test_set_find_eq_or_smaller},

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
