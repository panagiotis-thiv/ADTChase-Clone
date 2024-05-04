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
	//παραπάνω. Το καταφέρνω αυτό με δύο ελέγχους. Πρώτα ελέγχο ότι έχουν φτιαχτεί σε σω-
	//στή απόσταστη από το αρχικό σημείο του διαστημόπλοιου (0,0) και δεύτερον ότι έχουν
	//φτιαχτεί μόνο 6 (αφού ASTEROID_NUM = 6). 

	List objTest1 = state_objects(state, (Vector2){1000000000000,1000000000000}, (Vector2){-1000000000000,-1000000000000});
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

	//Ελέγχω άμα έχει προστεθεί σωστά η ταχύτητα στη θέση

	keys.up = false;
	state_update(state, &keys);
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,SPACESHIP_ACCELERATION}) );

	state_update(state, &keys);
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,SPACESHIP_ACCELERATION+(SPACESHIP_ACCELERATION*SPACESHIP_SLOWDOWN)}) );

	//TODO Rotation check


	//Ελέγχω άμα δουλεύει το πρώτο μέρος της άσκησης 3. Ο τρόπος σίγουρα δεν είναι ο πιο αποτελεσματικός αλλά δουλεύει.
	//Βρίσκω κάθε φορά πόσοι αστεροειδείς υπάρχουν σε όλο το state και μετά καλώ την state_update. Με αυτόν τον τρόπο 
	//ελέγχω και ότι οι αστεροειδείς κουνιούνται αλλά και ότι όταν δεν είναι "κοντά" στο διαστημόπλοιο θα δημιουργηθούν
	//όσοι χρειάζονται ώστε αυτοί που είναι "κοντά" να είναι 6. 
	//Συγκεκριμένα καλώ την state_update μέχρι οι αστεροειδείς να μην είναι 6. Άμα αυτό δεν συνέβαινε ποτέ τότε δεν θα έβγαινε
	//ποτέ από την while. Προεραιτερικό είναι το αποκάτω TEST_ASSERT αλλά το έβαλα για παραπάνω σιγουριά.
	List all_asteroids = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});
	while (list_size(all_asteroids) == 6) {
		state_update(state, &keys);
		all_asteroids = state_objects(state, (Vector2){-1000000000000,1000000000000}, (Vector2){1000000000000,-1000000000000});
	}
	TEST_ASSERT(list_size(all_asteroids) == 7);
	TEST_ASSERT(state_info(state)->score == 1);

	float spaceship_pos = state_info(state)->spaceship->position.y;

	//Έλεγχος δημιουργίας σφαίρας
	//Αρχικά καλούμε την state_objects για αντικείμενα που είναι πάρα πολύ κοντά στο διαστημόπλοιο. Επειδή μάλιστα είναι αρχή ακόμα στο
	//παιχνίδι, δεν γίνεται να υπάρχει τόσο κοντά κάποιος αστεροειδής.
	//Αφού ελέγχουμε ότι είναι 0, πατάμε το space και κάνουμε πάλι το ίδιο. Άμα τώρα μετρήσουμε 1 αντικείμενο τότε θα είναι η σφαίρα
	List bullets = state_objects(state, (Vector2){-10+spaceship_pos, 10+spaceship_pos}, (Vector2){10+spaceship_pos, -10+spaceship_pos});
	TEST_ASSERT(list_size(bullets) == 0);

	keys.space = true;
	state_update(state, &keys); // B_D(Bullet_Delay) = 1
	
	bullets = state_objects(state, (Vector2){10+spaceship_pos, 10+spaceship_pos}, (Vector2){-10+spaceship_pos, -10+spaceship_pos});
	TEST_ASSERT(list_size(bullets) == 1);

	//Η for εξηγείτε από κάτω αναλυτικά γιατί μέχρι 16. Όταν δημιουργείται η σφαίρα είμαστε frames 0, περνάνε 15 και στο 16ο
	//θα δημιουργηθεί η επόμενη σφαίρα
	for (int i = 0; i < 16; i++) {
		state_update(state, &keys);
	}
	// state_update(state, &keys); // B_D = 2 | i = 0 
	// ...
	// state_update(state, &keys); // B_D = 14 | i = 12
	// state_update(state, &keys); // B_D = 15 | i = 13
	// state_update(state, &keys); // B_D = 0 | i = 14
	// state_update(state, &keys); // B_D = 1 + new bullet created | i = 15
	
	spaceship_pos = state_info(state)->spaceship->position.y;

	bullets = state_objects(state, (Vector2){10+spaceship_pos, 10+spaceship_pos}, (Vector2){-10+spaceship_pos, -10+spaceship_pos});
	TEST_ASSERT(list_size(bullets) == 1); //Αφού είναι 1 σημαίνει ότι έχει φύγει η προηγούμενη σφαίρα οπότε δουλεύει και η ταχύτητα της

}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
